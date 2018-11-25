#include "SEPoint.h"
#include "SEMath.h"
NS_SDL_BEGIN
const Point Point::ZERO = Point(0.0f,0.0f);

Point::Point(const Point& point)
	:x(point.x),y(point.y)
{
}

Point::Point(const SDL_Point& point)
	:x((float)point.x),y((float)point.y)
{
}

float Point::getX() const
{
	return x;
}

void Point::setX(float x)
{
	this->x = x;
}

float Point::getY() const
{
	return y;
}

void Point::setY(float y)
{
	this->y = y;
}

void Point::set(float x,float y)
{
	this->x = x;
	this->y = y;
}

Point Point::rotateByAngle(const Point& pivot, float angle) const
{
	 return pivot + ( *this - pivot ).rotate( Point::forAngle(angle) );
}

bool Point::equals(const Point& point) const
{
	return fabs(x - point.x) < FLT_EPSILON
		&& fabs(y - point.y) < FLT_EPSILON;
}

// cross product of 2 vector. A->B X C->D
float crossProduct2Vector(const Point& A, const Point& B, const Point& C, const Point& D)
{
    return (D.y - C.y) * (B.x - A.x) - (D.x - C.x) * (B.y - A.y);
}

bool Point::isLineIntersect(const Point& A, const Point& B,
                            const Point& C, const Point& D,
                            float *S, float *T)
{
    // FAIL: Line undefined
    if ( (A.x==B.x && A.y==B.y) || (C.x==D.x && C.y==D.y) )
    {
        return false;
    }
    
    const float denom = crossProduct2Vector(A, B, C, D);
    
    if (denom == 0)
    {
        // Lines parallel or overlap
        return false;
    }
    
    if (S != nullptr) *S = crossProduct2Vector(C, D, C, A) / denom;
    if (T != nullptr) *T = crossProduct2Vector(A, B, C, A) / denom;
    
    return true;
}

SDL_Point Point::getSDL_Point() const
{
	SDL_Point point = { (int)x ,(int)y };
	return point;
}
NS_SDL_END
