#ifndef __SDL_Font_H__
#define __SDL_Font_H__
#include<string>

#include "SDL_ttf.h"

#include "SEObject.h"
#include "SESurface.h"
#include "SEColor.h"
/*字体类，封装了TTF的功能
命名规则 TTF_ 转换为ttf
新增换行wrap函数
*/
NS_SDL_BEGIN

class Font : public Object
{
private:
	TTF_Font*_font;
	//ttf文件路径
	std::string _path;
	int _size;
	/*TTF_STYLE_BOLD TTF_STYLE_ITALIC TTF_STYLE_UNDERLINE TTF_STYLE_STRIKETHROUGH*/
	/*抗锯齿 TTF_HINTING_NORMAL TTF_HINTING_LIGHT TTF_HINTLING_MONO TTF_HINTING_NONE*/
public:
	Font();
	virtual ~Font();
	static Font* create(const char*path,int fontSize);
	bool init(const char*path,int fontSize);
	void destroyFont();
	//text
	Surface*ttfRenderText_Solid(const char*text,const Color4B& color);
	Surface*ttfRenderText_Shaded(const char*text,const Color4B& fg,const Color4B &bg);
	Surface*ttfRenderText_Blended(const char*text,const Color4B &color);
	//换行字符串
	Surface*ttfRenderText_Blended_Wrapped(const char*text,const Color4B&color,Uint32 wrapLine);
	//utf-8
	Surface*ttfRenderUTF8_Solid(const char*text,const Color4B& color);
	Surface*ttfRenderUTF8_Shaded(const char*text,const Color4B& fg,const Color4B &bg);
	Surface*ttfRenderUTF8_Blended(const char*text,const Color4B &color);
	Surface*ttfRenderUTF8_Blended_Wrapped(const char*text,const Color4B&color,Uint32 wrapLine);
	//unicode
	Surface*ttfRenderUNICODE_Solid(const Uint16*text,const Color4B& color);
	Surface*ttfRenderUNICODE_Shaded(const Uint16*text,const Color4B& fg,const Color4B &bg);
	Surface*ttfRenderUNICODE_Blended(const Uint16*text,const Color4B &color);
	Surface*ttfRenderUNICODE_Blended_Wrapped(const Uint16*text,const Color4B&color,Uint32 wrapLine);
	//glyph
	Surface*ttfRenderGlyph_Solid(Uint16 ch,const Color4B&color);
	Surface*ttfRenderGlyph_Shaded(Uint16 ch,const Color4B&fg,const Color4B&bg);
	Surface*ttfRenderGlyph_Blended(Uint16 ch,const Color4B&color);
	//获得该Font的最高高度 通常等于_size height=descent+ascent
	int ttfFontHeight()const;
	//获得换行高度
	int ttfFontLineSkip()const;
	//获得基于Baseline的descent
	int ttfFontDescent()const;
	//获得基于Baseline的ascent
	int ttfFontAscent() const;
	//获取某个字符的相关信息
	int ttfGlyphMetrics(Uint16 ch,int*minX,int*maxX,int*minY,int*maxY,int*advance);
	//------------------------------------------------------------
	int ttfSizeText(const char*text,int*w,int*h);
	int ttfSizeUTF8(const char*text,int*w,int*h);
	int ttfSizeUNICODE(const Uint16*text,int*w,int*h);
	//------------------------------------------------------------
	//字体表现
	bool ttfSetFontStyle(int style);
	int ttfGetFontStyle()const;
	//设置轮廓 可能会失败
	bool ttfSetFontOutline(int outline);
	int ttfGetFontOutline()const;
	//抗锯齿 可能会失败
	bool ttfSetFontHinting(int hinting);
	int ttfGetFontHinting()const;
	//调整字距
	bool ttfSetFontKerning(int allowed);
	int ttfGetFontKerning()const;
	//------------------------------------------------------------
	const char*ttfFontFaceFamilyName();
	//暂时不知道什么作用
	long ttfFontFaces()const;
};
NS_SDL_END
#endif
