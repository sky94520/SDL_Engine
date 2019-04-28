#include "SERenderer.h"
#include "SEWindow.h"
#include "SETexture.h"
#include "SESurface.h"
#include "SERect.h"
#include "SERenderCommand.h"
NS_SDL_BEGIN

Renderer::Renderer()
	:_renderer(nullptr)
	,_window(nullptr)
{
}

Renderer::~Renderer()
{
	if(_renderer != nullptr)
		SDL_DestroyRenderer(_renderer);

	_renderer = nullptr;

	SDL_SAFE_RELEASE_NULL(_window);
}

Renderer* Renderer::create(Window* window,int index,Uint32 flags)
{
	Renderer* renderer = new Renderer();

	if(renderer && renderer->init(window,index,flags))
		renderer->autorelease();
	else
		SDL_SAFE_DELETE(renderer);

	return renderer;
}

bool Renderer::init(Window* window,int index,Uint32 flags)
{
	_window = window;
	SDL_SAFE_RETAIN(_window);
	//以win作为参数创建
	_renderer = SDL_CreateRenderer(window->_window,index,flags);

	return true;
}

int Renderer::clear()
{
	return SDL_RenderClear(_renderer);	 
}

void Renderer::addCommand(RenderCommand* command)
{
	_renderQueue.push_back(command);
}

unsigned int Renderer::render()
{
	int counts = _renderQueue.size();
	//先进行排序
	std::stable_sort(_renderQueue.begin(), _renderQueue.end(),commandComparisonLess);
	//遍历 渲染后删除该命令
	for (auto it = _renderQueue.begin(); it != _renderQueue.end();)
	{
		auto command = *it;
		command->execute(this);

		it = _renderQueue.erase(it);
	}

	return counts;
}

void Renderer::setDrawColor(Uint8 r,Uint8 g,Uint8 b,Uint8 a)
{
	SDL_SetRenderDrawColor(_renderer,r,g,b,a);
}

void Renderer::setDrawColor(const Color4B& color)
{
	SDL_SetRenderDrawColor(_renderer,color.r,color.g,color.b,color.a);
}

Color4B Renderer::getDrawColor() const
{
	Uint8 r = 0, g = 0, b = 0, a = 0;

	SDL_GetRenderDrawColor(_renderer,&r,&g,&b,&a);
	return Color4B(r,g,b,a);
}

void Renderer::present()
{
	SDL_RenderPresent(_renderer);
}

int Renderer::copy(Texture* texture,const SDL_Rect* srcRect,SDL_Rect* destRect)
{
	return SDL_RenderCopy(_renderer,texture->_texture,srcRect,destRect);
}

int Renderer::copy(Texture* texture, const Rect& srcRect, const Rect& destRect)
{
	SDL_Rect src = srcRect.getSDL_Rect();
	SDL_Rect dest = destRect.getSDL_Rect();

	return this->copy(texture, &src, &dest);
}

int Renderer::copyEx(Texture* texture,const SDL_Rect* srcRect,SDL_Rect* destRect,const double angle,const SDL_Point* center,SDL_RendererFlip flip)
{
	return SDL_RenderCopyEx(_renderer,texture->_texture,srcRect,destRect,angle,center,flip);
}

int Renderer::copyEx(Texture* texture,const Rect&srcRect,const Rect& destRect,const double angle,const Point& center,SDL_RendererFlip flip)
{
	SDL_Rect src = srcRect.getSDL_Rect();
	SDL_Rect dest = destRect.getSDL_Rect();
	SDL_Point c = center.getSDL_Point();

	return SDL_RenderCopyEx(_renderer,texture->_texture,&src,&dest,angle,&c,flip);
}

int Renderer::drawLine(const Point& startPos,const Point& endPos)
{
	int x1 = (int)startPos.x;
	int y1 = (int)startPos.y;
	int x2 = (int)endPos.x;
	int y2 = (int)endPos.y;

	return SDL_RenderDrawLine(_renderer,x1,y1,x2,y2);
}

int Renderer::drawRect(const Rect& rect)
{
	SDL_Rect r = rect.getSDL_Rect();

	return SDL_RenderDrawRect(_renderer,&r);
}

int Renderer::fillRect(const Rect& rect)
{
	SDL_Rect r = rect.getSDL_Rect();

	return SDL_RenderFillRect(_renderer,&r);
}

int Renderer::setScale(float scaleX, float scaleY)
{
	return SDL_RenderSetScale(_renderer,scaleX,scaleY);
}

int Renderer::setLogicalSize(int w,int h)
{
	return SDL_RenderSetLogicalSize(_renderer, w, h);
}

void Renderer::getLogicalSize(int& w,int& h)
{
	SDL_RenderGetLogicalSize(_renderer,&w,&h);
}

int Renderer::setDrawBlendMode(SDL_BlendMode mode)
{
	return SDL_SetRenderDrawBlendMode(_renderer,mode);
}

int Renderer::getDrawBlendMode(SDL_BlendMode *mode)
{
	return SDL_GetRenderDrawBlendMode(_renderer,mode);
}

int Renderer::setViewport(const SDL_Rect* rect)
{
	return SDL_RenderSetViewport(_renderer,rect);
}

Rect Renderer::getViewport()
{
	SDL_Rect rect;
	
	SDL_RenderGetViewport(_renderer,&rect);

	return Rect(rect);
}

int Renderer::setTarget(Texture*texture)
{
	if(texture)
		return SDL_SetRenderTarget(_renderer,texture->_texture);

	return SDL_SetRenderTarget(_renderer,nullptr);
}

int Renderer::setClipRect(const SDL_Rect*rect)
{
	return SDL_RenderSetClipRect(_renderer,rect);
}

Rect Renderer::getClipRect()
{
	SDL_Rect rect;

	SDL_RenderGetClipRect(_renderer,&rect);

	return Rect(rect);
}

bool Renderer::checkVisibility(const Rect& boundingBox) const
{
	Size visibleSize = _window->getWindowSize();
	Rect worldRect(Point::ZERO,visibleSize);

	return worldRect.intersectRect(boundingBox);
}

SDL_Renderer* Renderer::getSDL_Renderer()const
{
	return _renderer;
}

bool Renderer::commandComparisonLess(RenderCommand* n1, RenderCommand* n2)
{
	return n1->getGlobalZOrder() < n2->getGlobalZOrder();
}
NS_SDL_END
