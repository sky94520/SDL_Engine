#include "SEActionEase.h"
#include "SENode.h"

NS_SDL_BEGIN
ActionEase::ActionEase()
	:_pInnerAction(nullptr)
{
}

ActionEase::~ActionEase()
{
	SDL_SAFE_RELEASE_NULL(_pInnerAction);
}

bool ActionEase::initWithAction(ActionInterval*pAction)
{
	//有效性判断
	SDL_assert(pAction != nullptr);

	if (ActionInterval::initWithDuration(pAction->getDuration()))
	{
		_pInnerAction = pAction;
		_pInnerAction->retain();

		return true;
	}
	return false;
}

void ActionEase::startWithTarget(Node*target)
{
	ActionInterval::startWithTarget(target);
	_pInnerAction->startWithTarget(target);
}

void ActionEase::stop()
{
	_pInnerAction->stop();
	ActionInterval::stop();
}

void ActionEase::update(float time)
{
	_pInnerAction->update(time);
}
//-----------------------------EaseRateAction----------------------
EaseRateAction::EaseRateAction()
	:_rate(0.f)
{
}

EaseRateAction::~EaseRateAction()
{
}

EaseRateAction*EaseRateAction::create(ActionInterval*pAction,float rate)
{
	EaseRateAction*action = new EaseRateAction();

	if(action && action->initWithAction(pAction,rate))
		action->autorelease();
	else
		SDL_SAFE_DELETE(action);

	return action;
}

bool EaseRateAction::initWithAction(ActionInterval*pAction,float rate)
{
	if(ActionEase::initWithAction(pAction))
	{
		_rate = rate;

		return true;
	}
	return false;
}
EaseRateAction*EaseRateAction::reverse()const
{
	return EaseRateAction::create(_pInnerAction->reverse(),1.f/_rate);
}

EaseRateAction*EaseRateAction::clone()const
{
	return EaseRateAction::create(_pInnerAction->clone(),_rate);
}
//-------------------------------EaseIn--------------------
EaseIn*EaseIn::create(ActionInterval*pAction,float rate)
{
	EaseIn*action = new EaseIn();

	if (action && action->initWithAction(pAction,rate))
		action->autorelease();
	else
		SDL_SAFE_DELETE(action);

	return action;
}

void EaseIn::update(float time)
{
	_pInnerAction->update(powf(time,_rate));
}

EaseIn*EaseIn::reverse()const
{
	return EaseIn::create(_pInnerAction->reverse(),_rate);
}

EaseIn*EaseIn::clone()const
{
	return EaseIn::create(_pInnerAction->clone(),_rate);
}
//--------------------------EaseOut-----------------------
EaseOut*EaseOut::create(ActionInterval*pAction,float rate)
{
	EaseOut*action = new EaseOut();

	if (action && action->initWithAction(pAction,rate))
		action->autorelease();
	else
		SDL_SAFE_DELETE(action);

	return action;
}

void EaseOut::update(float time)
{
	_pInnerAction->update(powf(time,1.f/_rate));
}

EaseOut*EaseOut::reverse()const
{
	return EaseOut::create(_pInnerAction->reverse(),1.f/_rate);
}

EaseOut*EaseOut::clone()const
{
	return EaseOut::create(_pInnerAction->clone(),_rate);
}
//--------------------------EaseInOut----------------------
EaseInOut*EaseInOut::create(ActionInterval*pAction,float rate)
{
	EaseInOut*action = new EaseInOut();

	if (action && action->initWithAction(pAction,rate))
		action->autorelease();
	else
		SDL_SAFE_DELETE(action);

	return action;
}

void EaseInOut::update(float time)
{
	time *= 2;  

    if (time < 1)  
    {  
		_pInnerAction->update(0.5f * powf(time, _rate));  
    }  
    else  
    {
        _pInnerAction->update(1.0f - 0.5f * powf(2-time, _rate));  
    } 
}

EaseInOut*EaseInOut::reverse()const
{
	return EaseInOut::create(_pInnerAction->reverse(),_rate);
}

EaseInOut*EaseInOut::clone()const
{
	return EaseInOut::create(_pInnerAction->clone(),_rate);
}
//-----------------------------EaseExponentialIn--------------------
EaseExponentialIn*EaseExponentialIn::create(ActionInterval*pAction)
{
	EaseExponentialIn*action = new EaseExponentialIn();

	if (action && action->initWithAction(pAction))
		action->autorelease();
	else
		SDL_SAFE_DELETE(action);

	return action;
}

void EaseExponentialIn::update(float time)
{
	//此曲线没有系数。是固定曲线 
	_pInnerAction->update(time == 0 ? 0 : powf(2, 10 * (time/1 - 1)) - 1 * 0.001f);  
}

ActionInterval*EaseExponentialIn::reverse()const
{
	return EaseExponentialOut::create(_pInnerAction->reverse());
}

EaseExponentialIn*EaseExponentialIn::clone()const
{
	return EaseExponentialIn::create(_pInnerAction->clone());
}
//-----------------------EaseExponentialOut--------------------------------
EaseExponentialOut*EaseExponentialOut::create(ActionInterval*pAction)
{
	EaseExponentialOut*action = new EaseExponentialOut();

	if(action && action->initWithAction(pAction))
		action->autorelease();
	else
		SDL_SAFE_DELETE(action);

	return action;
}
	
void EaseExponentialOut::update(float time)
{
	_pInnerAction->update(time == 1 ? 1 : (-powf(2, -10 * time / 1) + 1));  
}

ActionInterval*EaseExponentialOut::reverse()const
{
	return EaseExponentialIn::create(_pInnerAction->reverse());
}

EaseExponentialOut*EaseExponentialOut::clone()const
{
	return EaseExponentialOut::create(_pInnerAction->clone());
}
NS_SDL_END
