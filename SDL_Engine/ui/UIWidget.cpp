#include "UIWidget.h"
#include "../SEObject.h"
#include "../SEEventListenerTouchOneByOne.h"
#include "../SETouch.h"

NS_SDL_BEGIN
namespace ui
{
Widget::Widget()
	:_resType(TextureResType::LOCAL)
	,_clickCallback(nullptr)
	,_touchCallback(nullptr)
	,_bTouchEnabled(false)
	,_swallowTouches(true)
	,_shouldCallback(false)
	,_touchListener(nullptr)
{
	_cascadeScale = true;
}

Widget::~Widget()
{
}

void Widget::addClickEventListener(const ccWidgetClickCallback&callback)
{
	_clickCallback = callback;
}

void Widget::addTouchEventListener(const ccWidgetTouchCallback&callback)
{
	_touchCallback = callback;
}

void Widget::setTouchEnabled(bool isEnable)
{
	if(isEnable == _bTouchEnabled)
		return;

	_bTouchEnabled = isEnable;
	if(isEnable)
	{
		_touchListener = EventListenerTouchOneByOne::create();

		_touchListener->onTouchBegan = SDL_CALLBACK_2(Widget::onTouchBegan,this);
		_touchListener->onTouchMoved = SDL_CALLBACK_2(Widget::onTouchMoved,this);
		_touchListener->onTouchEnded = SDL_CALLBACK_2(Widget::onTouchEnded,this);
		_touchListener->onTouchCancelled = SDL_CALLBACK_2(Widget::onTouchCancelled,this);

		_touchListener->setSwallowTouches(true);
		//设置为最高优先级
		_touchListener->setPriority(0);
		//添加事件
		_eventDispatcher->addEventListener(_touchListener,this);
	}
	else
	{
		_eventDispatcher->removeEventListener(_touchListener);
		_touchListener = nullptr;
	}
}

void Widget::setSwallowTouches(bool swallow)
{
	_swallowTouches = swallow;
}

bool Widget::isSwallowTouches()const
{
	return _swallowTouches;
}

Node* Widget::getVirtualRenderer()
{
	return nullptr;
}

bool Widget::onTouchBegan(Touch*touch,SDL_Event*event)
{
	//如果碰撞成功
	if(this->hitTest(touch))
	{
		_shouldCallback = true;
		this->touchBeganHook(touch,event);

		if(_touchCallback)
		{
			_touchCallback(this,TouchEventType::BEGAN);
		}
		
		return 	_swallowTouches;
	}

	return false;
}
void Widget::onTouchMoved(Touch*touch,SDL_Event*event)
{
	bool bRet = this->hitTest(touch);
	//如果碰撞成功
	if(_shouldCallback && bRet)
	{
		this->touchMovedInHook(touch,event);

		if(_touchCallback)
			_touchCallback(this,TouchEventType::MOVED);
	}
	else
		this->touchMovedOutHook(touch,event);
}
void Widget::onTouchEnded(Touch*touch,SDL_Event*event)
{
	this->retain();

	bool bRet = this->hitTest(touch);
	//应该回调并且在范围内，进行函数回调
	if(_shouldCallback && bRet)
	{
		this->touchEndedHook(touch,event);

		if(_clickCallback)
			_clickCallback(this);
		if(_touchCallback)
			_touchCallback(this,TouchEventType::ENDED);
	}
	//否则canceled
	else if(_shouldCallback && bRet == false)
	{
		this->touchCancelledHook(touch,event);
	}
	_shouldCallback = false;

	this->release();
}
void Widget::onTouchCancelled(Touch*touch,SDL_Event*event)
{
	//如果碰撞成功
	if(_shouldCallback && this->hitTest(touch))
	{
		if(_touchCallback)
			_touchCallback(this,TouchEventType::CANCELLED);
	}
	_shouldCallback = false;
	this->touchCancelledHook(touch,event);
}
bool Widget::hitTest(Touch*touch)
{
	auto pos = touch->getLocation();
	//转化为相对于父类的坐标
	pos = this->getParent()->convertToNodeSpace(pos);
	//获取包围盒
	auto rect = this->getBoundingBox();
	return rect.containsPoint(pos);
}
//end namespace ui
}
NS_SDL_END
