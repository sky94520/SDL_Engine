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
	:_inDispatch(false)
	, _reorderListenersDirty(false)
{
}
EventDispatcher::~EventDispatcher()
{
	for (auto iter = _listenerMap.begin(); iter != _listenerMap.end();)
	{
		auto listeners = iter->second;

		iter = _listenerMap.erase(iter);
		delete listeners;
	}

	for (auto iter = _nodeListenersMap.begin(); iter != _nodeListenersMap.end();)
	{
		auto listeners = iter->second;

		iter = _nodeListenersMap.erase(iter);
		delete listeners;
	}
}

void EventDispatcher::addEventListener(EventListener* listener, Node* node)
{
	//保证该事件监听器没被注册
	SDLASSERT(listener && !listener->isRegistered(), "listener should not nullptr");
	//注册
	listener->setAssociatedNode(node);
	listener->setRegistered(true);
	//当前正在分发事件
	if (_inDispatch)
	{
		_toAddedListeners.push_back(listener);
		listener->retain();
	}
	else
	{
		this->forceAddEventListener(listener, node);
	}
	//listener->retain();
}

void EventDispatcher::forceAddEventListener(EventListener* listener, Node* node)
{
	std::string listenerID = listener->getListenerID();
	std::vector<EventListener*>* listeners = nullptr;
	auto iter = _listenerMap.find(listenerID);

	//不存在对应的vector，则创建一个
	if (iter == _listenerMap.end())
	{
		listeners = new std::vector<EventListener*>();
		_listenerMap.insert(std::make_pair(listenerID, listeners));
	}
	else
	{
		listeners = iter->second;
	}
	listeners->push_back(listener);
	this->associateNodeAndEventListener(node, listener);

	listener->retain();
	_reorderListenersDirty = true;

	if (node->isRunning())
		this->resumeEventListenersForTarget(node);
}

void EventDispatcher::addEventCustomListener(const std::string&eventName, const std::function<void(EventCustom*)>&onEventCustom, Node*node)
{
	auto listener = EventListenerCustom::create(eventName, onEventCustom);
	this->addEventListener(listener, node);
}

void EventDispatcher::removeEventListener(EventListener* listener)
{
	if (listener == nullptr)
		return;

	auto removeListenerInVector = [&](std::vector<EventListener*>* listeners)->bool
	{
		bool isFound = false;

		if (listeners == nullptr)
			return false;
		auto iter = std::find(listeners->begin(), listeners->end(), listener);
		//找到，则进行逻辑删除
		if (iter != listeners->end())
		{
			listener->setRegistered(false);

			//维护_nodeListenersMap
			if (listener->getAssociatedNode() != nullptr)
			{
				this->dissociateNodeAndEventListener(listener->getAssociatedNode()
					, listener);
				listener->setAssociatedNode(nullptr);
			}

			if (_inDispatch)
			{
				_toRemovedListeners.push_back(listener);
			}
			else
			{
				listeners->erase(iter);

				listener->setAssociatedNode(nullptr);
				//取消对listener的引用
				listener->release();
			}
			isFound = true;
		}
		return isFound;
	};
	//已经在待删除列表中，直接退出
	if (std::find(_toRemovedListeners.begin(), _toRemovedListeners.end(), listener) != _toRemovedListeners.end())
		return;

	bool isFound = false;
	//遍历
	for (auto iter = _listenerMap.begin(); iter != _listenerMap.end();)
	{
		//找到对应的容器
		auto listeners = iter->second;
		if (iter->first == listener->getListenerID())
		{
			isFound = removeListenerInVector(listeners);
		}
		if (listeners->empty())
		{
			iter = _listenerMap.erase(iter);
			SDL_SAFE_DELETE(listeners);
		}
		else
		{
			++iter;
		}
		if (isFound)
			break;
	}
	//查看是否在待添加列表中
	if (!isFound)
	{
		for (auto iter = _toAddedListeners.begin(); iter != _toAddedListeners.end(); ++iter)
		{
			if (*iter == listener)
			{
				listener->setRegistered(false);
				SDL_SAFE_RELEASE(listener);
				_toAddedListeners.erase(iter);
				break;
			}
		}
	}
	return;
}

