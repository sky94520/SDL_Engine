#include "SEEventDispatcher.h"
#include "SENode.h"
#include "SETouch.h"
#include "SEDirector.h"
#include "SEEventListenerKeyboard.h"
#include "SEEventListenerTouchOneByOne.h"
#include "SEEventListenerAllAtOnce.h"
#include "SEEventListenerCustom.h"
#include "SEEventListenerMouse.h"
#include "SEEventListenerKeyboardState.h"
#include "SEEventListenerTextInput.h"

NS_SDL_BEGIN
EventDispatcher::EventDispatcher()
	:_reorderListenersDirty(false)
{
}
EventDispatcher::~EventDispatcher()
{
	for(auto iter = _listeners.begin();iter != _listeners.end();)
	{
		auto listeners = iter->second;

		iter = _listeners.erase(iter);
		delete listeners;
	}
}

void EventDispatcher::addEventListener(EventListener*listener,Node*node)
{
	//保证该事件监听器没被注册
	SDLASSERT(listener && !listener->isRegistered(),"listener should not nullptr");

	std::string listenerID = listener->getListenerID();
	std::vector<EventListener*>* listeners=nullptr;
	auto iter = _listeners.find(listenerID);

	if(iter == _listeners.end())
	{
		listeners = new std::vector<EventListener*>();
		_listeners.insert(std::make_pair(listenerID,listeners));
	}
	else
		listeners = iter->second;

	listeners->push_back(listener);

	listener->setAssociatedNode(node);
	listener->setRegistered(true);
	listener->retain();
	_reorderListenersDirty = true;

	if(node->isRunning())
		this->resumeEventListenersForTarget(node);
}

void EventDispatcher::addEventCustomListener(const std::string&eventName,const std::function<void(EventCustom*)>&onEventCustom,Node*node)
{
	auto listener = EventListenerCustom::create(eventName,onEventCustom);
	this->addEventListener(listener,node);
}

bool EventDispatcher::removeEventListener(EventListener*listener)
{
	if(listener==nullptr)
		return false;
	auto listenerVector = getListeners(listener->getListenerID());
	
	auto iter = std::find_if(listenerVector->begin(),listenerVector->end(),[listener](EventListener*l)
	{
		return listener == l;
	});
	if(iter != listenerVector->end())
	{
		listenerVector->erase(iter);
		listener->setRegistered(false);

		listener->setAssociatedNode(nullptr);
		//取消对listener的引用
		listener->release();
		return true;
	}
	return false;
}

void EventDispatcher::removeEventListenerForTarget(Node*node,bool recursive/*=false*/)
{
	SDLASSERT(node!=NULL,"node should not null");
	for(auto listenerIter=_listeners.begin();listenerIter!=_listeners.end();)
	{
		std::vector<EventListener*>* listenerVec = listenerIter->second;
		for(auto iter=listenerVec->begin();iter!=listenerVec->end();)
		{
			auto listener = *iter;

			if(listener->getAssociatedNode() == node)
			{
				iter = listenerVec->erase(iter);

				listener->setRegistered(false);
				listener->setAssociatedNode(nullptr);
				listener->release();
			}
			else
				++iter;
		}
		listenerIter++;
	}
}

void EventDispatcher::resumeEventListenersForTarget(Node*node,bool recursive/*=false*/)
{
	SDLASSERT(node,"node should not null");
	for(auto listenerIter=_listeners.begin();listenerIter!=_listeners.end();)
	{
		std::vector<EventListener*>* listeners = listenerIter->second;
		for(auto iter=listeners->begin();iter!=listeners->end();)
		{
			auto listener = *iter;
			if(listener->getAssociatedNode() == node)
			{
				listener->setPaused(false);
			}
			iter++;
		}
		listenerIter++;
	}
	/*if(recursive)
	{
		for(const auto child:node->getC)
	}*/
}

void EventDispatcher::pauseEventListenersForTarget(Node*node,bool recursive/*=false*/)
{
	SDLASSERT(node,"node should not null");
	for(auto listenerIter=_listeners.begin();listenerIter!=_listeners.end();)
	{
		std::vector<EventListener*>* listeners = listenerIter->second;
		for(auto iter=listeners->begin();iter!=listeners->end();)
		{
			auto listener = *iter;
			if(listener->getAssociatedNode() == node)
			{
				listener->setPaused(true);
			}
			iter++;
		}
		listenerIter++;
	}
	/*for(const auto&listener:_touchOneByOneVector)
	{
		if(listener->getAssociatedNode()==node)
			listener->setPaused(true);
	}*/
	/*if(recursive)
	{
		for(const auto child:node->getC)
	}*/
}

