#ifndef __SDL_TMXLayer_H__
#define __SDL_TMXLayer_H__
#include<vector>
#include<string>

#include "zlib.h"
#include "base64.h"

#include "SEObject.h"
#include "SELayer.h"

#include "SEValue.h"

#include "../rapidxml/rapidxml.hpp"

NS_SDL_BEGIN
class Sprite;
class TMXTiledMap;
class TMXLayer : public Layer
{
private:
	friend class TMXTiledMap;
private:
	std::string _layerName;
	//保存着该layer解压缩后的数据
	std::vector<int> _data;
	int _width;
	int _height;
	int _offsetX;
	int _offsetY;
	TMXTiledMap*_tiledMap;//保存对tiled map的引用
	ValueMap _properties;
public:
	TMXLayer();
	~TMXLayer();
	static TMXLayer*create(rapidxml::xml_node<>*root);
	bool init(rapidxml::xml_node<>*root);
	//根据坐标获取对应gid
	//pos param 为地图相应坐标
	int getTileGIDAt(const Point&tileCoordinate)const;
	int getTileGIDAt(int tileCoordinateX,int tileCoordinateY)const;
	//获取对应位置的精灵，若没有，则返回空值(使用的是tiled坐标系)
	Sprite* getTileAt(const Point&tileCoordinate);
	Sprite* getTileAt(int tileCoordinateX,int tileCoordinateY);
	//移除 对应位置的精灵
	void removeTileAt(const Point& tileCoordinate);
	//设置对应位置的gid
	void setTileGID(int gid, const Point& tileCoordinate);
	std::string getLayerName()const;
private:
	bool parseData(rapidxml::xml_node<>*root);
	bool parseProperties(rapidxml::xml_node<>*root);

	std::string trim(const std::string&str);
	std::string decode(const std::string&encoding,const std::string&str);
	void uncompression(const std::string&compression,const std::string&decodeIDs);
	void setTiledMap(TMXTiledMap*tiledMap);
};
NS_SDL_END
#endif
