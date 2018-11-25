#include "SELayerColor.h"
#include "SEDirector.h"
#include "SESprite.h"
#include "SETexture.h"
NS_SDL_BEGIN
LayerColor::LayerColor()
	:_sprite(nullptr)
	,_dirty(true)
{
}
LayerColor::~LayerColor()
{
}
LayerColor*LayerColor::create(const Color4B&color)
{
	auto layer = new LayerColor();
	Size visibleSize = Director::getInstance()->getVisibleSize();

	if(layer && layer->init(color,0,0,visibleSize.width,visibleSize.height))
		layer->autorelease();
	else
		SDL_SAFE_DELETE(layer);
	return layer;
}
LayerColor*LayerColor::create(const Color4B&color,float width,float height)
{
	auto layer = new LayerColor();
	if(layer && layer->init(color,0,0,width,height))
		layer->autorelease();
	else
		SDL_SAFE_DELETE(layer);
	return layer;
}
LayerColor*LayerColor::create(const Color4B&color,const Rect&rect)
{
	auto layer = new LayerColor();
	if(layer && layer->init(color,rect.origin.x,rect.origin.y,rect.size.width,rect.size.height))
		layer->autorelease();
	else
		SDL_SAFE_DELETE(layer);
	return layer;
}
LayerColor*LayerColor::create(const Color4B&color,float x,float y,float w,float h)
{
	auto layer = new LayerColor();
	if(layer && layer->init(color,x,y,w,h))
		layer->autorelease();
	else
		SDL_SAFE_DELETE(layer);
	return layer;
}
bool LayerColor::init(const Color4B&color,float x,float y,float w,float h)
{
	_color = color;
	_rect = Rect(x,y,w,h);

	this->setImage(color,x,y,w,h);
	return true;
}
void LayerColor::setColor(const Color4B& color)
{
	if(color.equals(_color))
		return;
	_color = color;
	
	this->setImage(color,_rect.origin.x,_rect.origin.y,_rect.size.width,_rect.size.height);
}
Color4B LayerColor::getColor()const
{
	return _color;
}

Sprite* LayerColor::getInnerSprite()const
{
	return _sprite;
}

void LayerColor::setImage(const Color4B&color,float x,float y,float width,float height)
{
	Texture*texture = nullptr;
	//如果为true，则创建新的精灵
	if(_dirty)
	{
		_dirty = false;
		//先移除原来的精灵
		if(_sprite)
			_sprite->removeFromParent();
		//创建空的texture
		_sprite = Sprite::createWithTexture(SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET, width, height);
		this->addChild(_sprite);
		//获取texture
		texture = _sprite->getTexture();
		texture->setBlendMode(SDL_BLENDMODE_BLEND);
		//设置锚点
		_sprite->setAnchorPoint(Point::ZERO);
	}
	else
		texture = _sprite->getTexture();

	_sprite->setPosition(x,y);
	//获取Renderer
	auto renderer = Director::getInstance()->getRenderer();
	//设置sprite为渲染目标
	renderer->setTarget(texture);
	//保存原先的混合模式
	SDL_BlendMode oldBlendMode;

	renderer->getDrawBlendMode(&oldBlendMode);
	//设置混合模式为SDL_BLENDMODE_NONE
	renderer->setDrawBlendMode(SDL_BLENDMODE_NONE);
	//清除
	renderer->clear();
	//绘制
	auto oldColor = renderer->getDrawColor();
	renderer->setDrawColor(color);
	renderer->fillRect(Rect(0,0,width,height));
	//还原
	renderer->setDrawColor(oldColor);
	renderer->setDrawBlendMode(oldBlendMode);
	renderer->setTarget(nullptr);
}
NS_SDL_END