void EventDispatcher::dispatchKeyboardEvent(SDL_Event& event)
{
	auto onEvent = [](EventListenerKeyboard*listener,SDL_Event& event)
	{
		if(listener->checkAvailable())
		{
			SDL_Keycode keyCode = event.key.keysym.sym;
			if(event.type == SDL_KEYDOWN && listener->onKeyPressed)
				listener->onKeyPressed(keyCode,&event);
			else if(event.type == SDL_KEYUP && listener->onKeyReleased)
				listener->onKeyReleased(keyCode,&event);
		}
	};
	//获取键盘事件监听器
	auto listeners = getListeners(EventListenerKeyboard::LISTENER_ID);
	//新添加的不进行事件接收
	auto size = listeners != nullptr ? listeners->size() : 0;
	/*在这里不能使用迭代器，因为在执行函数中可能会有replaceScen或popScene，会改变监听器的大小
	从而使得迭代器失效*/
	for(unsigned int i=0;listeners && i < size;i++)
	{
		auto listener = dynamic_cast<EventListenerKeyboard*>(listeners->at(i));
		onEvent(listener,event);
	}
}

void EventDispatcher::dispatchKeyboardStateEvent(SDL_Event& event)
{
	auto onEvent = [](EventListenerKeyboardState*listener,SDL_Event& event)
	{
		if(listener->checkAvailable() == false)
			return;
		const Uint8*keyStates = SDL_GetKeyboardState(NULL);
		if(listener->onEvent)
			listener->onEvent(keyStates,&event);
	};
	auto listeners = this->getListeners(EventListenerKeyboardState::LISTENER_ID);
	//新添加的不进行事件接收
	auto size = listeners != nullptr ? listeners->size() : 0;

	for(unsigned int i = 0;listeners && i < size;i++)
	{
		auto listener = dynamic_cast<EventListenerKeyboardState*>(listeners->at(i));
		onEvent(listener,event);
	}
}

void EventDispatcher::dispatchMouseEvent(SDL_Event& event)
{
	auto onEvent = [](EventListenerMouse*listener,SDL_Event& event)->bool
	{
		if(listener->checkAvailable() == false)
			return false;
		//发送事件
		Uint32 type = event.type;
		if(type == SDL_MOUSEBUTTONDOWN && listener->onMouseDown)
		{
			SDL_MouseButtonEvent*e = &event.button;
			listener->onMouseDown(e);
		}
		else if(type == SDL_MOUSEBUTTONUP && listener->onMouseUp)
		{
			SDL_MouseButtonEvent*e = &event.button;
			listener->onMouseUp(e);
		}
		else if(type == SDL_MOUSEMOTION && listener->onMouseMotion)
		{
			SDL_MouseMotionEvent*e = &event.motion;
			listener->onMouseMotion(e);
		}
		else if(type == SDL_MOUSEWHEEL && listener->onMouseScroll)
		{
			SDL_MouseWheelEvent*e = &event.wheel;
			listener->onMouseScroll(e);
		}
		return true;
	};
	//获取鼠标监听事件
	auto listeners = this->getListeners(EventListenerMouse::LISTENER_ID);
	//新添加的不进行事件接收
	auto size = listeners != nullptr ? listeners->size() : 0;

	for(unsigned int i=0;listeners && i < size;i++)
	{
		auto listener = dynamic_cast<EventListenerMouse*>(listeners->at(i));
		onEvent(listener,event);
	}
}

void EventDispatcher::dispatchTextInputEvent(SDL_Event& event)
{
	auto listeners = this->getListeners(EventListenerTextInput::LISTENER_ID);

	auto onEvent = [](EventListenerTextInput* listener, SDL_Event& event)
	{
		if (!listener->checkAvailable())
			return ;
		if (event.type == SDL_TEXTINPUT && listener->onTextInput != nullptr)
		{
			listener->onTextInput(event.text.text, &event);
		}
		else if (event.type == SDL_TEXTEDITING && listener->onTextEditing != nullptr)
		{
			char* composition = event.edit.text;
			Sint32 cursor = event.edit.start;
			Sint32 selection_len = event.edit.length;

			listener->onTextEditing(composition, cursor, selection_len, &event);
		}
	};
	//新添加的不进行事件接收
	auto size = listeners != nullptr ? listeners->size() : 0;
	for (unsigned int i = 0; listeners && i < size; i++)
	{
		auto listener = static_cast<EventListenerTextInput*>(listeners->at(i));
		onEvent(listener, event);
	}
}

void EventDispatcher::dispatchTouchEvent(const std::vector<Touch*>& touches,SDL_Event& event)
{
	sortEventListeners();

	if ( !touches.empty())
	{
		this->dispatchEventToTouchOneByOne(touches,&event);
		this->dispatchEventToTouchAllAtOnce(touches,&event);
	}
}

void EventDispatcher::dispatchCustomEvent(const std::string&eventName,void*userData)
{
	EventCustom eventCustom(eventName);
	eventCustom.setUserData(userData);

	this->dispatchCustomEvent(&eventCustom);
/*	auto onEvent = [eventName](EventListenerCustom*listener,EventCustom*event)
	{
		if(eventName == listener->getEventName())
			listener->onCustomEvent(event);
	};

	auto listeners = this->getListeners(EventListenerCustom::LISTENER_ID);

	for(unsigned int i=0;listeners && i < listeners->size();i++)
	{
		auto listener = dynamic_cast<EventListenerCustom*>(listeners->at(i));
		auto eventCustom = EventCustom(eventName);
		eventCustom.setUserData(userData);

		onEvent(listener,&eventCustom);
	}*/
}

