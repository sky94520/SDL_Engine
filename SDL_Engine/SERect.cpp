#include "SERect.h"
NS_SDL_BEGIN
const Rect Rect::ZERO = Rect(0.f,0.f,0.f,0.f);

Rect::Rect(float x,float y,float w,float h)
	:origin(x,y),size(w,h)
{
}

Rect::Rect(const Rect& rect)
	:origin(rect.origin),size(rect.size)
{
}

Rect::Rect(const SDL_Rect& other)
	:origin(other.x,other.y)
	,size(other.w,other.h)
{
}

Rect::Rect(const Point& origin,const Size& size)
	:origin(origin),size(size)
{
}

Rect::Rect(float x,float y,const Size& size)
	:origin(x,y),size(size)
{
}

Rect::Rect(const Point& origin,float w,float h)
	:origin(origin),size(w,h)
{
}

Rect& Rect::operator=(const Rect& rect)
{
	setRect(rect.origin.x,rect.origin.y,rect.size.width,rect.size.height);
	return *this;
}

void Rect::setRect(float x,float y,float w,float h)
{
	origin.x = x;
	origin.y = y;
	size.width = w;
	size.height = h;
}

void Rect::setRect(const Rect& rect)
{
	setRect(rect.origin.x,rect.origin.y,rect.size.width,rect.size.height);
}

bool Rect::equals(const Rect& rect) const
{
	return origin.equals(rect.origin)
		&&size.equals(rect.size);
}

bool Rect::containsPoint(const Point&point) const
{
	return (point.x > getMinX() && point.x < getMaxX())
		&&(point.y > getMinY() && point.y < getMaxY());
}

bool Rect::intersectRect(const Rect& rect) const
{
	return !(     getMaxX() < rect.getMinX() ||
             rect.getMaxX() <      getMinX() ||
                  getMaxY() < rect.getMinY() ||
             rect.getMaxY() <      getMinY());
}

bool Rect::intersectRect(const Rect& rect, Rect& result)
{
	SDL_Rect a = this->getSDL_Rect();
	SDL_Rect b = rect.getSDL_Rect();
	SDL_Rect r = {};
	SDL_bool ret = SDL_IntersectRect(&a, &b, &r);
	//set the result
	result = Rect(ret);

	return ret == SDL_TRUE;
}

bool Rect::intersectLine(const Point& pos1,const Point& pos2) const
{
	SDL_Rect rect;

	rect.x = (int)origin.x;
	rect.y = (int)origin.y;
	rect.w = (int)size.width;
	rect.h = (int)size.height;

	int x1 = (int)pos1.x;
	int y1 = (int)pos1.y;

	int x2 = (int)pos2.x;
	int y2 = (int)pos2.y;

	return SDL_IntersectRectAndLine(&rect
		,&x1,&y1,&x2,&y2) == SDL_TRUE;
}

SDL_Rect Rect::getSDL_Rect()const
{
	SDL_Rect rect = {(int)origin.x,(int)origin.y,(int)size.width,(int)size.height};
	return rect;
}
NS_SDL_END
