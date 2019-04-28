#include "SETMXLayer.h"
#include "SESprite.h"
#include "SENode.h"
#include "SETMXTiledMap.h"
#include "SETileset.h"

NS_SDL_BEGIN

TMXLayer::TMXLayer()
	:_layerName("")
	,_width(0)
	,_height(0)
	,_offsetX(0)
	,_offsetY(0)
	,_tiledMap(nullptr)
{
}
TMXLayer::~TMXLayer()
{
}
TMXLayer*TMXLayer::create(rapidxml::xml_node<>*root)
{
	auto layer = new TMXLayer();
	if(layer && layer->init(root))
		layer->autorelease();
	else
		SDL_SAFE_DELETE(layer);
	return layer;
}
bool TMXLayer::init(rapidxml::xml_node<>*root)
{
	bool visible = true;
	//遍历属性
	for (auto attribute = root->first_attribute();attribute != nullptr;attribute = attribute->next_attribute())
	{
		std::string sName = attribute->name();
		const char* value = attribute->value();

		if (sName == "name")
			_layerName = value;
		else if (sName == "width")
			_width = atoi(value);
		else if (sName == "height")
			_height = atoi(value);
		else if (sName == "offsetx")
			_offsetX = atoi(value);
		else if (sName == "offsety")
			_offsetY = atoi(value);
		else if (sName == "visible")
			visible = atoi(value) != 0;
	}

	this->setName(_layerName);
	this->setVisible(visible);

	bool bRet = false;
	for(auto node = root->first_node();node;node = node->next_sibling())
	{
		if (SDL_strcmp(node->name(),"data") == 0)
			bRet |= parseData(node);
		else if(SDL_strcmp(node->name(),"properties") == 0)
			bRet |= this->parseProperties(node);
	}

	return bRet;
}
int TMXLayer::getTileGIDAt(const Point&pos)const
{
	int x = (int)pos.x;
	int y = (int)pos.y;

	return getTileGIDAt(x,y);
}

int TMXLayer::getTileGIDAt(int tileCoordinateX,int tileCoordinateY)const
{
	if(tileCoordinateY >= 0 && tileCoordinateY < _height 
		&& tileCoordinateX >=0 && tileCoordinateX < _width)
	{
		int z = (int)(tileCoordinateX + tileCoordinateY * _width);
		return _data[z];
	}
	return 0;
}

Sprite*TMXLayer::getTileAt(const Point&pos)
{
	int x = (int)pos.x;
	int y = (int)pos.y;

	return getTileAt(x,y);
}

Sprite* TMXLayer::getTileAt(int tileCoordinateX,int tileCoordinateY)
{
	if(tileCoordinateX < _width && tileCoordinateY < _height 
		&& tileCoordinateX >= 0 &&tileCoordinateY >= 0)
	{
		int gid = this->getTileGIDAt(tileCoordinateX,tileCoordinateY);
		//gid不为空时才会又对应的精灵
		if(gid)
		{
			int z = tileCoordinateX + tileCoordinateY * _width;
			Sprite*tile = this->getChildByTag<Sprite*>(z);
			return tile;
		}
	}
	//SDL_assert(pos.x < _width && pos.y < _height && pos.x > 0 && pos.y > 0);
	return nullptr;
}

void TMXLayer::removeTileAt(const Point& pos)
{
	int gid = this->getTileGIDAt(pos);
	if(gid)
	{
		int z = (int)(pos.x + pos.y * _width);
		//设置对应位置为空
		_data[z] = 0;
		//获取对应的精灵
		Sprite*tileSprite = this->getChildByTag<Sprite*>(z);
		if(tileSprite)
			tileSprite->removeFromParent();
	}
}
void TMXLayer::setTileGID(int gid, const Point& pos)
{
	//获取对应位置的gid
	auto oldGid = this->getTileGIDAt(pos);
	if(oldGid == gid)
		return;
	//清除掉原来的精灵
	if(gid == 0)
	{
		this->removeTileAt(pos);
	}
	else
	{
		//暂时不支持
		//新建texture
		if(oldGid == 0)
		{
			SDL_assert(1 == 0);
		}
		else
		{
			//更新精灵
			Sprite*tileSprite = this->getTileAt(pos);
			Tileset*tileset = _tiledMap->getTilesetForGID(gid);
			Rect r = tileset->getRectForGID(gid);
			//停止动画
			tileSprite->stopAllActions();
			tileSprite->setTextureRect(r,false,tileSprite->getContentSize());
			//更新data
			int x = (int)pos.x;
			int y = (int)pos.y;
			int z = x +y * _width;
			_data[z] = gid;
		}
	}
}
std::string TMXLayer::getLayerName()const
{
	return _layerName;
}
bool TMXLayer::parseData(rapidxml::xml_node<>*root)
{
	std::string encoding = root->first_attribute("encoding")->value();
	std::string compression = root->first_attribute("compression")->value();
	std::string data = root->value();
	std::string decodeIDs;
	//判断encoding
	decodeIDs = this->decode(encoding,trim(data));
	this->uncompression(compression,decodeIDs);
	return true;
}

bool TMXLayer::parseProperties(rapidxml::xml_node<>*root)
{
	for(auto node = root->first_node();node;node = node->next_sibling())
	{
		std::string key = node->first_attribute("name")->value();
		std::string value = node->first_attribute("value")->value();

		_properties.insert(std::make_pair(key,Value(value)));
	}
	return true;
}

std::string TMXLayer::trim(const std::string&str)
{
	//分配一个同样大小的字符数组
	char*text = new char[str.length() + 1];
	for(unsigned int i=0;i<strlen(text);i++)
		text[i] = '\0';
	int i=0;//text指针
	for(unsigned int j=0;j < str.length();)
	{
		auto ch = str.at(j++);
		if(ch == '\t' || ch == '\n' || ch == '\r' || ch == ' ')
		{
			;
		}
		else
		{
			text[i++] = ch;
		}
	}
	text[i] = '\0';
	std::string trimstr = std::string(text);
	delete[] text;
	return trimstr;
}
std::string TMXLayer::decode(const std::string&encoding,const std::string&str)
{
	std::string decodeIDs;
	if(encoding == "base64")
	{
		decodeIDs = base64_decode(str);
	}
	return decodeIDs;
}
void TMXLayer::uncompression(const std::string&compression,const std::string&decodeIDs)
{
	if(compression == "zlib")
	{
		uLongf numGids = _width * _height*sizeof(unsigned);
		//后四分之三为空，不知道为什么
		auto gids = std::vector<unsigned>(numGids);
		uncompress((Bytef*)&gids[0],&numGids,(const Bytef*)decodeIDs.c_str(),decodeIDs.size());
		//赋值
		_data = std::vector<unsigned>(_width * _height);
		std::copy(gids.begin(), gids.begin() + _width * _height, _data.begin());
	}
}
void TMXLayer::setTiledMap(TMXTiledMap*tiledMap)
{
	_tiledMap = tiledMap;
}
NS_SDL_END
