#include "SEWindow.h"
#include "SESurface.h"
#include "SESize.h"
NS_SDL_BEGIN

Window::Window()
{
	_window = NULL;
}

Window::~Window()
{
	SDL_DestroyWindow(_window);
	_window = nullptr;
}

bool Window::init(const char* title,int x,int y,int width,int height,Uint32 flags)
{
	_window = SDL_CreateWindow(title,x,y,width,height,flags);
	//initlize
	_position.x = x;
	_position.y = y;
	_width = width;
	_height = height;

	return true;
}

Window* Window::create(const char* title,int x,int y,int width,int height,Uint32 flags)
{
	Window* window = new Window();

	if(window && window->init(title,x,y,width,height,flags))
		window->autorelease();
	else
		SDL_SAFE_DELETE(window);

	return window;
}

int Window::updateWindowSurface()
{
	if(_window != nullptr)
		return SDL_UpdateWindowSurface(_window);
	else
		return -1;
}

Surface* Window::getWindowSurface()
{
	if(_window != nullptr)
		return Surface::create(SDL_GetWindowSurface(_window));
	else
		return NULL;
}

void Window::showWindow()
{
	SDL_ShowWindow(_window);
}

void Window::hideWindow()
{
	SDL_HideWindow(_window);
}

Size Window::getWindowSize()const
{
	return Size((float)_width,(float)_height);
}

Size Window::getRealWindowSize()const
{
	int w = 0;
	int h = 0;

	SDL_GetWindowSize(_window,&w,&h);

	return Size((float)w,(float)h);
}

SDL_Point Window::getWindowPosition()
{
	if(_window != nullptr)
	{
		SDL_GetWindowPosition(_window,&_position.x,&_position.y);
	}
	return _position;
}

NS_SDL_END
