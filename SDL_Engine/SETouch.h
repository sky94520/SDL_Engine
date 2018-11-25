#ifndef __SDL_Touch_H__
#define __SDL_Touch_H__

#include "SDL.h"

#include "SEObject.h"
#include "SEPoint.h"
NS_SDL_BEGIN
typedef enum class TouchStatus
{
	TOUCH_NONE,
	TOUCH_DOWN,
	TOUCH_MOTION,
	TOUCH_UP
}TouchStatus;	
class Touch : public Object
{
private:
	SDL_FingerID _id;
	//当前状态，在为TOUCH_NONE时不可用
	TouchStatus _status;
	//是否已经记录开始坐标点
	bool _startPointCaptured;
	Point _startPoint;
	Point _point;
	Point _prevPoint;
public:
	Touch();
	virtual ~Touch();
	CREATE_FUNC(Touch);
	bool init();
	//获得当前触摸点
	Point getLocation() const;
	//获得上一个触摸点
	Point getPreviousLocation() const;
	//获得开始触摸点
	Point getStartLocation() const;
	//
	Point getDelta()const;
	//当前对象是否有用
	bool isAvailable()const;
	//获得触摸点的ID
	SDL_FingerID getID() const
	{
		return _id;
	}
	TouchStatus getStatus() const
	{
		return _status;
	}
	//改变触摸点
	void setTouchInfo(SDL_FingerID id,float x,float y,TouchStatus status)
	{
		_id = id;
		_prevPoint = _point;
		_point.x = x;
		_point.y = y;
		_status = status;

		if(!_startPointCaptured)
		{
			_startPointCaptured = true;
			_startPoint = _point;
			_prevPoint = _point;
		}
		//初始化状态
		if(status == TouchStatus::TOUCH_NONE)
		{
			_startPointCaptured = false;
			_startPoint = Point::ZERO;
			_prevPoint = Point::ZERO;
			_point = Point::ZERO;
		}
	}

};
NS_SDL_END
#endif
