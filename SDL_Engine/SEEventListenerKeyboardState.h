#ifndef __EventListenerKeyboardState_H__
#define __EventListenerKeyboardState_H__

#include<string>
#include<functional>
#include "SEEventListener.h"
NS_SDL_BEGIN
class EventListenerKeyboardState:public EventListener
{
public:
	static std::string LISTENER_ID;
public:
	std::function<void (const Uint8*,SDL_Event*)> onEvent;
public:
	EventListenerKeyboardState();
	~EventListenerKeyboardState();
	CREATE_FUNC(EventListenerKeyboardState);
	bool init();

	virtual EventListenerKeyboardState*clone();
	virtual bool checkAvailable()const;
};
NS_SDL_END
#endif
