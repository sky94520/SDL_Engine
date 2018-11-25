#include "SESprite.h"
#include "SEDirector.h"
#include "SERenderer.h"
#include "SETextureCache.h"
#include "SESpriteFrame.h"
#include "SESpriteFrameCache.h"
#include "SEPoint.h"
#include "SESize.h"

NS_SDL_BEGIN
Sprite::Sprite()
	:_texture(nullptr)
	,_rect(Rect::ZERO)
	,_rectRotated(false)
	,_offsetPosition(Point::ZERO)
	,_originalSize(Size::ZERO)
	,_insideBounds(true)
{
}

Sprite::~Sprite()
{
	SDL_SAFE_RELEASE(_texture);
	//printf("释放 sprite ");
}

Sprite* Sprite::create(const std::string& filename)
{
	Sprite* sprite = new Sprite();

	if(sprite && sprite->initWithFile(filename))
		sprite->autorelease();
	else
		SDL_SAFE_DELETE(sprite);

	return sprite;
}

Sprite* Sprite::create(const std::string& filename,const Rect& rect)
{
	Sprite* sprite = new Sprite();

	if(sprite && sprite->initWithFile(filename,rect))
		sprite->autorelease();
	else
		SDL_SAFE_DELETE(sprite);

	return sprite;
}

Sprite* Sprite::createWithTexture(Texture* texture,const Rect& rect)
{
	Sprite* sprite = new Sprite();

	if(sprite && sprite->initWithTexture(texture,rect))
		sprite->autorelease();
	else
		SDL_SAFE_DELETE(sprite);

	return sprite;
}

Sprite* Sprite::createWithTexture(Texture* texture)
{
	Sprite* sprite = new Sprite();

	if(sprite && sprite->initWithTexture(texture))
		sprite->autorelease();
	else
		SDL_SAFE_DELETE(sprite);

	return sprite;
}

Sprite* Sprite::createWithTexture(Texture* texture,const Rect& rect,bool rotated,const Point& offset,const Size& originalSize)
{
	Sprite* sprite = new Sprite();

	if(sprite && sprite->initWithTexture(texture,rect,rotated,offset,originalSize))
		sprite->autorelease();
	else
		SDL_SAFE_DELETE(sprite);

	return sprite;
}
Sprite* Sprite::createWithSpriteFrame(SpriteFrame* spriteFrame)
{
	auto sprite = new Sprite();

	if(sprite && sprite->initWithSpriteFrame(spriteFrame))
		sprite->autorelease();
	else
		SDL_SAFE_DELETE(sprite);

	return sprite;
}
Sprite* Sprite::createWithSpriteFrameName(const std::string& name)
{
	auto sprite = new Sprite();

	if(sprite && sprite->initWithSpriteFrameName(name))
		sprite->autorelease();
	else
		SDL_SAFE_DELETE(sprite);

	return sprite;
}

Sprite* Sprite::createWithSurface(Surface* surface)
{
	Sprite* sprite = new Sprite();

	if(sprite && sprite->initWithSurface(surface))
		sprite->autorelease();
	else
		SDL_SAFE_DELETE(sprite);

	return sprite;
}

Sprite* Sprite::createWithTexture(Uint32 format,int access,int w,int h)
{
	Sprite* sprite = new Sprite();

	if(sprite && sprite->initWithTexture(format,access,w,h))
		sprite->autorelease();
	else
		SDL_SAFE_DELETE(sprite);

	return sprite;
}

bool Sprite::init()
{
	return true;
}

bool Sprite::initWithFile(const std::string& filename)
{
	//加载Texture
	auto texture = Director::getInstance()->getTextureCache()->addImage(filename);
	
	if(texture != nullptr)
	{
		int w = 0, h = 0;

		texture->query(nullptr,nullptr,&w,&h);

		_rect.size.width = (float)w;
		_rect.size.height = (float)h;
		_originalSize = _rect.size;

		_texture = texture;
		_texture->retain();

		this->setAnchorPoint(Point(0.5f,0.5f));
		this->setContentSize(_originalSize);

		return true;
	}
	return false;
}