void EventDispatcher::removeEventListenerForTarget(Node*node, bool recursive/*=false*/)
{
	SDLASSERT(node != NULL, "node should not null");

	auto listenerIter = _nodeListenersMap.find(node);

	if (listenerIter != _nodeListenersMap.end())
	{
		auto listeners = listenerIter->second;
		auto listenersCopy = *listeners;

		for (auto& listener : listenersCopy)
		{
			this->removeEventListener(listener);
		}
	}
	/*
	for(auto listenerIter=_listenerMap.begin();listenerIter!=_listenerMap.end();)
	{
		std::vector<EventListener*>* listenerVec = listenerIter->second;
		for(auto iter=listenerVec->begin();iter!=listenerVec->end();)
		{
			auto listener = *iter;

			//找到对应的节点
			if(listener->getAssociatedNode() == node)
			{
				listener->setRegistered(false);
				listener->setAssociatedNode(nullptr);

				if (_inDispatch)
				{
					_toRemovedListeners.push_back(listener);
					++iter;
				}
				else
				{
					iter = listenerVec->erase(iter);

					listener->release();
				}
			}
			else
			{
				++iter;
			}
		}
		listenerIter++;
	}
	*/
	for (auto iter = _toAddedListeners.begin(); iter != _toAddedListeners.end();)
	{
		EventListener* listener = *iter;

		if (listener->getAssociatedNode() == node)
		{
			listener->setAssociatedNode(nullptr);
			listener->setRegistered(false);
			SDL_SAFE_RELEASE(listener);
			iter = _toAddedListeners.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	if (recursive)
	{
		const auto& children = node->getChildren();

		for (const auto& child : children)
		{
			removeEventListenerForTarget(child);
		}
	}
}

void EventDispatcher::resumeEventListenersForTarget(Node*node, bool recursive/*=false*/)
{
	SDLASSERT(node, "node should not null");

	auto listenerIter = _nodeListenersMap.find(node);

	if (listenerIter != _nodeListenersMap.end())
	{
		auto listeners = listenerIter->second;

		for (auto& listener : *listeners)
		{
			listener->setPaused(false);
		}
	}

	for (auto& listener : _toAddedListeners)
	{
		if (listener->getAssociatedNode() == node)
		{
			listener->setPaused(false);
		}
	}
	if (recursive)
	{
		const auto& children = node->getChildren();
		for (const auto& child : children)
		{
			resumeEventListenersForTarget(child, true);
		}
	}
}

void EventDispatcher::pauseEventListenersForTarget(Node*node, bool recursive/*=false*/)
{
	SDLASSERT(node, "node should not null");

	auto listenerIter = _nodeListenersMap.find(node);

	if (listenerIter != _nodeListenersMap.end())
	{
		auto listeners = listenerIter->second;

		for (auto& listener : *listeners)
		{
			listener->setPaused(true);
		}
	}

	for (auto& listener : _toAddedListeners)
	{
		if (listener->getAssociatedNode() == node)
		{
			listener->setPaused(true);
		}
	}
	if (recursive)
	{
		const auto& children = node->getChildren();
		for (const auto& child : children)
		{
			pauseEventListenersForTarget(child, true);
		}
	}
}

void EventDispatcher::dispatchKeyboardEvent(SDL_Event& event)
{
	auto onEvent = [](EventListenerKeyboard*listener, SDL_Event& event)
	{
		if (listener->checkAvailable())
		{
			SDL_Keycode keyCode = event.key.keysym.sym;
			if (event.type == SDL_KEYDOWN && listener->onKeyPressed)
				listener->onKeyPressed(keyCode, &event);
			else if (event.type == SDL_KEYUP && listener->onKeyReleased)
				listener->onKeyReleased(keyCode, &event);
		}
	};
	//获取键盘事件监听器
	auto listeners = getListeners(EventListenerKeyboard::LISTENER_ID);

	auto size = listeners != nullptr ? listeners->size() : 0;
	for (unsigned int i = 0; listeners && i < size; i++)
	{
		auto listener = dynamic_cast<EventListenerKeyboard*>(listeners->at(i));
		onEvent(listener, event);
	}
}

void EventDispatcher::dispatchKeyboardStateEvent(SDL_Event& event)
{
	auto onEvent = [](EventListenerKeyboardState*listener, SDL_Event& event)
	{
		if (listener->checkAvailable() == false)
			return;
		const Uint8*keyStates = SDL_GetKeyboardState(NULL);
		if (listener->onEvent)
			listener->onEvent(keyStates, &event);
	};
	auto listeners = this->getListeners(EventListenerKeyboardState::LISTENER_ID);
	//事件分发
	auto size = listeners != nullptr ? listeners->size() : 0;

	for (unsigned int i = 0; listeners && i < size; i++)
	{
		auto listener = dynamic_cast<EventListenerKeyboardState*>(listeners->at(i));
		onEvent(listener, event);
	}
}

void EventDispatcher::dispatchMouseEvent(SDL_Event& event)
{
	auto onEvent = [](EventListenerMouse*listener, SDL_Event& event)->bool
	{
		if (listener->checkAvailable() == false)
			return false;
		//发送事件
		Uint32 type = event.type;
		if (type == SDL_MOUSEBUTTONDOWN && listener->onMouseDown)
		{
			SDL_MouseButtonEvent*e = &event.button;
			listener->onMouseDown(e);
		}
		else if (type == SDL_MOUSEBUTTONUP && listener->onMouseUp)
		{
			SDL_MouseButtonEvent*e = &event.button;
			listener->onMouseUp(e);
		}
		else if (type == SDL_MOUSEMOTION && listener->onMouseMotion)
		{
			SDL_MouseMotionEvent*e = &event.motion;
			listener->onMouseMotion(e);
		}
		else if (type == SDL_MOUSEWHEEL && listener->onMouseScroll)
		{
			SDL_MouseWheelEvent*e = &event.wheel;
			listener->onMouseScroll(e);
		}
		return true;
	};
	//获取鼠标监听事件
	auto listeners = this->getListeners(EventListenerMouse::LISTENER_ID);
	auto size = listeners != nullptr ? listeners->size() : 0;

	for (unsigned int i = 0; listeners && i < size; i++)
	{
		auto listener = dynamic_cast<EventListenerMouse*>(listeners->at(i));
		onEvent(listener, event);
	}
}

void EventDispatcher::dispatchTextInputEvent(SDL_Event& event)
{
	auto listeners = this->getListeners(EventListenerTextInput::LISTENER_ID);

	auto onEvent = [](EventListenerTextInput* listener, SDL_Event& event)
	{
		if (!listener->checkAvailable())
			return;
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

	auto size = listeners != nullptr ? listeners->size() : 0;
	for (unsigned int i = 0; listeners && i < size; i++)
	{
		auto listener = static_cast<EventListenerTextInput*>(listeners->at(i));
		onEvent(listener, event);
	}
}

void EventDispatcher::dispatchTouchEvent(const std::vector<Touch*>& touches, SDL_Event& event)
{
	sortEventListeners();

	if (!touches.empty())
	{
		this->dispatchEventToTouchOneByOne(touches, &event);
		this->dispatchEventToTouchAllAtOnce(touches, &event);
	}
}

void EventDispatcher::dispatchCustomEvent(const std::string&eventName, void*userData)
{
	EventCustom eventCustom(eventName);
	eventCustom.setUserData(userData);

	this->dispatchCustomEvent(&eventCustom);
}

void EventDispatcher::dispatchCustomEvent(EventCustom*eventCustom)
{
	auto onEvent = [](EventListenerCustom*listener, EventCustom*event)
	{
		if (event->getEventName() == listener->getEventName())
			listener->onCustomEvent(event);
	};

	auto listeners = this->getListeners(EventListenerCustom::LISTENER_ID);

	auto size = listeners != nullptr ? listeners->size() : 0;
	for (unsigned int i = 0; listeners && i < size; i++)
	{
		auto listener = dynamic_cast<EventListenerCustom*>(listeners->at(i));

		onEvent(listener, eventCustom);
	}
}

void EventDispatcher::updateListeners(const std::string& listenerID)
{
	auto onUpdateListeners = [this](const std::string& listenerID)
	{
		auto listenerIter = _listenerMap.find(listenerID);
		if (listenerIter == _listenerMap.end())
			return;

		auto listeners = listenerIter->second;

		for (auto iter = listeners->begin(); iter != listeners->end();)
		{
			auto listener = *iter;

			//监听器未注册
			if (!listener->isRegistered())
			{
				iter = listeners->erase(iter);

				auto matchIter = std::find(_toRemovedListeners.begin()
					, _toRemovedListeners.end(), listener);

				if (matchIter != _toRemovedListeners.end())
					_toRemovedListeners.erase(matchIter);
				SDL_SAFE_RELEASE(listener);
			}
			else
			{
				++iter;
			}
		}
	};

	if (listenerID == EventListenerTouchOneByOne::LISTENER_ID
		|| listenerID == EventListenerAllAtOnce::LISTENER_ID)
	{
		onUpdateListeners(EventListenerTouchOneByOne::LISTENER_ID);
		onUpdateListeners(EventListenerAllAtOnce::LISTENER_ID);
	}
	else if (listenerID == EventListenerKeyboard::LISTENER_ID
		|| listenerID == EventListenerKeyboardState::LISTENER_ID)
	{
		onUpdateListeners(EventListenerKeyboard::LISTENER_ID);
		onUpdateListeners(EventListenerKeyboardState::LISTENER_ID);
	}
	else if (listenerID == EventListenerMouse::LISTENER_ID)
	{
		onUpdateListeners(EventListenerMouse::LISTENER_ID);
		onUpdateListeners(EventListenerTouchOneByOne::LISTENER_ID);
	}
	else
	{
		onUpdateListeners(listenerID);
	}
	//清空empty的vector
	for (auto iter = _listenerMap.begin(); iter != _listenerMap.end();)
	{
		if (iter->second->empty())
		{
			delete iter->second;

			iter = _listenerMap.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	if (!_toAddedListeners.empty())
	{
		for (auto& listener : _toAddedListeners)
		{
			this->forceAddEventListener(listener, listener->getAssociatedNode());
			SDL_SAFE_RELEASE(listener);
		}
		_toAddedListeners.clear();
	}
	if (!_toRemovedListeners.empty())
	{
		this->cleanToRemovedListeners();
	}
}

void EventDispatcher::dispatchEventToTouchOneByOne(const std::vector<Touch*> &touches, SDL_Event*event)
{
	auto listeners = getListeners(EventListenerTouchOneByOne::LISTENER_ID);

	if (listeners == nullptr || listeners->empty())
		return;

	auto onEvent = [&](EventListener*l, Touch*touch)->bool
	{
		auto listener = dynamic_cast<EventListenerTouchOneByOne*>(l);
		bool isSwallow = listener->isTouchBegan();

		if (listener->onTouchBegan && touch->getStatus() == TouchStatus::TOUCH_DOWN)
		{
			isSwallow = listener->onTouchBegan(touch, event);

			listener->setTouchBegan(isSwallow);
		}
		else if (listener->onTouchMoved && touch->getStatus() == TouchStatus::TOUCH_MOTION)
			listener->onTouchMoved(touch, event);
		else if (listener->onTouchEnded && touch->getStatus() == TouchStatus::TOUCH_UP)
			listener->onTouchEnded(touch, event);

		return isSwallow;
	};

	auto size = listeners != nullptr ? listeners->size() : 0;

	for (auto touch : touches)
	{
		if (touch->isAvailable() == false)
			continue;

		for (unsigned int i = 0; i < size; i++)
		{
			auto listener = listeners->at(i);

			if (listener->isRegistered() && listener->checkAvailable() && !listener->isPaused())
			{
				//是否阻塞事件 只是阻塞onTouchBegan
				bool bSwallow = onEvent(listener, touch);

				if (bSwallow && listener->isSwallowTouches())
					break;
			}
		}
	}
}

void EventDispatcher::dispatchEventToTouchAllAtOnce(const std::vector<Touch*>& touches, SDL_Event*event)
{
	auto listeners = getListeners(EventListenerAllAtOnce::LISTENER_ID);
	if (listeners == nullptr || listeners->empty())
		return;
	//分发事件
	auto onEvent = [&](EventListener*l, const std::vector<Touch*>&touches)->void
	{
		auto listener = dynamic_cast<EventListenerAllAtOnce*>(l);
		if (listener->onTouchesBegan && event->type == SDL_FINGERDOWN)
			listener->onTouchesBegan(touches, event);
		else if (listener->onTouchesMoved && event->type == SDL_FINGERMOTION)
			listener->onTouchesMoved(touches, event);
		else if (listener->onTouchesEnded && event->type == SDL_FINGERUP)
			listener->onTouchesEnded(touches, event);
	};
	//选出有用的touches
	std::vector<Touch*> t;
	for (auto iter = touches.begin(); iter != touches.end(); iter++)
	{
		auto touch = *iter;
		if (touch->isAvailable())
			t.push_back(touch);
	}

	auto size = listeners != nullptr ? listeners->size() : 0;
	for (unsigned int i = 0; i < size; i++)
	{
		auto listener = listeners->at(i);
		if (listener->isRegistered() && listener->checkAvailable() && !listener->isPaused())
		{
			onEvent(listener, t);
		}
	}
}

std::vector<EventListener*>* EventDispatcher::getListeners(const std::string&listenerID)
{
	auto iter = _listenerMap.find(listenerID);

	return iter == _listenerMap.end() ? nullptr : iter->second;
}

void EventDispatcher::sortEventListeners()
{
	if (_reorderListenersDirty)
	{
		_reorderListenersDirty = false;
		for (auto iter = _listenerMap.begin(); iter != _listenerMap.end();)
		{
			std::vector<EventListener*> &listenerVector = *iter->second;
			std::stable_sort(std::begin(listenerVector), std::end(listenerVector), sorted);
			iter++;
		}
	}
}

bool EventDispatcher::sorted(EventListener*e1, EventListener*e2)
{
	return e1->getPriority() < e2->getPriority();
}

void EventDispatcher::cleanToRemovedListeners()
{
	for (auto& listener : _toRemovedListeners)
	{
		auto listenersIter = _listenerMap.find(listener->getListenerID());

		if (listenersIter == _listenerMap.end())
		{
			SDL_SAFE_RELEASE(listener);
			continue;
		}
		auto listeners = listenersIter->second;
		auto matchIter = std::find(listeners->begin(), listeners->end(), listener);

		//找到 删除
		if (matchIter != listeners->end())
		{
			listeners->erase(matchIter);
			SDL_SAFE_RELEASE(listener);
		}
		else
		{
			SDL_SAFE_RELEASE(listener);
		}
	}
	_toRemovedListeners.clear();
}

void EventDispatcher::associateNodeAndEventListener(Node* node, EventListener* listener)
{
	std::vector<EventListener*>* listeners = nullptr;
	auto found = _nodeListenersMap.find(node);

	if (found != _nodeListenersMap.end())
	{
		listeners = found->second;
	}
	else
	{
		listeners = new std::vector<EventListener*>();
		_nodeListenersMap.emplace(node, listeners);
	}
	listeners->push_back(listener);
}

void EventDispatcher::dissociateNodeAndEventListener(Node* node, EventListener* listener)
{
	std::vector<EventListener*>* listeners = nullptr;
	auto found = _nodeListenersMap.find(node);

	if (found != _nodeListenersMap.end())
	{
		listeners = found->second;
		auto iter = std::find(listeners->begin(), listeners->end(), listener);

		if (iter != listeners->end())
		{
			listeners->erase(iter);
		}

		if (listeners->empty())
		{
			_nodeListenersMap.erase(found);
			delete listeners;
		}
	}
}

NS_SDL_END
