#include "SETouch.h"

NS_SDL_BEGIN
Touch::Touch()
	:_id(0)
	,_status(TouchStatus::TOUCH_NONE)
	,_startPointCaptured(false)
{
}
Touch::~Touch()
{
}
bool Touch::init()
{
	return true;
}
Point Touch::getLocation() const
{
	return _point;
}
Point Touch::getPreviousLocation() const
{
	return _prevPoint;
}
Point Touch::getStartLocation() const
{
	return _startPoint;
}
Point Touch::getDelta()const
{
	return _point - _prevPoint;
}
bool Touch::isAvailable()const
{
	return _status != TouchStatus::TOUCH_NONE;
}
NS_SDL_END
