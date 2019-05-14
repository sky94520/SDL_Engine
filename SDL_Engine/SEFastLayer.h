#ifndef __SDL_FastLayer_H__
#define __SDL_FastLayer_H__
#include<vector>
#include<string>

#include "zlib.h"
#include "base64.h"

#include "SEObject.h"
#include "SELayer.h"

#include "SEValue.h"
#include "SERenderCommand.h"

#include "../rapidxml/rapidxml.hpp"

NS_SDL_BEGIN

class Sprite;
class FastTiledMap;
class Texture;

class FastLayer : public Layer
{
public:
	FastLayer();
	~FastLayer();

	static FastLayer*create(rapidxml::xml_node<>*root);
	bool init(rapidxml::xml_node<>*root);

	virtual void draw();

	//根据坐标获取对应gid
	int getTileGIDAt(const Point& tileCoordinate) const;
	int getTileGIDAt(int tileCoordinateX,int tileCoordinateY) const;

	//移除对应位置的精灵
	void removeTileAt(const Point& tileCoordinate);

	//设置对应位置的gid
	void setTileGID(int gid, const Point& tileCoordinate);

	std::string getLayerName() const;
private:
	//初始化卡马克卷轴需要的变量
	bool initCarmark();
	//初始化缓冲区
	void initBuffer();
	//绘制 目前由draw()调用
	void fastDraw(int x, int y);
	void drawRegion(int srcX, int srcY, int width, int height, int destX, int destY);
	void drawTile(Renderer* renderer, int id, int destX, int destY);

	bool scroll(int x, int y);
	void updateBuffer(int x, int y);
	void copyBufferX(int indexMapX, int indexMapY, int tileColNum, int destX, int destY);
	void copyBufferY(int indexMapX, int indexMapY, int tileRowNum, int destX, int destY);

	//获得切割线所在的图块索引
	int getIndexCarmarkX() const;
	int getIndexCarmarkY() const;

	//获得切割线的在缓冲区的位置
	int getBufferCarmarkX() const;
	int getBufferCarmarkY() const;

	//获取缓冲区后面的索引
	int getIndexBufLastX() const;
	int getIndexBufLastY() const;

	//获得当前缓冲区去掉切割线的图块个数
	int getCarTileRowNum() const;
	int getCarTileColNum() const;
private:
	bool parseData(rapidxml::xml_node<>*root);
	bool parseProperties(rapidxml::xml_node<>*root);

	std::string trim(const std::string&str);
	std::string decode(const std::string&encoding,const std::string&str);
	void uncompression(const std::string&compression,const std::string&decodeIDs);
	void setTiledMap(FastTiledMap* tiledMap);
private:
	friend class FastTiledMap;
private:
	std::string _layerName;
	//保存着该layer解压缩后的数据
	std::vector<unsigned> _data;
	//该层的宽高的图块个数
	int _width;
	int _height;
	int _offsetX;
	int _offsetY;

	//保存对tiled map的引用
	FastTiledMap* _tiledMap;
	ValueMap _properties;

	//缓冲区尺寸
	int _bufferWidth;
	int _bufferHeight;

	//缓冲区瓦片个数
	int _bufferRowTileNum;
	int _bufferColTileNum;

	//瓦片宽度和高度
	int _tileWidth;
	int _tileHeight;
	//缓冲区增加的额外大小
	Size _extraSize;

	Texture* _buffer;

	//地图尺寸 - 缓冲区尺寸
	int _deltaWidth;
	int _deltaHeight;

	//地图在缓冲区的X、Y轴的偏移量，范围在[0, _deltaWidth | _deltaHeight]
	int _mapOffsetX;
	int _mapOffsetY;

	//缓冲区切割线
	int _carmarkX;
	int _carmarkY;

	//四个渲染命令 即最多渲染四次
	TextureCommand _textureCmds[4];
	unsigned _cmdIndex;

	//记录上一次的位置
	Point _lastPosition;

	int _otherPosX;
	int _otherPosY;
};
NS_SDL_END
#endif
