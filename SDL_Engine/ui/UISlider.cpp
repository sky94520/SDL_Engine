#include "UISlider.h"
#include "../SEScale9Sprite.h"
#include "../SESprite.h"
#include "../SETouch.h"

NS_SDL_BEGIN
namespace ui
{

Slider::Slider()
	:_bar(nullptr)
	,_progressBar(nullptr)
	
	,_slidBallNormalSprite(nullptr)
	,_slidBallPressedSprite(nullptr)
	,_slidBallDisabledSprite(nullptr)

	,_barLength(0)
	,_percent(0)
	,_maxPercent(100)
	,_callback(nullptr)
{
}

Slider::~Slider()
{
}

Slider* Slider::create()
{
	auto slider = new Slider();

	if (slider && slider->init())
		slider->autorelease();
	else
		SDL_SAFE_DELETE(slider);

	return slider;
}

Slider* Slider::create(Scale9Sprite* bar, Sprite* slidBallNormal)
{
	auto slider = new Slider();

	if (slider && slider->init(bar, slidBallNormal))
		slider->autorelease();
	else
		SDL_SAFE_DELETE(slider);

	return slider;
}

bool Slider::init()
{
	return true;
}

bool Slider::init(Scale9Sprite* bar, Sprite* slidBallNormal)
{
	this->loadBarSprite(bar);
	this->loadSlidBallSpriteNormal(slidBallNormal);

	this->setTouchEnabled(true);

	return true;
}

void Slider::loadBarSprite(Scale9Sprite* bar)
{
	if (bar == nullptr || bar->getParent() != nullptr)
	{
		LOG("bar is null or bar->getParent() is not null\n");
		return ;
	}
	if (_bar != nullptr)
	{
		_bar->removeFromParent();
	}
	_bar = bar;
	auto size = _bar->getContentSize();
	_barLength = size.width;

	this->setContentSize(size);

	this->addChild(_bar, -1);
	_bar->setPosition(size.width * 0.5f, size.height * 0.5f);
}

void Slider::loadProgressBarSprite(Scale9Sprite* progressBar)
{
	if (progressBar == nullptr || progressBar->getParent() != nullptr)
	{
		LOG("progressBar is null or progressBar->getParent() is not null\n");
	}
	//添加
	if (_progressBar != nullptr)
	{
		_progressBar->removeFromParent();
	}
	_progressBar = progressBar;
	this->addChild(_progressBar, 0);
	_progressBar->setAnchorPoint(Point(0.f, 0.f));
	_progressBar->setPreferredSize(_bar->getContentSize());
}

void Slider::loadSlidBallSpriteNormal(Sprite* slidBallNormalSprite)
{
	if (slidBallNormalSprite == nullptr || slidBallNormalSprite->getParent() != nullptr)
	{
		LOG("slidBallNormalSprite is null or slidBallNormalSprite->getParent() is not null\n");
	}
	if (_slidBallNormalSprite != nullptr)
		_slidBallNormalSprite->removeFromParent();

	_slidBallNormalSprite = slidBallNormalSprite;
	this->addChild(_slidBallNormalSprite, 1);
	//TODO:设置位置
	_slidBallNormalSprite->setPositionY(_bar->getPositionY());
}

void Slider::loadSlidBallSpritePressed(Sprite* slidBallPressedSprite)
{
	if (slidBallPressedSprite == nullptr || slidBallPressedSprite->getParent() != nullptr)
	{
		LOG("slidBallPressedSprite is null or slidBallPressedSprite->getParent() is not null\n");
	}
	if (_slidBallPressedSprite != nullptr)
		_slidBallPressedSprite->removeFromParent();

	_slidBallPressedSprite = slidBallPressedSprite;
	this->addChild(_slidBallPressedSprite, 1);
	//TODO:设置位置
	_slidBallPressedSprite->setPositionY(_bar->getPositionY());
}

void Slider::loadSlidBallSpriteDisabled(Sprite* slidBallDisabledSprite)
{
	if (slidBallDisabledSprite == nullptr || slidBallDisabledSprite->getParent() != nullptr)
	{
		LOG("slidBallDisabledSprite is null or slidBallDisabledSprite->getParent() is not null\n");
	}
	if (_slidBallDisabledSprite != nullptr)
		_slidBallDisabledSprite->removeFromParent();

	_slidBallDisabledSprite = slidBallDisabledSprite;
	this->addChild(_slidBallDisabledSprite, 1);

	_slidBallDisabledSprite->setPositionY(_bar->getPositionY());
}

void Slider::addEventListener(const ccSliderCallback& callback)
{
	_callback = callback;
}

float Slider::getPercent() const
{
	return _percent;
}

void Slider::setPercent(float value) 
{
	if (_percent == value)
		return ;
	if (value > _maxPercent)
		value = _maxPercent;
	else if (value < 0)
		value = 0;

	_percent = value;
	//设置显示
	this->updatePercent();
}

float Slider::getMaxPercent() const
{
	return _maxPercent;
}

void Slider::setMaxPercent(float percent)
{
	if (_maxPercent == percent)
		return ;

	_maxPercent = percent;
	if (_percent > _maxPercent)
		_percent = _maxPercent;
	//更新显示
	this->updatePercent();
}

bool Slider::touchBeganHook(Touch*touch,SDL_Event*event)
{
	//是否点击了按钮
	//转化为相对于父类的坐标
	auto pos = _startPos = touch->getLocation();
	pos = this->convertToNodeSpace(pos);
	//获取包围盒
	auto rect = _slidBallNormalSprite->getBoundingBox();
	
	//未曾点击到按钮，事件不再下发
	if (!rect.containsPoint(pos))
	{
		_shouldCallback = false;
		return true;
	}

	if (_callback != nullptr)
		_callback(this, EventType::ON_SLIDEBALL_DOWN);
	return true;
}

void Slider::touchMovedInHook(Touch*touch,SDL_Event*event)
{
	auto pos = touch->getLocation();
	auto delta = pos - _startPos;
	_startPos = pos;

	//更新
	auto percent = this->getPercent() + delta.x * _maxPercent / _barLength;
	this->setPercent(percent);
	if (_callback != nullptr)
		_callback(this, EventType::ON_PERCENTAGE_CHANGED);
}

void Slider::touchMovedOutHook(Touch*touch,SDL_Event*event)
{
	if (!_shouldCallback)
		return;
	auto pos = touch->getLocation();
	auto delta = pos - _startPos;
	_startPos = pos;

	if (_callback != nullptr)
		_callback(this, EventType::ON_PERCENTAGE_CHANGED);

	//更新
	auto percent = this->getPercent() + delta.x * _maxPercent / _barLength;
	this->setPercent(percent);
}

void Slider::touchEndedHook(Touch*touch,SDL_Event*event)
{
	if (_shouldCallback && _callback != nullptr)
		_callback(this, EventType::ON_SLIDEBALL_UP);
}

void Slider::touchCancelledHook(Touch*touch,SDL_Event*event)
{
	if (_shouldCallback && _callback != nullptr)
		_callback(this, EventType::ON_SLIDEBALL_CANCEL);
}

void Slider::updatePercent()
{
	//获取进度
	float percent = _barLength * _percent / _maxPercent;
	auto size = this->getContentSize();

	if (_progressBar != nullptr)
	{
		_progressBar->setPreferredSize(Size(percent, size.height));
	}
	if (_slidBallNormalSprite != nullptr)
	{
		_slidBallNormalSprite->setPositionX(percent);
	}
	//设置其他的位置
}

}
NS_SDL_END
