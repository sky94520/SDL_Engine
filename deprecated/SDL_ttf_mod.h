#ifndef __SDL_ttf_mod_H__
#define __SDL_ttf_mod_H__

#include "SDL_ttf.h"
#include <ft2build.h>
#include FT_FREETYPE_H

//动态修改字体大小
extern void TTF_SetFontSizeMod(TTF_Font *font,int ptsize);

#endif
