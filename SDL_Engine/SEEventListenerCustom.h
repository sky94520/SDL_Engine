#ifndef __EventListenerCustom_H__
#define __EventListenerCustom_H__
#include<string>
#include<functional>
#include "SEEventListener.h"
class EventCustom;
NS_SDL_BEGIN

class EventListenerCustom:public EventListener
{
private:
	std::string _eventName;
public:
	std::function<void(EventCustom*)> onCustomEvent;
public:
	static std::string LISTENER_ID;
	EventListenerCustom();
	~EventListenerCustom();
	static EventListenerCustom*create(const std::string&eventName,const std::function<void(EventCustom*)>&callback);
	bool init(const std::string&eventName,const std::function<void(EventCustom*)>&callback);
	virtual EventListenerCustom*clone();
	virtual bool checkAvailable()const;

	std::string&getEventName();
};
NS_SDL_END
#endif
