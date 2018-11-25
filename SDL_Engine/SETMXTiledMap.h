#ifndef __SDL_TMXTiledMap_H__
#define __SDL_TMXTiledMap_H__
#include<vector>
#include<map>
#include<cmath>
#include<string>
#include<memory>
#include<algorithm>
#include<sstream>

#include "SEObject.h"
#include "SENode.h"
#include "SEValue.h"
#include "SETMXLayer.h"
#include "SETMXImageLayer.h"

#include "../rapidxml/rapidxml.hpp"

NS_SDL_BEGIN
class Tileset;
class TMXLayer;
class TMXObjectGroup;
class Animate;
class Texture;

class TMXTiledMap : public Node
{
private:
	//保存图块集
	std::vector<Tileset*> _tilesets;
	//地图属性集
	ValueMap _properties;
	//对象集
	std::vector<TMXObjectGroup*> _objectGroups;
	Size _mapSize;
	Size _tileSize;
	int _mapOrientation;//地图方向
	std::string _resourcePath;//资源路径
public:
	TMXTiledMap();
	~TMXTiledMap();
	static TMXTiledMap*create(const std::string&filename);
	bool init(const std::string&filename);
	//通过id获取对应的Tileset
	Tileset*getTilesetForGID(int gid)const;
	//获取地图属性
	Value getPropertyForName(const std::string&key)const;
	ValueMap &getProperties();
	//获得图块属性
	ValueMap getTilePropertiesForGID(int gid)const;
	bool getTilePropertiesForGID(int gid, ValueMap** value);
	//通过name获取layer
	Node*getLayer(const std::string&name);
	template<typename T>
	T getLayer(const std::string&name){return dynamic_cast<T>(getLayer(name));}
	//获取图块大小
	Size getTileSize()const;
	//获取地图大小
	Size getMapSize()const;
	//获取所有对象层
	std::vector<TMXObjectGroup*>&getObjectGroups();
	TMXObjectGroup*getObjectGroup(const std::string&name);

	const std::vector<Tileset*>& getTilesets()const;
private:
	bool parse(const std::string&filename);

	void parseTileset(rapidxml::xml_node<>*tileRoot);
	void parseLayer(rapidxml::xml_node<>*layerRoot);
	void parseImageLayer(rapidxml::xml_node<>*layerRoot);
	void parseProperties(rapidxml::xml_node<>*propRoot);
	void parseObjectGroup(rapidxml::xml_node<>*objectLayer);
	//加载图片
	void loadImage(const std::string&filename);
	void parseProperties(rapidxml::xml_node<>*propertyRoot,ValueMap&properties);
	//尝试创建动画
	Animate*tryToCreateAnimate(int id,Tileset*tileset);
};
NS_SDL_END
#endif
