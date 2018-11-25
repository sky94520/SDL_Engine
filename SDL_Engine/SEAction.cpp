#include "SEAction.h"
#include "SENode.h"
#include "SEActionInterval.h"

NS_SDL_BEGIN
Action::Action()
	:_originalTarget(nullptr)
	,_target(nullptr)
	,_tag(-1)
{
}
Action::~Action()
{
}
int Action::getTag()const
{
	return _tag;
}
void Action::setTag(int tag)
{
	_tag = tag;
}
bool Action::isDone()const
{
	return true;
}
void Action::startWithTarget(Node*pTarget)
{
	_target = _originalTarget = pTarget;
}
void Action::stop()
{
	_target = nullptr;
}
void Action::step(float dt)
{
}
void Action::update(float time)
{
}
//------------------------------------Speed-----------------------------
Speed::Speed()
	:_speed(0.f),_pInnerAction(nullptr)
{
}
Speed::~Speed()
{
	SDL_SAFE_RELEASE(_pInnerAction);
}
Speed*Speed::create(ActionInterval*pAction,float speed)
{
	auto action = new Speed();
	if(action && action->initWithAction(pAction,speed))
		action->autorelease();
	else
		SDL_SAFE_DELETE(action);
	return action;
}
bool Speed::initWithAction(ActionInterval*action,float speed)
{
	if(action)
	{
		action->retain();
		_pInnerAction = action;

		_speed = speed;
		return true;
	}
	return false;
}
bool Speed::isDone()const
{
	return _pInnerAction->isDone();
}
void Speed::startWithTarget(Node*pTarget)
{
	Action::startWithTarget(pTarget);
	_pInnerAction->startWithTarget(pTarget);
}
void Speed::stop()
{
	_pInnerAction->stop();
	Action::stop();
}
void Speed::step(float dt)
{
	_pInnerAction->step(dt*_speed);
}
Action*Speed::clone()const
{
	return Speed::create(_pInnerAction,_speed);
}
NS_SDL_END
