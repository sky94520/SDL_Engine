#include "SETileset.h"

NS_SDL_BEGIN

Tileset::Tileset()
	:firstGrid(0)
	,tileWidth(0)
	,tileHeight(0)
	,spacing(0)
	,margin(0)
	,width(0)
	,height(0)
	,tileCount(0)
	,column(0)
{
}
Tileset::~Tileset()
{
}
Tileset*Tileset::create(rapidxml::xml_node<>*root)
{
	auto tileset = new Tileset();
	if(tileset && tileset->init(root))
		tileset->autorelease();
	else
		SDL_SAFE_DELETE(tileset);
	return tileset;
}

Tileset* Tileset::create(rapidxml::xml_node<>*root,rapidxml::xml_node<>*tsxRoot)
{
	auto tileset = new Tileset();

	if (tileset && tileset->init(root,tsxRoot))
		tileset->autorelease();
	else 
		SDL_SAFE_DELETE(tileset);

	return tileset;
}

bool Tileset::init(rapidxml::xml_node<>*root)
{
	//先获取对应的属性
	firstGrid = atoi(root->first_attribute("firstgid")->value());
	
	this->initlize(root);

	return true;
}

bool Tileset::init(rapidxml::xml_node<>*root,rapidxml::xml_node<>*tsxRoot)
{
	firstGrid = atoi(root->first_attribute("firstgid")->value());
	
	this->initlize(tsxRoot);

	return true;
}

Rect Tileset::getRectForGID(int gid)const
{
	gid = gid - this->firstGrid;

	auto currentRow = gid/this->column;
	auto currentFrame = gid%this->column;

	Rect r = Rect(float(margin + (spacing + tileWidth)*currentFrame),
	float(margin + (spacing + tileHeight)*currentRow)
	,(float)tileWidth
	,(float)tileHeight);

	return r;
}
ValueMap Tileset::getPropertiesForGID(int id)
{
	auto iter = _properties.find(id);
	if(iter != _properties.end())
		return iter->second;
	//不存在则返回一个空的valuemap
	return ValueMap();
}
std::unordered_map<int,std::vector<TileAnimation> >& Tileset::getTileAnimations()
{
	return _tileAnimations;
}

const std::unordered_map<int,ValueMap>& Tileset::getProperties()const
{
	return _properties;
}

void Tileset::initlize(rapidxml::xml_node<>* root)
{
	name = root->first_attribute("name")->value();
	tileWidth = atoi(root->first_attribute("tilewidth")->value());
	tileHeight = atoi(root->first_attribute("tileheight")->value());

	tileCount = atoi(root->first_attribute("tilecount")->value());
	column = atoi(root->first_attribute("columns")->value());
	if(root->first_attribute("spacing"))
		spacing = atoi(root->first_attribute("spacing")->value());
	if(root->first_attribute("margin"))
		margin = atoi(root->first_attribute("margin")->value());

	for(auto node = root->first_node();node;node = node->next_sibling())
	{
		if(strcmp(node->name(),"image") == 0)
		{
			this->parseImage(node);
		}
		else if(strcmp(node->name(),"tile") == 0)
		{
			this->parseTile(node);
		}
	}
}

void Tileset::parseImage(rapidxml::xml_node<>*imageRoot)
{
	source = imageRoot->first_attribute("source")->value();
	//TODO
	//auto trans = imageRoot->first_attribute("trans")->value();
}
void Tileset::parseTile(rapidxml::xml_node<>*tileRoot)
{
	//获取id
	std::string idstr = tileRoot->first_attribute("id")->value();
	int id = SDL_atoi(idstr.c_str());
	//解析图块
	for(auto node = tileRoot->first_node();node;node = node->next_sibling())
	{
		//animation
		if(strcmp(node->name(),"animation") == 0)
		{
			this->parseTileAnimation(id,node);
		}
		else if(strcmp(node->name(),"properties") == 0)
		{
			this->parseProperties(id,node);
		}
	}
}
void Tileset::parseProperties(int id,rapidxml::xml_node<>*root)
{
	ValueMap valueMap;
	for(auto node = root->first_node();node;node = node->next_sibling())
	{
		auto key = node->first_attribute("name")->value();
		auto value = node->first_attribute("value")->value();

		valueMap.insert(std::make_pair(key,Value(value)));
	}
	_properties.insert(std::make_pair(id,valueMap));
}
void Tileset::parseTileAnimation(int id,rapidxml::xml_node<>*animationRoot)
{
	std::vector<TileAnimation> tileAnimation;
	for(auto node = animationRoot->first_node();node;node = node->next_sibling())
	{
		int tileid = 0;
		float duration = 0.f;
		for(auto attribute = node->first_attribute();attribute;attribute = attribute->next_attribute())
		{
			if(SDL_strcmp(attribute->name(),"tileid") == 0)
				tileid = SDL_atoi(attribute->value());
			else if(SDL_strcmp(attribute->name(),"duration") == 0)
				duration = (float)SDL_atof(attribute->value());
		}
		tileAnimation.push_back(TileAnimation(tileid,duration));
	}
	_tileAnimations.insert(std::make_pair(id,tileAnimation));
}
NS_SDL_END
