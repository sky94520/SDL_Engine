#ifndef __EventListenerMouse_H__
#define __EventListenerMouse_H__
#include "SEEventListener.h"

#include<string>
#include<functional>

NS_SDL_BEGIN
class EventListenerMouse:public EventListener
{
public:
	static std::string LISTENER_ID;
public:
	std::function<void(SDL_MouseButtonEvent*)> onMouseDown;
	std::function<void(SDL_MouseButtonEvent*)> onMouseUp;
	std::function<void(SDL_MouseMotionEvent*)> onMouseMotion;
	std::function<void(SDL_MouseWheelEvent*)> onMouseScroll;
public:
	EventListenerMouse();
	~EventListenerMouse();
	CREATE_FUNC(EventListenerMouse);
	bool init();

	virtual EventListenerMouse*clone();
	virtual bool checkAvailable()const;
};
NS_SDL_END
#endif
