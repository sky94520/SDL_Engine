#include "SESize.h"
#include "SEMath.h"
NS_SDL_BEGIN
const Size Size::ZERO = Size(0.f,0.f);

Size::Size(float w,float h)
	:width(w),height(h)
{
}

Size::Size(const Size& s)
	:width(s.width),height(s.height)
{
}

Size& Size::operator=(const Size& other)
{
	setSize(other.width,other.height);
	return *this;
}

Size Size::operator+(Size& size)
{
	return Size(width + size.width, height + size.height);
}

Size Size::operator-(Size& size)
{
	return Size(width - size.width, height - size.height);
}

Size Size::operator*(float a) const
{
	return Size(width * a, height * a);
}

Size Size::operator/(float a) const
{
	SDLASSERT(a != 0,"CCSize division by 0.");
	return Size(width / a, height / a);
}

Size Size::operator/(const Size& other) const
{
	return Size(width / other.width, height / other.height);
}

bool Size::operator==(const Size& other) const
{
	return width == other.width
		&& height == other.height;
}

bool Size::operator!=(const Size& other) const
{
	return width != other.width 
		|| height != other.height;
}

void Size::setSize(float width,float height)
{
	this->width = width;
	this->height = height;
}

bool Size::equals(const Size& s) const
{
	return fabs(width-s.width) < FLT_EPSILON
		&& fabs(height - s.height) < FLT_EPSILON;
}

NS_SDL_END
