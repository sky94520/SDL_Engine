#ifndef __SDL_ActionEase_H__
#define __SDL_ActionEase_H__
#include "SEActionInterval.h"

NS_SDL_BEGIN
class Node;

class ActionEase:public ActionInterval
{
protected:
	ActionInterval*_pInnerAction;
public:
	ActionEase();
	virtual ~ActionEase();
	bool initWithAction(ActionInterval*pAction);

	virtual void startWithTarget(Node*target);
	virtual void stop(void);
	virtual void update(float time);
};
//-------------------------EaseRateAction----------------------
class EaseRateAction:public ActionEase
{
protected:
	float _rate;
public:
	EaseRateAction();
	~EaseRateAction();
	
	static EaseRateAction*create(ActionInterval*pAction,float rate); 
	bool initWithAction(ActionInterval*pAction,float rate);

	inline void setRate(float rate){_rate = rate;}
	inline float getRate()const{return _rate;}

	virtual EaseRateAction*reverse()const;
	virtual EaseRateAction*clone()const;
};
//---------------------------EaseIn由快到慢-----------------------
class EaseIn : public EaseRateAction
{
public:
	static EaseIn*create(ActionInterval*pAction,float rate);
	
	virtual void update(float time);
	virtual EaseIn*reverse()const;
	virtual EaseIn*clone()const;
};
//--------------------------EaseOut由慢到快-----------------------
class EaseOut : public EaseRateAction
{
public:
	static EaseOut*create(ActionInterval*pAction,float rate);
	
	virtual void update(float time);
	virtual EaseOut*reverse()const;
	virtual EaseOut*clone()const;
};
//----------------------EaseInOut--------------------------------
class EaseInOut:public EaseRateAction
{
public:
	static EaseInOut*create(ActionInterval*pAction,float rate);
	
	virtual void update(float time);
	virtual EaseInOut*reverse()const;
	virtual EaseInOut*clone()const;	
};
//----------------------EaseExponentialIn----------------------------------
class EaseExponentialIn : public ActionEase
{
public:
	static EaseExponentialIn*create(ActionInterval*pAction);
	
	virtual void update(float time);
	virtual ActionInterval*reverse()const;
	virtual EaseExponentialIn*clone()const;
};
//-----------------------EaseExponentialOut--------------------------------
class EaseExponentialOut : public ActionEase
{
public:
	static EaseExponentialOut*create(ActionInterval*pAction);
	
	virtual void update(float time);
	virtual ActionInterval*reverse()const;
	virtual EaseExponentialOut*clone()const;
};
NS_SDL_END
#endif
