#ifndef __SDL_ActionInstant_H__
#define __SDL_ActionInstant_H__

#include "SEFiniteTimeAction.h"
#include "SEPoint.h"
#include "SEColor.h"
NS_SDL_BEGIN
class ActionInstant:public FiniteTimeAction
{
public:
	virtual bool isDone()const;
	virtual void update(float time);
	virtual void step(float dt);

	virtual ActionInstant*clone()const=0;
	virtual ActionInstant*reverse()const=0;
};
//---------------------------FlipX--------------------------------
class FlipX:public ActionInstant
{
private:
	bool  _flipX;
public:
	FlipX():_flipX(false){}
	virtual ~FlipX(){}

	static FlipX*create(bool x);
	bool initWithFlipX(bool x);
	//override
	virtual void update(float time);
	virtual FlipX*clone()const;
	virtual FlipX*reverse()const;
};
//-------------------------FlipY---------------------------
class FlipY:public ActionInstant
{
private:
	bool _flipY;
public:
	FlipY():_flipY(false){}
	virtual ~FlipY(){}
	static FlipY*create(bool y);
	bool initWithFlipY(bool y);
	//override
	virtual void update(float time);
	FlipY*clone()const;
	FlipY*reverse()const;
};
//---------------------------------Show--------------------------------
class Show:public ActionInstant
{
public:
	Show(){}
	virtual ~Show(){}
	static Show*create();
	//override
	virtual void update(float time);
	virtual Show*clone()const;
	virtual ActionInstant*reverse()const;
};
//------------------------------------Hide----------------------------------
class Hide:public ActionInstant
{
public:
	Hide(){}
	virtual ~Hide(){}
	static Hide*create();
	//override
	virtual void update(float time);
	virtual Hide*clone()const;
	virtual ActionInstant*reverse()const;
};
//----------------------------------ToggleVisibility-------------------------------------
class ToggleVisibility:public ActionInstant
{
public:
	ToggleVisibility(){}
	virtual ~ToggleVisibility(){}

	static ToggleVisibility*create();
	//override
	virtual void update(float time);
	virtual ToggleVisibility*clone()const;
	virtual ToggleVisibility*reverse()const;
};
//---------------------------Place-------------------------------------
class Place:public ActionInstant
{
private:
	Point _position;
public:
	Place():_position(Point::ZERO){}
	virtual ~Place(){}
	static Place*create(const Point& pos);
	bool initWithPosition(const Point& pos);
	//override
	virtual void update(float time);
	virtual Place*clone()const;
	virtual Place*reverse()const;
};
//---------------------------CallFunc---------------------------------
class CallFunc:public ActionInstant
{
private:
	std::function<void()> _callback;
public:
	CallFunc():_callback(nullptr){}
	virtual ~CallFunc(){}
	static CallFunc*create(const std::function<void()>& function);
	bool initWithCallback(const std::function<void()>& function);
	void setCallback(const std::function<void()>& callback);
	//override
	virtual void update(float time);
	virtual CallFunc*clone()const;
	virtual CallFunc*reverse()const;
};
//---------------------------------CallFunN(CallFunc Node)------------------------------------
class CallFuncN:public ActionInstant
{
private:
	std::function<void (Node*)> _callback;
public:
	CallFuncN():_callback(nullptr){}
	virtual ~CallFuncN(){}

	static CallFuncN*create(const std::function<void (Node*)>&callback);
	bool initWithCallFuncN(const std::function<void (Node*)>& callback);
	void setCallback(const std::function<void (Node*)>& callback);
	//override
	virtual void update(float time);
	virtual CallFuncN*clone()const;
	virtual CallFuncN*reverse()const;
};
class RemoveSelf:public ActionInstant
{
public:
	RemoveSelf(){}
	virtual ~RemoveSelf(){}
	CREATE_FUNC(RemoveSelf);
	bool init();

	virtual void update(float time);
	virtual RemoveSelf*clone()const;
	virtual RemoveSelf*reverse()const;
};
//------------------------------ColorMod-------------------------------
class ColorMod:public ActionInstant
{
private:
	Uint8 _r;
	Uint8 _g;
	Uint8 _b;
public:
	ColorMod();
	virtual ~ColorMod(){}
	static ColorMod*create(Uint8 r,Uint8 g,Uint8 b);
	bool init(Uint8 r,Uint8 g,Uint8 b);

	virtual void update(float time);
	virtual ColorMod*clone()const;
	virtual ColorMod*reverse()const;
};
NS_SDL_END
#endif
