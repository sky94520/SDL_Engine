#include "SEEventListenerAllAtOnce.h"
#include "SETouch.h"
NS_SDL_BEGIN
const std::string EventListenerAllAtOnce::LISTENER_ID = "EventTouchAllAtOnce";

EventListenerAllAtOnce::EventListenerAllAtOnce()
:onTouchesBegan(nullptr),onTouchesMoved(nullptr)
,onTouchesEnded(nullptr),onTouchesCanceled(nullptr)
{
}
EventListenerAllAtOnce::~EventListenerAllAtOnce()
{
}
bool EventListenerAllAtOnce::init()
{
	return EventListener::init(LISTENER_ID);
}
bool EventListenerAllAtOnce::checkAvailable()const
{
	return true;
}
EventListener*EventListenerAllAtOnce::clone()
{
	return nullptr;
}
NS_SDL_END
