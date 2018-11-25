#ifndef __Window_H__
#define __Window_H__
#include "SDL.h"
#include "SEObject.h"
NS_SDL_BEGIN
class Surface;
class Renderer;
class Size;

class Window : public Object
{
private:
	SDL_Window* _window;
	//窗口坐标和大小 
	SDL_Point _position;
	int _width;
	int _height;
public:
	Window();
	~Window();

	static Window* create(const char* title,int x,int y,int width,int height,Uint32 flags);
	bool init(const char* title,int x,int y,int width,int height,Uint32 flags);
	//更新
	int updateWindowSurface();
	//销毁窗口
	void destroyWindow();
	//获得窗口的surface
	Surface* getWindowSurface();
	//显示窗口
	void showWindow();
	//隐藏窗口
	void hideWindow();
	//获得窗口大小
	Size getWindowSize()const;
	//获得窗口真实大小
	Size getRealWindowSize()const;
	//更新窗口位置并返回
	SDL_Point getWindowPosition();
	//Renderer作为友元
	friend class Renderer;
};
NS_SDL_END
#endif
