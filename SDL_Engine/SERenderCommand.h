#ifndef __SDL_RenderCommand_H__
#define __SDL_RenderCommand_H__
#include "SDL.h"

#include "SERect.h"
#include "SEColor.h"

NS_SDL_BEGIN

class Texture;
class Renderer;

class RenderCommand
{
protected:
	float _globalZOrder;
public:
	RenderCommand();
	~RenderCommand();
	
	float getGlobalZOrder() const { return _globalZOrder; }
	void setGlobalZOrder(float globalZOrder) { _globalZOrder = globalZOrder;}
	//一般是进行绘制
	virtual void execute(Renderer* renderer);
};
//-------------------------------------TextureCommand-------------------------------
class TextureCommand : public RenderCommand
{
private:
	Texture* _texture;
	Rect _srcRect;
	Rect _destRect;
	float _rotation;
	Point _center;
	SDL_RendererFlip _flip;
	Uint8 _opacity;
	Color3B _colorMod;
public:
	TextureCommand();
	~TextureCommand();

	void init(float globalZOrder, Texture* texture, const Rect& srcRect, const Rect& destRect, float rotation,const Point& center, SDL_RendererFlip flip);
	//透明度
	void setOpacity(Uint8 opacity) { _opacity = opacity; }
	void setColorMod(const Color3B& colorMod) { _colorMod = colorMod; }

	virtual void execute(Renderer* renderer);
};
NS_SDL_END
#endif
