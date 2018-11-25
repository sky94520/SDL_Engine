#include "SEFont.h"
#include "SEFileUtils.h"
NS_SDL_BEGIN
Font::Font()
	:_font(nullptr)
	,_path("")
{
}
Font::~Font()
{
	this->destroyFont();
}
Font*Font::create(const char*path,int size)
{
	auto font = new Font();
	if(font && font->init(path,size))
		font->autorelease();
	else
		SDL_SAFE_DELETE(font);
	return font;
}
bool Font::init(const char*path,int size)
{
	//initlize ,if failed,return false
	if(TTF_WasInit() == 0 && TTF_Init() != 0)
	{
		LOG("errror:%s\n",TTF_GetError());
		return false;
	}
	auto fullpath = FileUtils::getInstance()->fullPathForFilename(path);
	if(fullpath.empty())
		return false;
	_font = TTF_OpenFont(fullpath.c_str(),size);
	if(!_font)
	{
		LOG("error:%s",TTF_GetError());
		return false;
	}
	//好像好多变量都不需要
	_path = path;
	_size = size;

	return _font?true:false;
}
void Font::destroyFont()
{
	if(_font)
	{
		TTF_CloseFont(_font);
		_font = nullptr;
	}
}
//-----------------------TEXT-----------------------------
Surface*Font::ttfRenderText_Solid(const char*text,const Color4B& color)
{
	SDL_Color fg={color.r,color.g,color.b,color.a};
	return _font?Surface::create(TTF_RenderText_Solid(_font,text,fg)):nullptr;
}
Surface*Font::ttfRenderText_Shaded(const char*text,const Color4B& fg,const Color4B& bg)
{
	SDL_Color sfg = {fg.r,fg.g,fg.b,fg.a};
	SDL_Color sbg = {bg.r,bg.g,bg.b,bg.a};
	return _font?Surface::create(TTF_RenderText_Shaded(_font,text,sfg,sbg)):nullptr;
}
Surface*Font::ttfRenderText_Blended(const char*text,const Color4B&color)
{
	SDL_Color fc = {color.r,color.g,color.b,color.a};
	return _font?Surface::create(TTF_RenderText_Blended(_font,text,fc)):nullptr;
}
Surface*Font::ttfRenderText_Blended_Wrapped(const char*text,const Color4B&color,Uint32 wrapLine)
{
	SDL_Color fontcolor = {color.r,color.g,color.b,color.a};
	return _font?Surface::create(TTF_RenderText_Blended_Wrapped(_font,text,fontcolor,wrapLine)):nullptr;
}
//----------------------UTF-8---------------------------------
Surface*Font::ttfRenderUTF8_Solid(const char*text,const Color4B& color)
{
	SDL_Color fg={color.r,color.g,color.b,color.a};
	return _font?Surface::create(TTF_RenderUTF8_Solid(_font,text,fg)):nullptr;
}
Surface*Font::ttfRenderUTF8_Shaded(const char*text,const Color4B& fg,const Color4B &bg)
{
	SDL_Color sfg = {fg.r,fg.g,fg.b,fg.a};
	SDL_Color sbg = {bg.r,bg.g,bg.b,bg.a};
	return _font?Surface::create(TTF_RenderUTF8_Shaded(_font,text,sfg,sbg)):nullptr;

}
Surface*Font::ttfRenderUTF8_Blended(const char*text,const Color4B &color)
{
	SDL_Color fc = {color.r,color.g,color.b,color.a};
	return _font?Surface::create(TTF_RenderUTF8_Blended(_font,text,fc)):nullptr;
}
Surface*Font::ttfRenderUTF8_Blended_Wrapped(const char*text,const Color4B&color,Uint32 wrapLine)
{
	SDL_Color fc = {color.r,color.g,color.b,color.a};
	return _font?Surface::create(TTF_RenderUTF8_Blended_Wrapped(_font,text,fc,wrapLine)):nullptr;
}
//--------------------unicode--------------------------
Surface*Font::ttfRenderUNICODE_Solid(const Uint16*text,const Color4B& color)
{
	SDL_Color fg={color.r,color.g,color.b,color.a};
	return _font?Surface::create(TTF_RenderUNICODE_Solid(_font,text,fg)):nullptr;
}
Surface*Font::ttfRenderUNICODE_Shaded(const Uint16*text,const Color4B& fg,const Color4B &bg)
{
	SDL_Color sfg = {fg.r,fg.g,fg.b,fg.a};
	SDL_Color sbg = {bg.r,bg.g,bg.b,bg.a};
	return _font?Surface::create(TTF_RenderUNICODE_Shaded(_font,text,sfg,sbg)):nullptr;

}
Surface*Font::ttfRenderUNICODE_Blended(const Uint16*text,const Color4B &color)
{
	SDL_Color fc = {color.r,color.g,color.b,color.a};
	return _font?Surface::create(TTF_RenderUNICODE_Blended(_font,text,fc)):nullptr;
}
Surface*Font::ttfRenderUNICODE_Blended_Wrapped(const Uint16*text,const Color4B&color,Uint32 wrapLine)
{
	SDL_Color fc = {color.r,color.g,color.b,color.a};
	return _font?Surface::create(TTF_RenderUNICODE_Blended_Wrapped(_font,text,fc,wrapLine)):nullptr;
}
//-----------------------GLYPH-----------------------------------
Surface*Font::ttfRenderGlyph_Solid(Uint16 ch,const Color4B& color)
{
	SDL_Color fg={color.r,color.g,color.b,color.a};
	return _font?Surface::create(TTF_RenderGlyph_Solid(_font,ch,fg)):nullptr;
}
Surface*Font::ttfRenderGlyph_Shaded(Uint16 ch,const Color4B& fg,const Color4B &bg)
{
	SDL_Color sfg = {fg.r,fg.g,fg.b,fg.a};
	SDL_Color sbg = {bg.r,bg.g,bg.b,bg.a};
	return _font?Surface::create(TTF_RenderGlyph_Shaded(_font,ch,sfg,sbg)):nullptr;

}
Surface*Font::ttfRenderGlyph_Blended(Uint16 ch,const Color4B &color)
{
	SDL_Color fc = {color.r,color.g,color.b,color.a};
	return _font?Surface::create(TTF_RenderGlyph_Blended(_font,ch,fc)):nullptr;
}
//-------------------相关高度-----------------------------------
int Font::ttfFontHeight()const
{
	return _font?TTF_FontHeight(_font):0;
}
int Font::ttfFontLineSkip()const
{
	return _font?TTF_FontLineSkip(_font):0;
}
int Font::ttfFontDescent()const
{
	return _font?TTF_FontDescent(_font):0;
}
int Font::ttfFontAscent()const
{
	return _font?TTF_FontAscent(_font):0;
}
int Font::ttfGlyphMetrics(Uint16 ch,int*minX,int*maxX,int*minY,int*maxY,int*advance)
{
	return _font?TTF_GlyphMetrics(_font,ch,minX,maxX,minY,maxY,advance):-1;
}
int Font::ttfSizeText(const char*text,int*w,int*h)
{
	return _font?TTF_SizeText(_font,text,w,h):-1;
}
int Font::ttfSizeUTF8(const char*text,int*w,int*h)
{
	return _font?TTF_SizeUTF8(_font,text,w,h):-1;
}
int Font::ttfSizeUNICODE(const Uint16*text,int*w,int*h)
{
	return _font?TTF_SizeUNICODE(_font,text,w,h):-1;
}
//---------------------------------------------------------
bool Font::ttfSetFontStyle(int style)
{
	if(_font)
	{
		TTF_SetFontStyle(_font,style);
		return true;
	}
	return false;
}
int Font::ttfGetFontStyle()const
{
	return _font?TTF_GetFontStyle(_font):TTF_STYLE_NORMAL;
}
bool Font::ttfSetFontOutline(int outline)
{
	if(_font)
	{
		TTF_SetFontOutline(_font,outline);
		return outline == TTF_GetFontOutline(_font);
	}
	return false;
}
int Font::ttfGetFontOutline()const
{
	return _font?TTF_GetFontOutline(_font):0;
}
//抗锯齿
bool Font::ttfSetFontHinting(int hinting)
{
	if(_font)
	{
		TTF_SetFontHinting(_font,hinting);
		return hinting == TTF_GetFontHinting(_font);
	}
	return false;
}
int Font::ttfGetFontHinting()const
{
	return _font?TTF_GetFontHinting(_font):TTF_HINTING_NORMAL;
}
//调整字距
bool Font::ttfSetFontKerning(int allowed)
{
	if(_font)
	{
		TTF_SetFontKerning(_font,allowed);
		return true;
	}
	return false;
}
int Font::ttfGetFontKerning()const
{
	return _font?TTF_GetFontKerning(_font):0;
}
//---------------------------------------------------------
const char*Font::ttfFontFaceFamilyName()
{
	//return _font?TTF_FontFaceFamilyName(_font):nullptr;
	return _path.c_str();
}
long Font::ttfFontFaces()const
{
	return _font?TTF_FontFaces(_font):0;
}
NS_SDL_END
