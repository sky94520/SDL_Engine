#include "SETexture.h"
#include "SERenderer.h"
#include "SESurface.h"

NS_SDL_BEGIN

Texture::Texture()
{
	_texture = nullptr;
}

Texture::~Texture()
{
	if(_texture != nullptr)
		SDL_DestroyTexture(_texture);
}

Texture* Texture::create(SDL_Texture* tex)
{
	Texture* texture = new Texture();

	if(texture && texture->init(tex))
		texture->autorelease();
	else
		SDL_SAFE_DELETE(texture);

	return texture;
}
Texture* Texture::create(Renderer* renderer, const std::string& filename)
{
	Texture* texture = new Texture();

	if(texture && texture->init(renderer, filename))
		texture->autorelease();
	else
		SDL_SAFE_DELETE(texture);

	return texture;
}
Texture* Texture::create(Renderer* renderer, Uint32 format, int access, int w, int h)
{
	Texture* texture = new Texture();

	if(texture && texture->init(renderer, format, access, w, h))
		texture->autorelease();
	else
		SDL_SAFE_DELETE(texture);

	return texture;
}
Texture* Texture::createWithSurface(Renderer* renderer, Surface* surface)
{
	Texture* texture = new Texture();

	if(texture && texture->initWithSurface(renderer, surface))
		texture->autorelease();
	else
		SDL_SAFE_DELETE(texture);

	return texture;
}

bool Texture::init(SDL_Texture* pTex)
{
	if(pTex != nullptr)
	{
		_texture = pTex;
		return true;
	}
	return false;
}

bool Texture::init(Renderer* renderer, const std::string& filename)
{
	SDL_Texture* tex = IMG_LoadTexture(renderer->getSDL_Renderer(), filename.c_str());

	if (tex != nullptr)
	{       
       		 _texture = tex;
        	return true;
	}
	LOG("error:%s\n", IMG_GetError());
	return false;
}

bool Texture::init(Renderer* renderer, Uint32 format, int access, int w, int h)
{
	SDL_Texture* tex = SDL_CreateTexture(renderer->getSDL_Renderer(), format, access, w, h);

	if (tex != nullptr)
	{
		_texture = tex;
		return true;
	}
	LOG("error:%s\n", SDL_GetError());
	return false;
}

bool Texture::initWithSurface(Renderer* renderer, Surface* surface)
{
	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer->getSDL_Renderer(), surface->getSDL_Surface());
	
	if (tex != nullptr)
	{
		_texture = tex;
		return true;
	}
	LOG("error:%s\n", SDL_GetError());
	return false;
}

void Texture::query(Uint32* format,int* access,int* w,int* h)
{
	if(_texture)
	{
		SDL_QueryTexture(_texture,format,access,w,h);
	}
}

int Texture::setColorMod(Uint8 r,Uint8 g,Uint8 b) const
{
	if(_texture)
		return SDL_SetTextureColorMod(_texture,r,g,b);
	return -1;
}

int Texture::getColorMod(Uint8* r,Uint8* g,Uint8* b) const
{
	if(_texture)
		return SDL_GetTextureColorMod(_texture,r,g,b);
	return -1;
}

int Texture::setAlphaMod(Uint8 alpha) const
{
	if(_texture)
		return SDL_SetTextureAlphaMod(_texture,alpha);
	return -1;
}

int Texture::getAlphaMod(Uint8* alpha) const
{
	if(_texture)
		return SDL_GetTextureAlphaMod(_texture,alpha);
	return -1;
}

int Texture::setBlendMode(SDL_BlendMode blendMode) const
{
	if(_texture)
		return SDL_SetTextureBlendMode(_texture,blendMode);
	return -1;
}

int Texture::getBlendMode(SDL_BlendMode* blendMode) const
{
	if(_texture)
		return SDL_GetTextureBlendMode(_texture,blendMode);
	return -1;
}

SDL_Texture* Texture::getSDL_Texture() const
{
	return _texture;
}
NS_SDL_END
