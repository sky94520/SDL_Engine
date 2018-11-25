#include "SELoadingBar.h"
#include "SEDirector.h"
#include "SERenderer.h"
#include "SETextureCache.h"
#include "SESprite.h"
NS_SDL_BEGIN
LoadingBar::LoadingBar()
	:_sprite(nullptr)
	,_percent(1.f)
	,_dir(LoadingBarDir::kLoadingBar_Right)
{
	this->setCascadeOpacityEnabled(true);
}

LoadingBar::~LoadingBar()
{
}

LoadingBar*LoadingBar::create(Sprite* sprite, float percent, LoadingBarDir dir)
{
	auto widget = new LoadingBar();

	if(widget && widget->init(sprite, percent, dir))
		widget->autorelease();
	else
		SDL_SAFE_DELETE(widget);

	return widget;
}
bool LoadingBar::init(Sprite* sprite, float percent, LoadingBarDir dir)
{
	_sprite = sprite;
	Size size = sprite->getContentSize();
	this->addChild(_sprite);
	//根据类型设置位置
	if(dir == LoadingBarDir::kLoadingBar_Left)
	{
		_sprite->setAnchorPoint(Point(0.f,0.5f));
		_sprite->setPosition(0, size.height / 2);
	}
	else if(dir == LoadingBarDir::kLoadingBar_Right)
	{
		_sprite->setAnchorPoint(Point(1.f, 0.5f));
		_sprite->setPosition(size.width,size.height / 2);
	}
	_dir = dir;
	_rect = _sprite->getTextureRect();

	this->setContentSize(size);

	return true;
}

void LoadingBar::draw()
{
	Rect rect = _rect;
	
	rect.size.width = _percent * rect.size.width;
	_sprite->setTextureRect(rect);

	Node::draw();
}
NS_SDL_END
