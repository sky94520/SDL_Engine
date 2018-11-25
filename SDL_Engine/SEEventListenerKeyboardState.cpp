#include "SEEventListenerKeyboardState.h"
NS_SDL_BEGIN
std::string EventListenerKeyboardState::LISTENER_ID = "EventListenerKeyboardState";

EventListenerKeyboardState::EventListenerKeyboardState()
	:onEvent(nullptr)
{
}
EventListenerKeyboardState::~EventListenerKeyboardState()
{
}
bool EventListenerKeyboardState::init()
{
	return EventListener::init(LISTENER_ID);
}
EventListenerKeyboardState*EventListenerKeyboardState::clone()
{
	return EventListenerKeyboardState::create();
}
bool EventListenerKeyboardState::checkAvailable()const
{
	return onEvent != nullptr;
}
NS_SDL_END