void EventDispatcher::dispatchCustomEvent(EventCustom*eventCustom)
{
	auto onEvent = [](EventListenerCustom*listener,EventCustom*event)
	{
		if(event->getEventName() == listener->getEventName())
			listener->onCustomEvent(event);
	};

	auto listeners = this->getListeners(EventListenerCustom::LISTENER_ID);

	//新添加的不进行事件接收
	auto size = listeners != nullptr ? listeners->size() : 0;
	for(unsigned int i=0;listeners && i < size;i++)
	{
		auto listener = dynamic_cast<EventListenerCustom*>(listeners->at(i));

		onEvent(listener,eventCustom);
	}
}

void EventDispatcher::dispatchEventToTouchOneByOne(const std::vector<Touch*> &touches,SDL_Event*event)
{
	auto listeners = getListeners(EventListenerTouchOneByOne::LISTENER_ID);

	if(listeners == nullptr || listeners->empty())
		return;

	auto onEvent = [&](EventListener*l,Touch*touch)->bool
	{
		auto listener = dynamic_cast<EventListenerTouchOneByOne*>(l);
		bool isSwallow = listener->isTouchBegan();

		if(listener->onTouchBegan && touch->getStatus() == TouchStatus::TOUCH_DOWN)
		{
			isSwallow = listener->onTouchBegan(touch,event);

			listener->setTouchBegan(isSwallow);
		}
		else if(listener->onTouchMoved && touch->getStatus() == TouchStatus::TOUCH_MOTION)
			listener->onTouchMoved(touch,event);
		else if(listener->onTouchEnded && touch->getStatus() == TouchStatus::TOUCH_UP)
			listener->onTouchEnded(touch,event);

		return isSwallow;
	};

	//新添加的不进行事件接收
	auto size = listeners != nullptr ? listeners->size() : 0;

	for(auto touch:touches)
	{
		if(touch->isAvailable() == false)
			continue;

		for(unsigned int i = 0;i < size;i++)
		{
			auto listener = listeners->at(i);

			if(listener->isRegistered() && listener->checkAvailable() && !listener->isPaused())
			{
				//是否阻塞事件 只是阻塞onTouchBegan
				bool bSwallow = onEvent(listener,touch);

				if(bSwallow && listener->isSwallowTouches())
					break;
			}
		}
	}
}

void EventDispatcher::dispatchEventToTouchAllAtOnce(const std::vector<Touch*>& touches,SDL_Event*event)
{
	auto listeners = getListeners(EventListenerAllAtOnce::LISTENER_ID);
	if(listeners == nullptr || listeners->empty())
		return;
	//分发事件
	auto onEvent = [&](EventListener*l,const std::vector<Touch*>&touches)->void
	{
		auto listener = dynamic_cast<EventListenerAllAtOnce*>(l);
		if(listener->onTouchesBegan && event->type == SDL_FINGERDOWN)
			listener->onTouchesBegan(touches,event);
		else if(listener->onTouchesMoved && event->type == SDL_FINGERMOTION)
			listener->onTouchesMoved(touches,event);
		else if(listener->onTouchesEnded && event->type == SDL_FINGERUP)
			listener->onTouchesEnded(touches,event);
	};
	//选出有用的touches
	std::vector<Touch*> t;
	for(auto iter = touches.begin();iter != touches.end();iter++)
	{
		auto touch = *iter;
		if(touch->isAvailable())
			t.push_back(touch);
	}
	//新添加的不进行事件接收
	auto size = listeners != nullptr ? listeners->size() : 0;
	for(unsigned int i = 0;i < size;i++)
	{
		auto listener = listeners->at(i);
		if(listener->isRegistered() && listener->checkAvailable() && !listener->isPaused())
		{
			onEvent(listener,t);
		}
	}
}

std::vector<EventListener*>* EventDispatcher::getListeners(const std::string&listenerID)
{
	auto iter = _listeners.find(listenerID);

	return iter == _listeners.end() ? nullptr:iter->second;
}

void EventDispatcher::sortEventListeners()
{
	if(_reorderListenersDirty)
	{
		_reorderListenersDirty = false;
		for(auto iter=_listeners.begin();iter!=_listeners.end();)
		{
			std::vector<EventListener*> &listenerVector = *iter->second;
			std::stable_sort(std::begin(listenerVector),std::end(listenerVector),sorted);
			iter++;
		}
	}
}

bool EventDispatcher::sorted(EventListener*e1,EventListener*e2)
{
	return e1->getPriority() < e2->getPriority();
}

NS_SDL_END
