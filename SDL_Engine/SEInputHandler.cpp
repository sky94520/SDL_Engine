#include "SEInputHandler.h"
#include "SEDirector.h"
#include "SEEventListener.h"
#include "SEEventListenerTouchOneByOne.h"
#include "SEEventListenerAllAtOnce.h"
#include "SEEventListenerMouse.h"
#include "SEEventListenerTextInput.h"
#include "SEEventListenerCustom.h"
#include "SEEventListenerKeyboardState.h"
#include "SEEventListenerKeyboard.h"
#include "SETouch.h"
#include "SEEventDispatcher.h"
NS_SDL_BEGIN
InputHandler::InputHandler()
{
}
InputHandler::~InputHandler()
{
	for(auto iter = _touches.begin();iter != _touches.end();)
	{
		auto touch = *iter;
		touch->release();
		iter = _touches.erase(iter);
	}
}
void InputHandler::update()
{
	SDL_Event event = {};
	EventDispatcher* eventDispatcher = Director::getInstance()->getEventDispatcher();
	//标记：当前正在分发事件
	eventDispatcher->setInDispatch(true);
	std::string listenerID = "";

	//对事件进行轮询
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_QUIT:Director::getInstance()->setRunning(false);break;
			//按键事件
			case SDL_KEYUP:
			case SDL_KEYDOWN:
			{
				eventDispatcher->dispatchKeyboardEvent(event);
				eventDispatcher->dispatchKeyboardStateEvent(event);
				listenerID = EventListenerKeyboard::LISTENER_ID;
			}break;
			//触碰事件
			case SDL_FINGERDOWN:
			{
				touchBegan(event);
				eventDispatcher->dispatchTouchEvent(_touches,event); 
				listenerID = EventListenerTouchOneByOne::LISTENER_ID;
			}break;
			case SDL_FINGERMOTION:
			{
				touchMoved(event);
				eventDispatcher->dispatchTouchEvent(_touches,event); 
				listenerID = EventListenerTouchOneByOne::LISTENER_ID;
			}break;
			case SDL_FINGERUP:
			{
				touchEnded(event);
				eventDispatcher->dispatchTouchEvent(_touches,event); 

				listenerID = EventListenerTouchOneByOne::LISTENER_ID;
			}break;
			//鼠标事件
			case SDL_MOUSEBUTTONDOWN:
			{
				mouseBtnDown(event);
				eventDispatcher->dispatchTouchEvent(_touches,event);
				eventDispatcher->dispatchMouseEvent(event); 

				listenerID = EventListenerMouse::LISTENER_ID;
			}break;
			case SDL_MOUSEBUTTONUP:
			{
				mouseBtnUp(event); 
				eventDispatcher->dispatchTouchEvent(_touches,event);
				eventDispatcher->dispatchMouseEvent(event);

				listenerID = EventListenerMouse::LISTENER_ID;
			}break;
			case SDL_MOUSEMOTION:
			{
				mouseMotion(event); 
				eventDispatcher->dispatchTouchEvent(_touches,event);
				eventDispatcher->dispatchMouseEvent(event);

				listenerID = EventListenerMouse::LISTENER_ID;
			}break;
			case SDL_MOUSEWHEEL:
			{
				eventDispatcher->dispatchMouseEvent(event);
				listenerID = EventListenerMouse::LISTENER_ID;
			}
				break;
			//文本输入事件
			case SDL_TEXTINPUT:
			case SDL_TEXTEDITING:
			{
				eventDispatcher->dispatchTextInputEvent(event);
			}
				break;
		}
	}
	if (!listenerID.empty())
	{
		eventDispatcher->updateListeners(listenerID);
	}

	eventDispatcher->setInDispatch(false);

	for(unsigned int i = 0;i<_touches.size();i++)
	{
		auto touch = _touches.at(i);
		//设置当前touch没被使用
		if(touch->getStatus() == TouchStatus::TOUCH_UP) 
				touch->setTouchInfo(-1,0.f,0.f,TouchStatus::TOUCH_NONE);
	}
}
//--private

void InputHandler::mouseBtnDown(SDL_Event event)
{
	//在win下 把鼠标左键转换为触碰点
	if(event.button.button == SDL_BUTTON_LEFT)
	{
		//获取当前鼠标位置
		float x = (float)event.motion.x;
		float y = (float)event.motion.y;

		auto touch = getAvaiableTouch();

		touch->setTouchInfo(1,x,y,TouchStatus::TOUCH_DOWN);
	}
}
void InputHandler::mouseBtnUp(SDL_Event event)
{
	if(event.button.button == SDL_BUTTON_LEFT)
	{
		//获取当前鼠标位置
		float x = (float)event.motion.x;
		float y = (float)event.motion.y;

		auto touch = getTouchByID(1);
		if(touch)
			touch->setTouchInfo(1,x,y,TouchStatus::TOUCH_UP);
	}
}
void InputHandler::mouseMotion(SDL_Event event)
{
	if(event.button.button == SDL_BUTTON_LEFT)
	{
		//获取当前鼠标位置
		float x = (float)event.motion.x;
		float y = (float)event.motion.y;

		auto touch = getTouchByID(1);

		touch->setTouchInfo(1,x,y,TouchStatus::TOUCH_MOTION);
	}
}
void InputHandler::touchBegan(SDL_Event event)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	SDL_FingerID id = event.tfinger.fingerId;
	float x = event.tfinger.x * visibleSize.width;
	float y = event.tfinger.y * visibleSize.height;

	Touch*touch = this->getAvaiableTouch();
	touch->setTouchInfo(id,x,y,TouchStatus::TOUCH_DOWN);
}
void InputHandler::touchMoved(SDL_Event event)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	SDL_FingerID id = event.tfinger.fingerId;
	float x = event.tfinger.x * visibleSize.width;
	float y = event.tfinger.y * visibleSize.height;

	auto touch = this->getTouchByID(id);
	touch->setTouchInfo(id,x,y,TouchStatus::TOUCH_MOTION);
}
void InputHandler::touchEnded(SDL_Event event)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	SDL_FingerID id = event.tfinger.fingerId;
	float x = event.tfinger.x * visibleSize.width;
	float y = event.tfinger.y * visibleSize.height;

	auto touch = this->getTouchByID(id);
	touch->setTouchInfo(id,x,y,TouchStatus::TOUCH_UP);
}
Touch*InputHandler::getAvaiableTouch()
{
	std::string platform = SDL_GetPlatform();

	Touch*touch = nullptr;
	for(auto iter = _touches.begin();iter != _touches.end();++iter)
	{
		auto t = *iter;
		if(t->isAvailable() == false)
		{
			touch = t;
			break;
		}
	}
	//如果是在win平台，保证touch只有一个
	if( !_touches.empty() 
	&& (platform == "Windows" || platform == "Linux"))
		touch = _touches.at(0);
	//没有可用的Touch，新建
	if(touch == nullptr)
	{
		touch = Touch::create();
		touch->retain();
		_touches.push_back(touch);
	}
	return touch;
}
Touch*InputHandler::getTouchByID(SDL_FingerID id)
{
	auto iter = std::find_if(_touches.begin(),_touches.end(),[id](Touch*touch){
		return touch->isAvailable() && touch->getID() == id;
	});
	if(iter != _touches.end())
		return *iter;
	return nullptr;
}
NS_SDL_END