bool Sprite::initWithFile(const std::string& filename,const Rect& rect)
{
	auto ret = Sprite::initWithFile(filename);

	if(ret)
	{
		_rect = rect;
		_originalSize = rect.size;
		this->setContentSize(rect.size);

		return true;
	}
	return false;
}

bool Sprite::initWithTexture(Texture* texture)
{
	if(texture != nullptr)
	{
		int w = 0, h = 0;
		texture->query(nullptr,nullptr,&w,&h);

		return Sprite::initWithTexture(texture,Rect(0.f,0.f,(float)w,(float)h));
	}
	return false;
}

bool Sprite::initWithTexture(Texture* texture,const Rect& rect)
{
	if(texture != nullptr)
	{
		return initWithTexture(texture,rect,false,Point::ZERO,rect.size);
	}
	return false;
}

bool Sprite::initWithTexture(Texture* texture,const Rect& rect,bool rotated,const Point& offset,const Size& originalSize)
{
	if(texture != nullptr)
	{
		this->setTexture(texture);

		_rect = rect;
		_rectRotated = rotated;
		_offsetPosition = offset;
		_originalSize = originalSize;
		_contentSize = originalSize;

		this->setAnchorPoint(Point(0.5,0.5));
		return true;
	}
	return true;
}

bool Sprite::initWithSpriteFrame(SpriteFrame* spriteFrame)
{
	if(spriteFrame != nullptr)
	{
		this->setSpriteFrame(spriteFrame);

		return true;
	}
	return false;
}

bool Sprite::initWithSpriteFrameName(const std::string& name)
{
	auto spriteFrame = Director::getInstance()->getSpriteFrameCache()->getSpriteFrameByName(name);
	
	return Sprite::initWithSpriteFrame(spriteFrame);
}

bool Sprite::initWithSurface(Surface* surface)
{
	SDL_SAFE_RELEASE(_texture);

	Renderer* ren = Director::getInstance()->getRenderer();
	_texture = Texture::createWithSurface(ren, surface);
	//保持对_texture的引用
	_texture->retain();
	
	int w = 0, h = 0;
	_texture->query(NULL,NULL,&w,&h);

	_originalSize = Size(w,h);
	_rect.size.width = (float)w;
	_rect.size.height = (float)h;
	//设置contentSize
	this->setContentSize(Size((float)w,(float)h));
	setAnchorPoint(Point(0.5,0.5));

	return true;
}

bool Sprite::initWithTexture(Uint32 format,int access,int w,int h)
{
	SDL_SAFE_RELEASE(_texture);

	Renderer* ren = Director::getInstance()->getRenderer();
	_texture = Texture::create(ren, format, access, w, h);
	_texture->retain();

	_rect.size.width=(float)w;
	_rect.size.height=(float)h;
	_originalSize = Size(w,h);

	setContentSize(Size((float)w,(float)h));
	setAnchorPoint(Point(0.5,0.5));

	return true;
}

Texture* Sprite::getTexture() const
{
	return _texture;
}
/*不建议使用,以后扩展*/
void Sprite::setTexture(Texture* texture)
{
	texture->retain();
	//是否需要删除原先的texture
	if(_texture != nullptr)
		SDL_SAFE_RELEASE(_texture);
	_texture = texture;

	int w = 0, h = 0;

	_texture->query(nullptr,nullptr,&w,&h);

	_rect.size.width = (float)w;
	_rect.size.height = (float)h;
	_originalSize = _rect.size;


	this->setContentSize(_originalSize);
}

void Sprite::setTexture(const std::string& filename)
{
	//加载Texture
	auto texture = Director::getInstance()->getTextureCache()->addImage(filename);

	if(texture != nullptr)
	{
		this->setTexture(texture);
	}
}

