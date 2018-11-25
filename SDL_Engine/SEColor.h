#ifndef __SDL_Color_H__
#define __SDL_Color_H__
#include<string>
#include "SDL.h"
#include "SEObject.h"
NS_SDL_BEGIN
//---------------------------Color4B------------------------------------
class Color4B
{
public:
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 a;
public:
	Color4B(Uint8 r = 0,Uint8 g = 0,Uint8 b = 0,Uint8 a = 255)
		:r(r),g(g),b(b),a(a)
	{
	}
	Color4B(const SDL_Color& color)
	{
		r = color.r;
		g = color.g;
		b = color.b;
		a = color.a;
	}
	/*Format 255,255,255,255*/
	Color4B(const std::string& str)
	{
		int rr = 0, gg = 0, bb = 0, aa = 0;

		SDL_sscanf(str.c_str(),"%d,%d,%d,%d",&rr,&gg,&bb,&aa);
		r= rr, g = gg, b = bb, a = aa;
	}
	bool equals(const Color4B& other) const
	{
		if(this == &other)
			return true;
		return r == other.r
			&& g == other.g 
			&& b == other.b
			&&a == other.a;
	}
	//重载运算符
	Color4B operator*(Uint8 d)
	{
		return Color4B(r * d, g * d, b * d, a * d);
	}
	Color4B operator+(const Color4B& color)
	{
		return Color4B(r + color.r, g + color.g, b + color.b,a + color.a);
	}
	Color4B operator-(const Color4B& color)
	{
		return Color4B(r - color.r, g - color.g, b - color.b);
	}
	//重载== !=
	bool operator==(const Color4B& other)
	{
		if(this == &other)
			return true;
		return r == other.r
			&& g == other.g
			&& b == other.b
			&& a == other.a;
	}
	bool operator!=(const Color4B& other)
	{
		if(this == &other)
			return false;
		return r != other.r 
			|| g != other.g
			|| b != other.b 
			|| a != other.a;
	}
	//转换成SDL_Color
	SDL_Color getSDL_Color()const
	{
		SDL_Color color = { r, g, b, a};
		return color;
	}
};
//----------------------------------------Color3B-------------------------------
class Color3B
{
public:
	Uint8 r;
	Uint8 g;
	Uint8 b;
public:
	Color3B(Uint8 r = 0, Uint8 g = 0,Uint8 b = 0)
		:r(r),g(g),b(b)
	{
	}
	Color3B(const SDL_Color& color)
	{
		r = color.r;
		g = color.g;
		b = color.b;
	}
	/*Format 255,255,255*/
	Color3B(const std::string& str)
	{
		int rr = 0, gg = 0, bb = 0;

		SDL_sscanf(str.c_str(),"%d,%d,%d", &rr, &gg, &bb);
		r = rr, g = gg, b = bb;
	}
	bool equals(const Color3B& other) const
	{
		if(this == &other)
			return true;
		return r == other.r
			&& g == other.g 
			&& b == other.b;
	}
	//重载运算符
	Color3B operator*(Uint8 d)
	{
		return Color3B(r * d, g * d, b * d);
	}
	Color3B operator+(const Color3B& color)
	{
		return Color3B(r + color.r, g + color.g, b + color.b);
	}
	Color3B operator-(const Color3B& color)
	{
		return Color3B(r - color.r, g - color.g, b - color.b);
	}
	//重载== !=
	bool operator==(const Color3B& other)
	{
		if(this == &other)
			return true;
		return r == other.r
			&& g == other.g 
			&& b == other.b;
	}
	bool operator!=(const Color3B& other)
	{
		if(this == &other)
			return false;
		return r != other.r 
			|| g != other.g 
			|| b != other.b;
	}
	//转换成SDL_Color
	SDL_Color getSDL_Color(Uint8 alpha = 255)const
	{
		SDL_Color color={ r, g, b, alpha};
		return color;
	}
};
//----------------------------------Color4F-----------------------------------------
class Color4F
{
public:
	float r;
	float g;
	float b;
	float a;
public:
	Color4F()
	:r(0.f), g(0.f), b(0.f), a(0.f)
	{
	}
	Color4F(float r, float g, float b, float a)
	:r(r), g(g), b(b), a(a)
	{
	}
	explicit Color4F(const Color3B& color, float a = 1.f)
	:r(color.r / 255.f)
	,g(color.g / 255.f)
	,b(color.b / 255.f)
	,a(a)
	{
	}
	explicit Color4F(const Color4B& color)
	:r(color.r / 255.f)
	,g(color.g / 255.f)
	,b(color.b / 255.f)
	,a(color.a / 255.f)
	{
	}

	bool operator==(const Color4F& right) const
	{
		return (r == right.r && g == right.g && b == right.b && a == right.a);
	}
	bool operator!=(const Color4F& right) const
	{
		return !(*this == right);
	}

	bool equals(const Color4F& other) const
	{
		return (*this == other);
	}
	SDL_Color getSDL_Color() const
	{
		SDL_Color color = {Uint8(r * 255), Uint8(g * 255), Uint8(b * 255)};
		return color;
	}
};
NS_SDL_END
#endif
