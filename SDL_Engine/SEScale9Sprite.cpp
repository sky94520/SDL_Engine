#include "SEScale9Sprite.h"
#include "SETextureCache.h"
#include "SEDirector.h"
#include "SETexture.h"
#include "SESprite.h"

NS_SDL_BEGIN
Scale9Sprite::Scale9Sprite()
	:_scale9Sprite(nullptr)
	,_modX(false)
	,_modY(false)
{
	memset(_innerSpriteList, 0, sizeof(_innerSpriteList));
	_cascadeOpacity = true;
}

Scale9Sprite::~Scale9Sprite()
{
	SDL_SAFE_RELEASE_NULL(_scale9Sprite);
}

Scale9Sprite* Scale9Sprite::create(Sprite* sprite)
{
	auto ret = new Scale9Sprite();

	if (ret && ret->init(sprite, Rect::ZERO))
		ret->autorelease();
	else
		SDL_SAFE_DELETE(ret);

	return ret;
}

Scale9Sprite* Scale9Sprite::create(Sprite* sprite, const Rect& capInsets)
{
	auto ret = new Scale9Sprite();

	if (ret && ret->init(sprite, capInsets))
		ret->autorelease();
	else
		SDL_SAFE_DELETE(ret);

	return ret;
}

Scale9Sprite* Scale9Sprite::create(const std::string& file)
{
	auto sprite = new Scale9Sprite();

	if (sprite && sprite->initWithFile(file,Rect::ZERO))
		sprite->autorelease();
	else
		SDL_SAFE_DELETE(sprite);

	return sprite;
}

Scale9Sprite* Scale9Sprite::create(const std::string& file,const Rect& capInsets)
{
	auto sprite = new Scale9Sprite();

	if (sprite && sprite->initWithFile(file,capInsets))
		sprite->autorelease();
	else
		SDL_SAFE_DELETE(sprite);

	return sprite;
}

bool Scale9Sprite::initWithFile(const std::string& file,const Rect& capInsets)
{
	Sprite* sprite = Sprite::create(file);

	if (sprite != nullptr)
		return this->init(sprite,capInsets);

	return false;
}

bool Scale9Sprite::init(Sprite*sprite,const Rect& capInsets)
{
	_scale9Sprite = sprite;
	SDL_SAFE_RETAIN(_scale9Sprite);
	//获取贴图
	Texture*texture = sprite->getTexture();
	Size size = _scale9Sprite->getContentSize();
	this->setContentSize(size);

	if (capInsets.equals(Rect::ZERO))
	{
		_capInsets.origin.x = size.width/3;
		_capInsets.origin.y = size.height/3;
		_capInsets.size.width = size.width/3;
		_capInsets.size.height = size.height/3;

		if ((int)size.width % 3 != 0)
			_modX = true;
		if ((int)size.height % 3 != 0)
			_modY = true;
	}
	else
	{
		_capInsets = capInsets;
	}
	//9宫图
	for (int i = 0;i < 9;i++)
	{
		int x = i % 3;
		int y = i / 3;
		Point offset;
		float w = 0.f,h = 0.f;
		
		switch (x)
		{
		case 0: w = _capInsets.origin.x; offset.x = 0.f; break;
		case 1: w = _capInsets.size.width; offset.x = _capInsets.origin.x; break;
		case 2: 
			offset.x = _capInsets.origin.x + _capInsets.size.width;
			if (_modX)
				offset -= 1;
			w = size.width - offset.x;
			break;
		}
		switch (y)
		{
		case 0: h = _capInsets.origin.y; offset.y = 0.f; break;
		case 1: h = _capInsets.size.height; offset.y = _capInsets.origin.y; break;
		case 2: 
			offset.y = _capInsets.origin.y + _capInsets.size.height;
			if (_modY)
				offset.y -= 1;
			h = size.height - offset.y;
			break;
		}
		//创建精灵
		_innerSpriteList[i] = Sprite::createWithTexture(texture,Rect(offset,Size(w,h)));
		_innerSpriteList[i]->setAnchorPoint(Point::ZERO);
		//设置位置
		auto pos = Point(offset.x,offset.y);

		_innerSpriteList[i]->setPosition(pos);
		this->addChild(_innerSpriteList[i]);
	}
	return true;
}

void Scale9Sprite::setPreferredSize(const Size&preferredSize)
{
	auto size  = _scale9Sprite->getContentSize();
	this->setContentSize(preferredSize);
	//设置缩放
	_scaleX = preferredSize.width / size.width;
	_scaleY = preferredSize.height / size.height;
	
	float scaleX = (preferredSize.width - size.width + _capInsets.size.width) / _capInsets.size.width;
	float scaleY = (preferredSize.height - size.height + _capInsets.size.height) / _capInsets.size.height;

	Rect capInsets = _capInsets;

	capInsets.size.width *= scaleX;
	capInsets.size.height *= scaleY;
	//9宫图
	for (int i = 0;i < 9;i++)
	{
		int x = i % 3;
		int y = i / 3;
		Point offset;
		float w = 0.f,h = 0.f;
		
		switch (x)
		{
		case 0: w = capInsets.origin.x; offset.x = 0.f; break;
		case 1: w = capInsets.size.width; offset.x = capInsets.origin.x; break;
		case 2: 
			offset.x = capInsets.origin.x + capInsets.size.width;
			if (_modX)
				offset.x -= 1;

			w = preferredSize.width - offset.x;
			break;
		}
		switch (y)
		{
		case 0: h = capInsets.origin.y; offset.y = 0.f; break;
		case 1: h = capInsets.size.height; offset.y = capInsets.origin.y; break;
		case 2: 
			offset.y = capInsets.origin.y + capInsets.size.height;

			if (_modY)
				offset.y -= 1;
			h = size.height - offset.y;
			break;
		}
		//设置位置
		auto pos = Point(offset.x ,offset.y);

		_innerSpriteList[i]->setPosition(pos);

		if (i == 1 || i == 7)
			_innerSpriteList[i]->setScaleX(scaleX);
		else if (i == 3 || i == 5)
			_innerSpriteList[i]->setScaleY(scaleY);
		else if (i == 4)
			_innerSpriteList[i]->setScale(scaleX,scaleY);
	}
}

Texture* Scale9Sprite::getTexture() const
{
	return _scale9Sprite->getTexture();
}

void Scale9Sprite::setTexture(Texture*pTexture)
{
	;
}

NS_SDL_END
