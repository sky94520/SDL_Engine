#include "UIAbstractCheckButton.h"
#include "../SESprite.h"
#include "../SETextureCache.h"
NS_SDL_BEGIN


namespace ui
{
const int AbstractCheckButton::INTERVAL_TAG = 100;

AbstractCheckButton::AbstractCheckButton()
	:_backgroundSprite(nullptr)
	,_backgroundSelectedSprite(nullptr)
	,_crossSprite(nullptr)
	,_backgroundDisabledSprite(nullptr)
	,_crossDisabledSprite(nullptr)
	,_selected(false)
{
}
AbstractCheckButton::~AbstractCheckButton()
{
}
bool AbstractCheckButton::init(const std::string&background
			,const std::string&backgroundSelected
			,const std::string&cross
			,const std::string&backgroundDisabled
			,const std::string&crossDisabled
			,TextureResType resType)
{
	_resType = _resType;
	//加载资源
	this->loadTextureBackground(background);
	this->loadTextureBackgroundSelected(backgroundSelected);
	this->loadTextureCross(cross);
	this->loadTextureBackgroundDisabled(backgroundDisabled);
	this->loadTextureCrossDisabled(crossDisabled);
	//设置显示状态
	if(_backgroundSelectedSprite)
		_backgroundSelectedSprite->setVisible(false);
	if(_crossSprite)
		_crossSprite->setVisible(false);
	if(_backgroundDisabledSprite)
		_backgroundDisabledSprite->setVisible(false);
	if(_crossDisabledSprite)
		_crossDisabledSprite->setVisible(false);
	//接收事件
	setTouchEnabled(true);

	return true;
}
bool AbstractCheckButton::init(const std::string&background
			,const std::string&cross
			,TextureResType resType)
{
	_resType = resType;
	this->loadTextureBackground(background);
	this->loadTextureCross(cross);
	//设置显示状态
	if(_crossSprite)
		_crossSprite->setVisible(false);
	//接收事件
	setTouchEnabled(true);
	return true;
}
void AbstractCheckButton::loadTextureBackground(const std::string&background)
{
	if(background.empty())
		return;
	//是否需要新建精灵
	if(_backgroundSprite == nullptr)
	{
		_backgroundSprite = Sprite::create();
		this->addChild(_backgroundSprite,-1);
	}
	//加载新精灵
	switch(_resType)
	{
	case TextureResType::LOCAL:
		{
			_backgroundSprite->setTexture(background);
		}break;
	case TextureResType::XML:
		{
			_backgroundSprite->setSpriteFrame(background);
		}break;
	}
	Size size = _backgroundSprite->getContentSize();
	this->setContentSize(size);

	_backgroundSprite->setPosition(size.width*.5f,size.height*.5f);
}
void AbstractCheckButton::loadTextureBackgroundSelected(const std::string&backgroundSelected)
{
	if(backgroundSelected.empty())
		return;
	//是否需要新建精灵
	if(_backgroundSelectedSprite == nullptr)
	{
		_backgroundSelectedSprite = Sprite::create();
		this->addChild(_backgroundSelectedSprite,1);
	}
	//加载新精灵
	switch(_resType)
	{
	case TextureResType::LOCAL:
		{
			_backgroundSelectedSprite->setTexture(backgroundSelected);
		}break;
	case TextureResType::XML:
		{
			_backgroundSelectedSprite->setSpriteFrame(backgroundSelected);
		}break;
	}
	Size s = _backgroundSprite->getContentSize();
	_backgroundSelectedSprite->setPosition(s.width*.5f,s.height*.5f);
}
void AbstractCheckButton::loadTextureCross(const std::string&cross)
{
	if(cross.empty())
		return;
	//是否需要新建精灵
	if(_crossSprite == nullptr)
	{
		_crossSprite = Sprite::create();
		this->addChild(_crossSprite,INTERVAL_TAG);
	}
	//加载新精灵
	switch(_resType)
	{
	case TextureResType::LOCAL:
		{
			_crossSprite->setTexture(cross);
		}break;
	case TextureResType::XML:
		{
			_crossSprite->setSpriteFrame(cross);
		}break;
	}
	Size s = _backgroundSprite->getContentSize();
	_crossSprite->setPosition(s.width*.5f,s.height*.5f);
}
void AbstractCheckButton::loadTextureBackgroundDisabled(const std::string&backDisabled)
{
	if(backDisabled.empty())
		return;
	//是否需要新建精灵
	if(_backgroundDisabledSprite == nullptr)
	{
		_backgroundDisabledSprite = Sprite::create();
		this->addChild(_backgroundDisabledSprite,-1);
	}
	//加载新精灵
	switch(_resType)
	{
	case TextureResType::LOCAL:
		{
			_backgroundDisabledSprite->setTexture(backDisabled);
		}break;
	case TextureResType::XML:
		{
			_backgroundDisabledSprite->setSpriteFrame(backDisabled);
		}break;
	}
	Size s = _backgroundSprite->getContentSize();
	_backgroundDisabledSprite->setPosition(s.width*.5f,s.height*.5f);
}
void AbstractCheckButton::loadTextureCrossDisabled(const std::string&crossDisabled)
{
	if(crossDisabled.empty())
		return;
	//是否需要新建精灵
	if(_crossDisabledSprite == nullptr)
	{
		_crossDisabledSprite = Sprite::create();
		this->addChild(_crossDisabledSprite,INTERVAL_TAG);
	}
	//加载新精灵
	switch(_resType)
	{
	case TextureResType::LOCAL:
		{
			_crossDisabledSprite->setTexture(crossDisabled);
		}break;
	case TextureResType::XML:
		{
			_crossDisabledSprite->setSpriteFrame(crossDisabled);
		}break;
	}
	Size s = _backgroundSprite->getContentSize();
	_crossDisabledSprite->setPosition(s.width*.5f,s.height*.5f);
}

bool AbstractCheckButton::isSelected()const
{
	return _selected;
}

void AbstractCheckButton::setSelected(bool selected)
{
	if(_selected == selected)
		return ;
	_selected = selected;
	_crossSprite->setVisible(selected);
}

Sprite* AbstractCheckButton::getCrossSprite()const
{
	return _crossSprite;
}

void AbstractCheckButton::setTouchEnabled(bool enable)
{
	bool curEnabled = this->getTouchEnabled();
	if(curEnabled == enable)
		return ;
	Widget::setTouchEnabled(enable);
	//当前可用
	if(enable)
	{
		if(_crossDisabledSprite)
			_crossDisabledSprite->setVisible(false);
		if(_backgroundDisabledSprite)
			_backgroundDisabledSprite->setVisible(false);
		_backgroundSprite->setVisible(true);
	}
	else
	{
		if(_crossDisabledSprite)
			_crossDisabledSprite->setVisible(true);
		if(_backgroundDisabledSprite)
			_backgroundDisabledSprite->setVisible(true);
		_backgroundSprite->setVisible(false);
	}
}
}//end namespace ui
NS_SDL_END
