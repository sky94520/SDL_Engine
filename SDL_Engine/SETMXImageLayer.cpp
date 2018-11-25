#include "SETMXImageLayer.h"
#include "SESprite.h"
#include "SEPoint.h"

NS_SDL_BEGIN
TMXImageLayer::TMXImageLayer()
	:_sprite(nullptr)
{
}
TMXImageLayer::~TMXImageLayer()
{
}
bool TMXImageLayer::init()
{
	return true;
}
void TMXImageLayer::setSprite(const std::string& image)
{
	_sprite = Sprite::create(image);
	_sprite->setAnchorPoint(Point::ZERO);

	auto size = _sprite->getContentSize();

	this->addChild(_sprite);
	this->setContentSize(size);
	this->setAnchorPoint(Point::ZERO);
}

Sprite*TMXImageLayer::getSprite()const
{
	return _sprite;
}

ValueMap& TMXImageLayer::getProperies()
{
	return _properties;
}
NS_SDL_END
