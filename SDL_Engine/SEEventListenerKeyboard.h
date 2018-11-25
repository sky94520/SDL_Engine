#ifndef __SDL_EventListenerKeyboard_H__
#define __SDL_EventListenerKeyboard_H__
#include<string>
#include<functional>
#include "SDL.h"
#include "SEEventListener.h"
NS_SDL_BEGIN
class EventListenerKeyboard:public EventListener
{
public:
	static std::string LISTENER_ID;
public:
	std::function<void (SDL_Keycode,SDL_Event*)> onKeyPressed;
	std::function<void (SDL_Keycode,SDL_Event*)> onKeyReleased;
public:
	EventListenerKeyboard();
	~EventListenerKeyboard();
	CREATE_FUNC(EventListenerKeyboard);
	bool init();

	virtual EventListenerKeyboard*clone();
	virtual bool checkAvailable()const;
};
NS_SDL_END
#endif
