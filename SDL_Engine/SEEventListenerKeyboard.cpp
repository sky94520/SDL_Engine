#include "SEEventListenerKeyboard.h"
NS_SDL_BEGIN

std::string EventListenerKeyboard::LISTENER_ID = "EventListenerKeyboard";
EventListenerKeyboard::EventListenerKeyboard()
	:onKeyPressed(nullptr)
	,onKeyReleased(nullptr)
{
}
EventListenerKeyboard::~EventListenerKeyboard()
{
}
bool EventListenerKeyboard::init()
{
	EventListener::init("EventListenerKeyboard");
	return true;
}
EventListenerKeyboard*EventListenerKeyboard::clone()
{
	auto listener = EventListenerKeyboard::create();
	listener->_bRegistered = this->_bRegistered;
	listener->_paused = this->_paused;
	listener->_associatedNode = this->_associatedNode;

	return listener;
}
bool EventListenerKeyboard::checkAvailable()const
{
	return isRegistered() && getAssociatedNode() && !isPaused();
}
NS_SDL_END
