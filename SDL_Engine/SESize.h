#ifndef __SDL_Size_H__
#define __SDL_Size_H__

#include<cmath>
#include<string>
#include "SDL.h"
#include "SEPlatformMarcos.h"

NS_SDL_BEGIN
class Size
{
public:
	float width;
	float height;
	
	static const Size ZERO;
public:
	Size(float w = 0.f, float h = 0.f);
	~Size(){}
	//复制构造函数
	Size(const Size& s);
	Size& operator=(const Size& other);

	Size operator+(Size& size);
	Size operator-(Size& size);
	Size operator*(float a) const;
	Size operator/(float a) const;
	Size operator/(const Size& other) const;
	//重载==运算符
	bool operator==(const Size& other) const;
	bool operator!=(const Size& other) const;
	void setSize(float width,float height);
	bool equals(const Size& s) const;
};
NS_SDL_END
#endif
