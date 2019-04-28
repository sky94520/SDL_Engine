#include "SEFastTiledMap.h"
#include "SETileset.h"
#include "SETMXObjectGroup.h"
#include "SEFastLayer.h"
#include "SETexture.h"
#include "SESprite.h"
#include "SEDirector.h"
#include "SETextureCache.h"
#include "SEFileUtils.h"
#include "SEActionInterval.h"
#include "SESpriteFrame.h"
#include "SEAnimationFrame.h"
#include "SEAnimation.h"
#include "SETMXImageLayer.h"
#include "SEStringUtils.h"

NS_SDL_BEGIN

FastTiledMap::FastTiledMap()
	:_mapSize(Size::ZERO)
	,_tileSize(Size::ZERO)
	,_mapOrientation(90)
{
}

FastTiledMap::~FastTiledMap()
{
	_properties.clear();

	//release tileset
	for(auto iter = _tilesets.begin();iter != _tilesets.end();)
	{
		auto tileset = *iter;
		iter = _tilesets.erase(iter);
		tileset->release();
	}
	
	//release object group
	for(auto iter = _objectGroups.begin();iter != _objectGroups.end();)
	{
		auto objectGroup = *iter;
		iter = _objectGroups.erase(iter);
		objectGroup->release();
	}
}

FastTiledMap* FastTiledMap::create(const std::string& filename)
{
	auto tiledMap = new FastTiledMap();

	if(tiledMap && tiledMap->init(filename))
		tiledMap->autorelease();
	else
		SDL_SAFE_DELETE(tiledMap);

	return tiledMap;
}

bool FastTiledMap::init(const std::string& filename)
{
	//TODO:设置锚点
	this->setAnchorPoint(Point(0.f,0.f));

	//从filename读取出资源路径
	int startPos = filename.find_last_of('/');
	if(startPos != -1)
		_resourcePath = filename.substr(0,startPos);

	auto file = FileUtils::getInstance()->fullPathForFilename(filename);
	if(file.empty())
		return false;

	return this->parse(file);
}

Tileset* FastTiledMap::getTilesetForGID(int gid) const
{
	if(gid == 0)
		return nullptr;

	//get the tileset by gid
	for(unsigned int i = 0;i < _tilesets.size();i++)
	{
		auto firstGrid = _tilesets.at(i)->firstGrid;
		auto tileCount = _tilesets.at(i)->tileCount;

		if(gid >= firstGrid && gid < firstGrid + tileCount)
			return _tilesets.at(i);
	}
	return nullptr;
}

Value FastTiledMap::getPropertyForName(const std::string& key)const
{
	if(_properties.find(key) != _properties.end())
		return _properties.at(key);

	return Value();
}

const ValueMap& FastTiledMap::getProperties() const
{
	return _properties;
}

ValueMap FastTiledMap::getTilePropertiesForGID(int gid) const
{
	Tileset* tileset = this->getTilesetForGID(gid);

	if(tileset != nullptr)
	{
		return tileset->getPropertiesForGID(gid - tileset->firstGrid);
	}
	return ValueMap();
}

bool FastTiledMap::getTilePropertiesForGID(int gid, ValueMap** value)
{
	//get tileset
	auto tileset = this->getTilesetForGID(gid);

	if (tileset == nullptr)
		return false;

	//get properties
	auto& properties = tileset->_properties;

	auto iter = properties.find(gid - tileset->firstGrid);
	if(iter != properties.end())
	{
		*value = &iter->second;
		return true;
	}
	return false;
}

Size FastTiledMap::getTileSize() const
{
	return _tileSize;
}

Size FastTiledMap::getMapSize() const
{
	return _mapSize;
}

const std::vector<TMXObjectGroup*>& FastTiledMap::getObjectGroups() const
{
	return _objectGroups;
}

const TMXObjectGroup* FastTiledMap::getObjectGroup(const std::string& name) const
{
	for(auto it = _objectGroups.begin();it != _objectGroups.end();it++)
	{
		auto objectGroup = *it;

		if(objectGroup->getGroupName() == name)
			return objectGroup;
	}
	return nullptr;
}

const std::vector<Tileset*>& FastTiledMap::getTilesets() const
{
	return _tilesets;
}

const Node* FastTiledMap::getLayer(const std::string&name) const
{
	if(name.empty())
		return nullptr;

	auto iter = std::find_if(_children.begin(),_children.end(),[name](Node* node)
	{
		return node->getName() == name;
	});

	if (iter != _children.end())
	{
		return *iter;
	}

	return nullptr;
}

