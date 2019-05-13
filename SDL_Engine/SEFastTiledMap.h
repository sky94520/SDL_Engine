#ifndef __SDL_FastTiledMap_H__
#define __SDL_FastTiledMap_H__

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
#include "SETMXImageLayer.h"

#include "../rapidxml/rapidxml.hpp"

NS_SDL_BEGIN
class Tileset;
class FastLayer;
class TMXObjectGroup;
class Texture;

class FastTiledMap : public Node
{
public:
	FastTiledMap();
	~FastTiledMap();
	static FastTiledMap* create(const std::string& filename);
	bool init(const std::string& filename);
	//通过id获取对应的Tileset
	Tileset* getTilesetForGID(int gid) const;
	//获取地图属性
	Value getPropertyForName(const std::string& key) const;
	const ValueMap& getProperties() const;
	//获得图块属性
	ValueMap getTilePropertiesForGID(int gid) const;
	bool getTilePropertiesForGID(int gid, ValueMap** value);
	//通过name获取layer
	Node* getLayer(const std::string& name);
	const Node* getLayer(const std::string& name) const;

	template<typename T>
	T getLayer(const std::string& name) { return dynamic_cast<T>(getLayer(name)); }
	//获取图块大小
	Size getTileSize() const;
	//获取地图大小
	Size getMapSize() const;
	//获取所有对象层
	const std::vector<TMXObjectGroup*>& getObjectGroups() const;
	const TMXObjectGroup* getObjectGroup(const std::string& name) const;

	const std::vector<Tileset*>& getTilesets() const;
private:
	bool parse(const std::string&filename);

	void parseTileset(rapidxml::xml_node<>* tileRoot);
	void parseLayer(rapidxml::xml_node<>* layerRoot);
	void parseImageLayer(rapidxml::xml_node<>* layerRoot);
	void parseProperties(rapidxml::xml_node<>* propRoot);
	void parseObjectGroup(rapidxml::xml_node<>* objectLayer);
	//加载图片
	void loadImage(const std::string& filename);
	void parseProperties(rapidxml::xml_node<>* propertyRoot, ValueMap& properties);
private:
	//保存图块集
	std::vector<Tileset*> _tilesets;
	//地图属性集
	ValueMap _properties;
	//对象集
	std::vector<TMXObjectGroup*> _objectGroups;
	Size _mapSize;
	Size _tileSize;
	//地图方向 目前仅仅支持90
	int _mapOrientation;
	//资源路径
	std::string _resourcePath;
};
NS_SDL_END
#endif
