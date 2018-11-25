#include "SEEventListenerTextInput.h"

NS_SDL_BEGIN
std::string EventListenerTextInput::LISTENER_ID = "EventListenerTextInput";

EventListenerTextInput::EventListenerTextInput()
	:onTextInput(nullptr),
	 onTextEditing(nullptr)
{
}

EventListenerTextInput::~EventListenerTextInput()
{
}

bool EventListenerTextInput::init()
{
	EventListener::init("EventListenerTextInput");
	return true;
}

EventListenerTextInput* EventListenerTextInput::clone()
{
	auto listener = EventListenerTextInput::create();
	listener->_bRegistered = this->_bRegistered;
	listener->_paused = this->_paused;
	listener->_associatedNode = this->_associatedNode;

	return listener;
}

bool EventListenerTextInput::checkAvailable() const
{
	return isRegistered() && getAssociatedNode() && !isPaused();
}

NS_SDL_END