void Sprite::draw()
{
	Renderer* renderer = Director::getInstance()->getRenderer();
	//相对坐标系
	Point pos = _position;
	//相对缩放
	float scaleX = _scaleX;
	float scaleY = _scaleY;

	Point parentAnchor;
	Size parentSize;

	for(Node* node = _parent;node != nullptr; node = node->getParent())
	{
		parentAnchor = node->getAnchorPoint();
		parentSize = node->getContentSize();

		pos += node->getPosition() - Point(parentAnchor.x * parentSize.width,parentAnchor.y * parentSize.height);
		
		if (!node->isCascadeScaleEnabled())
			continue;

		scaleX *= node->getScaleX();
		scaleY *= node->getScaleY();
	}
	SDL_RendererFlip flip = SDL_FLIP_NONE;

	if(_flipX && !_flipY)
	{
		flip = SDL_FLIP_HORIZONTAL;
	}
	else if(!_flipX && _flipY)
	{
		flip = SDL_FLIP_VERTICAL;
	}
	SDL_Rect srcRect = {(int)_rect.origin.x,(int)_rect.origin.y,(int)_rect.size.width,(int)_rect.size.height};
	SDL_Rect destRect = {int(pos.x),int(pos.y),int(_rect.size.width * scaleX),int(_rect.size.height * scaleY)};
	
	destRect.x -= int(_anchorPoint.x * _originalSize.width * scaleX);
	destRect.y -= int(_anchorPoint.y * _originalSize.height * scaleY);
	//是否显示
	_insideBounds = renderer->checkVisibility(destRect);
	if (!_insideBounds)
		return;
	Point anchorPointInPoints(_anchorPoint.x * _originalSize.width,_anchorPoint.y * _originalSize.height);

	_textureCommand.init(_globalZOrder, _texture, srcRect, destRect, _rotation, anchorPointInPoints, flip);
	_textureCommand.setOpacity(_displayedOpacity);
	_textureCommand.setColorMod(_colorMod);

	renderer->addCommand(&_textureCommand);
}
//---------------------------------------SpriteFrame相关---------------------------
SpriteFrame* Sprite::getSpriteFrame() const
{
	return SpriteFrame::createWithTexture(_texture,
		_rect,
		_rectRotated,
		_offsetPosition,
		_originalSize,
		_anchorPoint);
}

void Sprite::setSpriteFrame(const std::string& spriteFrameName)
{
	auto spriteFrame = Director::getInstance()->getSpriteFrameCache()->getSpriteFrameByName(spriteFrameName);
	SDL_assert(spriteFrame);
	
	this->setSpriteFrame(spriteFrame);
}

void Sprite::setSpriteFrame(SpriteFrame* spriteFrame)
{
	//各种赋值
	setTexture(spriteFrame->getTexture());

	_rect = spriteFrame->getRect();
	_rectRotated = spriteFrame->isRotated();
	_offsetPosition = spriteFrame->getOffsetPosition();
	_originalSize = spriteFrame->getOriginalSize();
	//_anchorPoint = spriteFrame->getAnchorPoint();

	if(_originalSize != Size::ZERO)
	{
		setContentSize(_originalSize);
	}
	else
	{
		if(_rectRotated)
			setContentSize(Size(_rect.size.height,_rect.size.width));
		else
			setContentSize(_rect.size);
	}
	//设置精灵表位置
	auto texturePackerRect = spriteFrame->getTexturePackerRect();
	_spriteFrameRect = texturePackerRect;

	if (_rectRotated)
	{
		_spriteFrameRect.size  = Size(texturePackerRect.size.height,texturePackerRect.size.width);
	}
}
Rect Sprite::getSpriteFrameRect() const
{
/*	if(_rectRotated)
		return Rect(_offsetPosition,Size(_rect.size.height,_rect.size.width));
	return Rect(_offsetPosition,_rect.size);*/
	return _spriteFrameRect;
}

Rect Sprite::getTextureRect() const
{
	return _rect;
}

void Sprite::setTextureRect(const Rect& rect)
{
	this->setTextureRect(rect,false,rect.size);
}

void Sprite::setTextureRect(const Rect& rect,bool rotated,Size untrimmedSize)
{
	_rectRotated = rotated;
	this->setContentSize(untrimmedSize);

	_rect = rect;
}

Sprite* Sprite::clone() const
{
	Sprite* sprite = Sprite::createWithSpriteFrame(this->getSpriteFrame());

	return sprite;
}

NS_SDL_END
