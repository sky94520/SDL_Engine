#ifndef _SDL_GFX_MOD_H
#define _SDL_GFX_MOD_H

#include "SDL2_gfxPrimitives.h"

#ifdef __cplusplus
extern "C" {
#endif

	//texturePolygon 函数的修改版（参数5改为 SDL_Texture 类型）
	SDL2_GFXPRIMITIVES_SCOPE int texturedPolygonMod(SDL_Renderer * renderer, const Sint16 * vx, const Sint16 * vy, int n, SDL_Texture * texture, int texture_dx, int texture_dy);

#ifdef __cplusplus
}
#endif

#endif