bool FastTiledMap::parse(const std::string& filename)
{
	std::unique_ptr<char> text_ptr = std::move(FileUtils::getInstance()->getUniqueDataFromFile(filename));

	rapidxml::xml_document<> doc;
	doc.parse<0>(text_ptr.get());
	//root 结点 map
	rapidxml::xml_node<>*root = doc.first_node();
	std::string orientation = root->first_attribute("orientation")->value();
	//get render order
	std::string renderorder = root->first_attribute("renderorder")->value();

	auto width = atoi(root->first_attribute("width")->value());
	auto height = atoi(root->first_attribute("height")->value());
	auto tileWidth = atoi(root->first_attribute("tilewidth")->value());
	auto tileHeight = atoi(root->first_attribute("tileheight")->value());
	//仅支持right down朝向
	if(renderorder != "right-down")
		return false;
	//赋值
	_mapSize = Size((float)width,(float)height);
	_tileSize = Size((float)tileWidth,(float)tileHeight);
	//only support orthogonal
	if(orientation == "orthogonal")
		_mapOrientation = 90;
	/*
	else if(orientation == "isometric")
		_mapOrientation = 45;
	*/

	for(auto node = root->first_node();node;node = node->next_sibling())
	{
		std::string name = node->name();
		//解析图块
		if(name == "tileset")
			this->parseTileset(node);
		else if(name == "layer")
			this->parseLayer(node);
		else if(name == "properties")
			this->parseProperties(node);
		else if(name == "objectgroup")
			this->parseObjectGroup(node);
		else if(name == "imagelayer")
			this->parseImageLayer(node);
	}
	text_ptr.reset();

	return true;
}

void FastTiledMap::parseTileset(rapidxml::xml_node<>* tileRoot)
{
	//兼容tmx 1.0
	auto attribute = tileRoot->first_attribute("source");
	Tileset*tileset = nullptr;
	//进行tsx文件的读取
	if (attribute != nullptr)
	{
		auto source = attribute->value();
		//tsx 路径
		std::string filename = FileUtils::getInstance()->fullPathForFilename(source);
		//查找失败，再次进行查找
		if (filename.empty())
		{
			std::string temp = _resourcePath + "/" + source;
			filename = FileUtils::getInstance()->fullPathForFilename(temp);
		}
		if (filename.empty())
			return;
		//进行xml文件的加载
		std::unique_ptr<char> text_ptr = std::move(FileUtils::getInstance()->getUniqueDataFromFile(filename));
		rapidxml::xml_document<> doc;

		doc.parse<0>(text_ptr.get());
		//root 结点 map
		rapidxml::xml_node<>*tsxRoot = doc.first_node();
		//root节点传递给tileset
		tileset = Tileset::create(tileRoot,tsxRoot);
	}
	else
	{
		tileset = Tileset::create(tileRoot);
	}

	if(tileset != nullptr)
	{
		tileset->retain();
		_tilesets.push_back(tileset);
		//尝试直接加载图片资源
		this->loadImage(tileset->source);
	}
}

void FastTiledMap::parseLayer(rapidxml::xml_node<>*layerRoot)
{
	FastLayer* layer = FastLayer::create(layerRoot);
	//TODO:
	//设置父类
	layer->setTiledMap(this);
	//初始化卡马克卷轴
	layer->initCarmark();

	this->addChild(layer);
}

void FastTiledMap::parseImageLayer(rapidxml::xml_node<>* layerRoot)
{
	TMXImageLayer* layer = TMXImageLayer::create();

	this->addChild(layer);
	//对子树操作
	for(auto node = layerRoot->first_node();node;node = node->next_sibling())
	{
		if(SDL_strcmp(node->name(),"image") == 0)
		{
			auto image = node->first_attribute("source")->value();
			this->loadImage(image);
			//创建精灵
			layer->setSprite(image);
		}
		else if(SDL_strcmp(node->name(),"properties") == 0)
		{
			for(auto child = node->first_node();child;child = child->next_sibling())
			{
				std::string key = child->first_attribute("name")->value();
				std::string value = child->first_attribute("value")->value();

				//insert property
				layer->_properties.insert(std::make_pair(key,Value(value)));
			}
		}
	}
	int offsetX = 0, offsetY = 0;
	//获取属性
	for (auto attribute = layerRoot->first_attribute();attribute != nullptr;attribute = attribute->next_attribute())
	{
		auto name = attribute->name();
		if (SDL_strcmp("name", name) == 0)
		{
			layer->setName(attribute->value());
		}
		else if (SDL_strcmp("offsetx", name) == 0)
			offsetX = atoi(attribute->value());
		else if (SDL_strcmp("offsety", name) == 0)
			offsetY = atoi(attribute->value());
	}
	layer->setPosition(offsetX, offsetY);
}

