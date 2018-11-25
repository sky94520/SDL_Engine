#ifndef __EventListenerTouchAllAtOnce_H__
#define __EventListenerTouchAllAtOnce_H__
#include<vector>
#include<functional>
#include "SEEventListener.h"

NS_SDL_BEGIN
class Touch;
class EventListenerAllAtOnce:public EventListener
{
public:
 	static const std::string LISTENER_ID;
public:
	typedef std::function<void (std::vector<Touch*>,SDL_Event*)> ccTouchesCallback;
	ccTouchesCallback onTouchesBegan;
	ccTouchesCallback onTouchesMoved;
	ccTouchesCallback onTouchesEnded;
	ccTouchesCallback onTouchesCanceled;
public:
	public:
	EventListenerAllAtOnce();
	~EventListenerAllAtOnce();
	CREATE_FUNC(EventListenerAllAtOnce);
	bool init();
	virtual bool checkAvailable()const;
	
	virtual EventListener*clone();
};
NS_SDL_END
#endif
