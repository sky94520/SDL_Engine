#include "UIRadioButton.h"
#include "UIRadioButtonGroup.h"
#include "../SESprite.h"
NS_SDL_BEGIN
namespace ui
{
RadioButton::RadioButton()
	:_radioButtonEventCallback(nullptr)
	,_group(nullptr)
{
}
RadioButton::~RadioButton()
{
}
RadioButton*RadioButton::create(const std::string&background
			,const std::string&cross
			,TextureResType resType)
{
	auto widget = new RadioButton();
	if(widget && widget->init(background,cross,resType))
		widget->autorelease();
	else
		SDL_SAFE_DELETE(widget);
	return widget;
}
RadioButton* RadioButton::create(const std::string&background
		,const std::string&backgroundSelected
		,const std::string&cross
		,const std::string&backgroundDisabled
		,const std::string&crossDisabled
		,TextureResType resType)
{
	auto widget = new RadioButton();
	if(widget && widget->init(background,backgroundSelected,cross,backgroundDisabled,crossDisabled,resType))
		widget->autorelease();
	else
		SDL_SAFE_DELETE(widget);
	return widget;
}
void RadioButton::addEventListener(const ccRadioButtonCallback&callback)
{
	_radioButtonEventCallback = callback;
}
void RadioButton::touchBeganHook(Touch*touch,SDL_Event*event)
{
	this->selected();
}
void RadioButton::touchMovedInHook(Touch*touch,SDL_Event*event)
{
	this->selected();
}
void RadioButton::touchMovedOutHook(Touch*touch,SDL_Event*event)
{
	this->unselected();
}
void RadioButton::touchEndedHook(Touch*touch,SDL_Event*event)
{
	//只有在未选中的情况下才会产生有效事件
	bool selected = this->isSelected();
	//结束放大
	this->updateStatus(false);
	if(selected)
		return;
	//改变状态
	this->setSelected(!selected);
	//尝试发送事件
	if(_radioButtonEventCallback)
		_radioButtonEventCallback(this,EventType::SELECTED);
	//尝试发送事件给父类RadioButtonGroup
	if(_group)
		_group->setSelectedButton(this);
}
void RadioButton::touchCancelledHook(Touch*touch,SDL_Event*event)
{
	this->unselected();
}
void RadioButton::updateStatus(bool selected)
{
	//当前处于选中状态，则要播放未选中状态
	if(selected)
		this->selected();
	else
		this->unselected();
}
void RadioButton::selected()
{
	//存在，则显示
	if(_backgroundSelectedSprite)
	{
		_backgroundSelectedSprite->setVisible(true);
		_backgroundSelectedSprite->setScale(1.1f,1.1f);
		_backgroundSprite->setVisible(false);
	}
	else//简单地放大
	{
		_backgroundSprite->setScale(1.1f,1.1f);
	}
}
void RadioButton::unselected()
{
	if(_backgroundSelectedSprite)
	{
		_backgroundSelectedSprite->setVisible(false);
		_backgroundSelectedSprite->setScale(1.f,1.f);
		_backgroundSprite->setVisible(true);
	}
	else//简单地放大
	{
		_backgroundSprite->setScale(1.f,1.f);
	}
}
void RadioButton::dispatchSelectChangeEvent(bool selected)
{
	if(_selected == selected)
		return;
	//设置状态
	this->setSelected(selected);
	//获取当前事件类型并分发事件
	EventType type = selected?EventType::SELECTED:EventType::UNSELECTED;
	if(_radioButtonEventCallback)
		_radioButtonEventCallback(this,type);
}
}//end namespace ui
NS_SDL_END
