#include "SEEventListenerCustom.h"
#include "SEEventCustom.h"

NS_SDL_BEGIN
std::string EventListenerCustom::LISTENER_ID = "EventListenerCustom";
EventListenerCustom::EventListenerCustom()
{
}
EventListenerCustom::~EventListenerCustom()
{
}
EventListenerCustom*EventListenerCustom::create(const std::string&eventName,const std::function<void(EventCustom*)>&callback)
{
	auto listener = new EventListenerCustom();
	if(listener && listener->init(eventName,callback))
		listener->autorelease();
	else
		SDL_SAFE_DELETE(listener);
	return listener;
}
bool EventListenerCustom::init(const std::string&eventName,const std::function<void(EventCustom*)>&callback)
{
	_eventName = eventName;
	onCustomEvent = callback;
	return EventListener::init(LISTENER_ID);
}
EventListenerCustom*EventListenerCustom::clone()
{
	auto listener = EventListenerCustom::create(this->getListenerID(),onCustomEvent);
	return listener;
}
bool EventListenerCustom::checkAvailable()const
{
	return onCustomEvent != nullptr;
}
std::string&EventListenerCustom::getEventName()
{
	return _eventName;
}
NS_SDL_END
