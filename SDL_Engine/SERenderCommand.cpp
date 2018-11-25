#include "SERenderCommand.h"
#include "SETexture.h"
#include "SERenderer.h"

NS_SDL_BEGIN
RenderCommand::RenderCommand()
	:_globalZOrder(0.f)
{
}

RenderCommand::~RenderCommand()
{
}

void RenderCommand::execute(Renderer* renderer)
{
}
//-------------------------------------TextureCommand-------------------------------
TextureCommand::TextureCommand()
	:_texture(nullptr)
	,_rotation(0.f)
	,_flip(SDL_FLIP_NONE)
	,_opacity(255)
	,_colorMod(255,255,255)
{
}

TextureCommand::~TextureCommand()
{
	SDL_SAFE_RELEASE_NULL(_texture);
}

void TextureCommand::init(float globalZOrder, Texture* texture, const Rect& srcRect, const Rect& destRect, float rotation,const Point& center, SDL_RendererFlip flip)
{
	_globalZOrder = globalZOrder;

	if (_texture != texture)
	{
		SDL_SAFE_RETAIN(texture);
		SDL_SAFE_RELEASE(_texture);

		_texture = texture;
	}
	_srcRect = srcRect;
	_destRect = destRect;
	_rotation = rotation;
	_center = center;
	_flip = flip;
}

void TextureCommand::execute(Renderer* renderer)
{
	//设置透明度和ColorMod
	_texture->setAlphaMod(_opacity);
	_texture->setColorMod(_colorMod.r,_colorMod.g,_colorMod.b);
	//进行绘制
	renderer->copyEx(_texture, _srcRect, _destRect, _rotation, _center, _flip);
}
NS_SDL_END
