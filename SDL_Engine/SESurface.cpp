#include "SESurface.h"
NS_SDL_BEGIN

Surface::Surface()
	:_surface(nullptr)
{
}


Surface::~Surface()
{
	SDL_FreeSurface(_surface);
	_surface = nullptr;
}

Surface* Surface::create(SDL_Surface* surface)
{
	Surface* sur = new Surface();

	if(sur && sur->init(surface))
		sur->autorelease();
	else
		SDL_SAFE_DELETE(sur);

	return sur;
}
Surface* Surface::create(const std::string& filepath)
{
	auto sur = new Surface();

	if(sur&&sur->init(filepath))
		sur->autorelease();
	else
		SDL_SAFE_DELETE(sur);

	return sur;
}

Surface* Surface::createWithBMP(const std::string& filepath)
{
	auto sur = new Surface();

	if(sur&&sur->initWithBMP(filepath))
		sur->autorelease();
	else
		SDL_SAFE_DELETE(sur);

	return sur;
}

Surface*Surface::createWithRGBA(Uint32 flags,int width,int height,int depth,Uint32 Rmask,Uint32 Gmask,Uint32 Bmask,Uint32 Amask)
{
	Surface*sur = new Surface();

	if(sur && sur->initWithRGBA(flags,width,height,depth,Rmask,Gmask,Bmask,Amask))
		sur->autorelease();
	else
		SDL_SAFE_DELETE(sur);

	return sur;
}

bool Surface::init(SDL_Surface* sur)
{
	_surface = sur;
	SDL_GetClipRect(_surface,&_rect);

	return true;
}

bool Surface::init(const std::string& filepath)
{
	_surface = IMG_Load(filepath.c_str());

	if(_surface == nullptr)
	{
		LOG("error:%s\n",IMG_GetError());
		return false;
	}
	SDL_GetClipRect(_surface,&_rect);

	return true;
}

bool Surface::initWithBMP(const std::string& filepath)
{
	_surface = SDL_LoadBMP(filepath.c_str());

	if(_surface == nullptr)
	{
		LOG("error:%s\n",SDL_GetError());
		return false;
	}
	SDL_GetClipRect(_surface,&_rect);

	return true;
}

bool Surface::initWithRGBA(Uint32 flags,int width,int height,int depth,Uint32 Rmask,Uint32 Gmask,Uint32 Bmask,Uint32 Amask)
{
	_surface = SDL_CreateRGBSurface(flags,width,height,depth,Rmask,Gmask,Bmask,Amask);
	SDL_GetClipRect(_surface,&_rect);

	return true;
}

int Surface::blit(const SDL_Rect* srcRect,Surface* dest,SDL_Rect* destRect)
{
	return SDL_BlitSurface(_surface,srcRect,dest->_surface,destRect);
}

Surface* Surface::convert(const SDL_PixelFormat* fmt,Uint32 flags)
{
	SDL_Surface* sur = SDL_ConvertSurface(_surface,fmt,flags);
	return Surface::create(sur);
}

int Surface::fillRect(const SDL_Rect* rect,Uint32 color)
{
	return SDL_FillRect(_surface,rect,color);
}

Uint32 Surface::mapRGB(Uint8 r,Uint8 g,Uint8 b) const
{
	return SDL_MapRGB(_surface->format,r,g,b);
}

Uint32 Surface::mapRGBA(Uint8 r,Uint8 g,Uint8 b,Uint8 a) const
{
	return SDL_MapRGBA(_surface->format,r,g,b,a);
}

int Surface::setColorKey(Uint32 colorkey)const
{
	return SDL_SetColorKey(_surface,SDL_TRUE,colorkey);
}

Uint32 Surface::getARGB(int x,int y) const
{
	int index = y * _surface->w + x;
	Uint32* pixels = (Uint32*)_surface->pixels;

	return pixels[index];
}

int Surface::setBlendMode(SDL_BlendMode mode)
{
	return SDL_SetSurfaceBlendMode(_surface,mode);
}

int Surface::getBlendMode(SDL_BlendMode* mode)
{
	return SDL_GetSurfaceBlendMode(_surface,mode);
}

int Surface::savePNG(const char* filename)
{
	return IMG_SavePNG(_surface, filename);
}

SDL_Surface* Surface::getSDL_Surface() const
{
	return _surface;
}

SDL_Rect Surface::getClipRect() const
{
	return _rect;
}

NS_SDL_END
