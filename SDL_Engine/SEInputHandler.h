/*
接收系统事件（比如按键，鼠标）并发送给EventDispatcher
*/
#ifndef __SDL_InputHandler_H__
#define __SDL_InputHandler_H__
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <functional>

#include "SDL.h"

#include "SEObject.h"
#include "SEPlatformMarcos.h"
NS_SDL_BEGIN

class Node;
class Touch;
class EventListener;

class InputHandler:public Object
{
private:
	std::vector<Touch*> _touches;
public:
	InputHandler();
	~InputHandler();

	void update();

private:
	void mouseBtnDown(SDL_Event event);
	void mouseBtnUp(SDL_Event event);

	void mouseMotion(SDL_Event event);

	void touchBegan(SDL_Event event);
	void touchMoved(SDL_Event event);
	void touchEnded(SDL_Event event);

	//获得未在使用的Touch
	Touch*getAvaiableTouch();
	//获得对应id的Touch
	Touch*getTouchByID(SDL_FingerID id);
};
NS_SDL_END
#endif
