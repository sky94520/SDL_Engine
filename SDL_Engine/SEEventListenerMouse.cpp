#include "SEEventListenerMouse.h"
NS_SDL_BEGIN
std::string EventListenerMouse::LISTENER_ID = "EventListenerMouse";

EventListenerMouse::EventListenerMouse()
	:onMouseDown(nullptr),onMouseUp(nullptr)
	,onMouseMotion(nullptr),onMouseScroll(nullptr)
{
}
EventListenerMouse::~EventListenerMouse()
{
}
bool EventListenerMouse::init()
{
	return EventListener::init(LISTENER_ID);
}
EventListenerMouse*EventListenerMouse::clone()
{
	auto listener = EventListenerMouse::create();

	listener->_bRegistered = this->_bRegistered;
	listener->_paused = this->_paused;
	listener->_associatedNode = this->_associatedNode;

	return listener;
}
bool EventListenerMouse::checkAvailable()const
{
	return true;
}
NS_SDL_END
