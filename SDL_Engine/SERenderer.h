#ifndef __Renderer_H__
#define __Renderer_H__
#include <vector>
#include <algorithm>

#include "SDL.h"
#include "SDL_image.h"

#include "SEObject.h"
#include "SEColor.h"
#include "SERect.h"
#include "SEPoint.h"

NS_SDL_BEGIN

class Window;
class Surface;
class Texture;
class Rect;
class RenderCommand;

class Renderer : public Object
{
private:
	SDL_Renderer* _renderer;
	Window* _window;
	//渲染队列
	std::vector<RenderCommand*> _renderQueue;
public:
	Renderer();
	~Renderer();

	static Renderer* create(Window* window,int index,Uint32 flags);
	//初始化
	bool init(Window* win,int index,Uint32 flags);
	//清空渲染器
	int clear();
	//添加命令
	void addCommand(RenderCommand* command);
	//进行绘制 返回渲染次数
	unsigned int render();
	//设置刷新颜色
	void setDrawColor(Uint8 r,Uint8 g,Uint8 b,Uint8 a);
	void setDrawColor(const Color4B& color);
	//获取当前刷新颜色
	Color4B getDrawColor()const;
	//刷新渲染器
	void present();
	//画
	int copy(Texture* texture,const SDL_Rect* srcRect,SDL_Rect* destRect);
	int copy(Texture* texture, const Rect& srcRect, const Rect& destRect);

	int copyEx(Texture* texture,const SDL_Rect* srcRect,SDL_Rect* destRect,const double angle,const SDL_Point* center,SDL_RendererFlip flip);
	int copyEx(Texture* texture,const Rect& srcRect,const Rect& destRect,const double angle,const Point& center,SDL_RendererFlip flip);
	//一些画图函数
	int drawLine(const Point& startPos,const Point& endPos);
	int drawRect(const Rect& rect);

	int fillRect(const Rect&rect);
	//进行与渲染无关的分辨率
	int setScale(float scaleX,float scaleY);
	Size getScale()const;
	//进行逻辑缩放
	int setLogicalSize(int w,int h);
	void getLogicalSize(int& w,int& h);
	//设置填充模式
	int setDrawBlendMode(SDL_BlendMode mode);
	int getDrawBlendMode(SDL_BlendMode* mode);
	//渲染区域
	int setViewport(const SDL_Rect* rect);
	//这里，SDL返回的是void，所以API做了相应改变
	Rect getViewport();
	/*改变渲染目标，为空时则改为默认渲染目标 
	*@ texture 必须有SDL_TEXTUREACCESS_TARGET*/
	int setTarget(Texture* texture);
	/**
	设置遮挡
	*/
	int setClipRect(const SDL_Rect* rect);
	Rect getClipRect();
	//检测是否应该显示
	bool checkVisibility(const Rect& boundingBox) const;
	//返回SDL_Renderer
	::SDL_Renderer* getSDL_Renderer() const;
public:
	static bool commandComparisonLess(RenderCommand* n1, RenderCommand* n2);
};
NS_SDL_END
#endif