void FastTiledMap::parseProperties(rapidxml::xml_node<>*propRoot)
{
	this->parseProperties(propRoot,_properties);
}

void FastTiledMap::parseObjectGroup(rapidxml::xml_node<>*objectRoot)
{
	auto objectGroup = TMXObjectGroup::create();
	//设置一些基础属性
	for (auto attri = objectRoot->first_attribute();attri != nullptr;
		attri = attri->next_attribute())
	{
		std::string name = std::string(attri->name());

		if (name == "name")
		{
			objectGroup->setGroupName(attri->value());
		}
	}
	//对对象进行遍历 生成object
	for (auto node = objectRoot->first_node();node != nullptr;
		node = node->next_sibling())
	{
		ValueMap dict;
		//遍历属性
		for (auto attri = node->first_attribute();attri != nullptr;
			attri = attri->next_attribute())
		{
			auto key = attri->name();
			auto value = attri->value();

			dict.insert(std::make_pair(key,Value(value)));
		}
		for (auto child = node->first_node();child != nullptr;child = child->next_sibling())
		{
			if (SDL_strcmp(child->name(),"properties") == 0)
			{
				ValueMap properties;

				this->parseProperties(child,properties);
				dict.insert(std::make_pair("properties",Value(properties)));
			}
			else
			{
				std::string type = child->name();
				ValueVector pointArray;
				std::stringstream pointStream;

				for (auto attri = child->first_attribute();attri != nullptr;
				attri = attri->next_attribute())
				{
					if (SDL_strcmp(attri->name(),"points") == 0)
					{
						pointStream.clear();
						pointStream<<attri->value();
						
						std::string pointPair;

						while (std::getline(pointStream,pointPair,' '))
						{
							ValueMap pointDict;
							int x = 0,y = 0;
							SDL_sscanf(pointPair.c_str(),"%d,%d",&x,&y);
							pointDict["x"] = x;
							pointDict["y"] = y;

							pointArray.push_back(Value(pointDict));
						}
					}
				}//end
				dict.insert(std::make_pair(type,Value(pointArray)));
			}
		}
		objectGroup->addObject(dict);
	}

	objectGroup->retain();
	_objectGroups.push_back(objectGroup);
}

void FastTiledMap::loadImage(const std::string&filename)
{
	//尝试直接加载图片资源
	auto fullPath = FileUtils::getInstance()->fullPathForFilename(filename);
	//如果没有则尝试在当前目录下进行加载
	if(fullPath.empty() && !_resourcePath.empty())
	{
		fullPath = _resourcePath + "/" + filename;
	}
	Director::getInstance()->getTextureCache()->addImage(fullPath,filename);
}

void FastTiledMap::parseProperties(rapidxml::xml_node<>* propRoot,ValueMap& properties)
{
	for(auto node = propRoot->first_node();node;node = node->next_sibling())
	{
		std::string key;
		std::string value;
		std::string type = "string";
		//遍历属性
		for (auto attri = node->first_attribute();attri != nullptr
			;attri = attri->next_attribute())
		{
			if (SDL_strcmp(attri->name(),"name") == 0)
			{
				key = std::string(attri->value());
			}
			else if (SDL_strcmp(attri->name(),"value") == 0)
			{
				value = std::string(attri->value());
			}
			else if (SDL_strcmp(attri->name(),"type") == 0)
			{
				type = std::string(attri->value());
			}
		}
		//进行赋值
		if (type == "bool" && value == "true")
		{
			properties.insert(std::make_pair(key,Value(true)));
		}
		else if (type == "bool" && value == "false")
		{
			properties.insert(std::make_pair(key,Value(false)));
		}
		else if (type == "float")
		{
			properties.insert(std::make_pair(key,Value(atof(value.c_str()))));
		}
		else
		{
			properties.insert(std::make_pair(key,Value(value)));
		}
	}
}

NS_SDL_END
