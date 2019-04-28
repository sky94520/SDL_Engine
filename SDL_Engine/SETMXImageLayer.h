#ifndef __SDL_TMXImageLayer_H__
#define __SDL_TMXImageLayer_H__

#include<string>

#include "SENode.h"
#include "SEValue.h"
#include "../rapidxml/rapidxml.hpp"


NS_SDL_BEGIN

class Sprite;
class TMXTiledMap;
class FastTiledMap;

class TMXImageLayer:public Node
{
private:
	friend class TMXTiledMap;
	friend class FastTiledMap;
private:
	ValueMap _properties;
	Sprite* _sprite;
public:
	TMXImageLayer();
	~TMXImageLayer();

	CREATE_FUNC(TMXImageLayer);
	bool init();
	//设置显示精灵，主要在TMXTiledMap中进行设置
	void setSprite(const std::string&image);
	Sprite*getSprite()const;

	ValueMap& getProperies();
};

NS_SDL_END
#endif
