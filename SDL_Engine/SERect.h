#ifndef __SDL_Rect_H__
#define __SDL_Rect_H__
#include<string>
#include "SDL.h"
#include "SEPlatformMarcos.h"
#include "SEPoint.h"
#include "SESize.h"

NS_SDL_BEGIN
class Rect
{
public:
	Point origin;
	Size size;

	static const Rect ZERO;
public:
	Rect(float x = 0.f,float y = 0.f,float w = 0.f,float h = 0.f);
	Rect(const Rect& rect);
	Rect(const SDL_Rect& other);
	Rect(const Point& origin,const Size& size);
	Rect(float x,float y,const Size& size);
	Rect(const Point& origin,float w,float h);
	~Rect(){}
	//重载运算符
	Rect& operator=(const Rect& rect);
	void setRect(float x,float y,float w,float h);
	void setRect(const Rect& rect);

	float getMinX() const {return origin.x;}
	float getMidX() const {return origin.x + size.width/2;}
	float getMaxX() const {return origin.x + size.width;}

	float getMinY() const {return origin.y;}
	float getMidY() const {return origin.y + size.height/2;}
	float getMaxY() const {return origin.y + size.height;}
	//是否相等
	bool equals(const Rect& rect) const;
	//是否包含点
	bool containsPoint(const Point& point) const;
	//两rect是否相交或包含
	bool intersectRect(const Rect& rect) const;
	bool intersectRect(const Rect& rect, Rect& result);
	//矩形是否和线段交叉
	bool intersectLine(const Point& pos1,const Point& pos2) const;
	//返回SDL_Rect
	SDL_Rect getSDL_Rect()const;
};
NS_SDL_END
#endif
