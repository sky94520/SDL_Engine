#ifndef __SDL_Texture_H__
#define __SDL_Texture_H__

#include "SDL.h"
#include "SEObject.h"
NS_SDL_BEGIN

class Renderer;
class Surface;

class Texture : public Object
{
private:
	SDL_Texture* _texture;
public:
	Texture();
	~Texture();
	
	static Texture* create(SDL_Texture* tex);
	//IMG_LoadTexture
	static Texture* create(Renderer* renderer, const std::string& filename);
	//SDL_CreateTexture
	static Texture* create(Renderer* renderer, Uint32 format, int access, int w, int h);
	//SDL_CreateTextureFromSurface
	static Texture* createWithSurface(Renderer* renderer, Surface* surface);

	bool init(SDL_Texture* tex);
	bool init(Renderer* renderer, const std::string& filename);
	bool init(Renderer* renderer, Uint32 format, int access, int w, int h);
	bool initWithSurface(Renderer* renderer, Surface* surface);
	//获得贴图的属性
	void query(Uint32* format,int* access,int* w,int* h);
	//设置追加颜色 srcColor = srcColor*(color/255)	//获得追加颜色
	/*即 如果原来颜色为(255,255,255) 设置为(255,128,255)  结果为(255,128,255)*/
	int setColorMod(Uint8 r,Uint8 g,Uint8 b) const;
	int getColorMod(Uint8* r,Uint8* g,Uint8* b) const;
	//透明度
	int setAlphaMod(Uint8 alpha) const;
	int getAlphaMod(Uint8* alpha) const;
	//设置混合,主要用于SDL_RenderCopy
	/*SDL_BLENDMODE_NONE SDL_BLENDMODE_BLEND SDL_BLEND_ADD SDL_BLEND_ADD*/
	int setBlendMode(SDL_BlendMode blendMode) const;
	int getBlendMode(SDL_BlendMode* blendMode) const;
	//获取SDL_Texture
	SDL_Texture* getSDL_Texture() const;
	//Renderer的友元
	friend class Renderer;
};
NS_SDL_END
#endif
