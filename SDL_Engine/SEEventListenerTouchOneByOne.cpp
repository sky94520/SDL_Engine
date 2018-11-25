#include "SEEventListenerTouchOneByOne.h"
#include "SETouch.h"
NS_SDL_BEGIN
const std::string EventListenerTouchOneByOne::LISTENER_ID="EventListenerTouchOneByOne";
EventListenerTouchOneByOne::EventListenerTouchOneByOne()
	:_bTouchBegan(false)
	,onTouchBegan(nullptr)
	,onTouchMoved(nullptr)
	,onTouchEnded(nullptr)
	,onTouchCancelled(nullptr)
{
}
EventListenerTouchOneByOne::~EventListenerTouchOneByOne()
{
}
EventListenerTouchOneByOne*EventListenerTouchOneByOne::create()
{
	auto listener = new EventListenerTouchOneByOne();
	if(listener && listener->init())
		listener->autorelease();
	else
		SDL_SAFE_DELETE(listener);
	return listener;
}
bool EventListenerTouchOneByOne::init()
{
	EventListener::init(LISTENER_ID);
	return true;
}

bool EventListenerTouchOneByOne::isTouchBegan()const
{
	return _bTouchBegan;
}

void EventListenerTouchOneByOne::setTouchBegan(bool touchBegan)
{
	_bTouchBegan = touchBegan;
}

bool EventListenerTouchOneByOne::checkAvailable()const
{
	if(onTouchBegan ==nullptr && onTouchMoved==nullptr && 
		onTouchEnded==nullptr&&onTouchCancelled==nullptr)
		return false;
	return true;
}
EventListener*EventListenerTouchOneByOne::clone()
{
	auto listener=EventListenerTouchOneByOne::create();
	listener->onTouchBegan = this->onTouchBegan;
	listener->onTouchMoved = this->onTouchMoved;
	listener->onTouchEnded = this->onTouchEnded;
	listener->onTouchCancelled = this->onTouchCancelled;
	listener->setPriority(this->getPriority());
	listener->setSwallowTouches(this->isSwallowTouches());
	return listener;
}
NS_SDL_END
