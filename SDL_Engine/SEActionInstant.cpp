#include "SEActionInstant.h"
#include "SESprite.h"

NS_SDL_BEGIN
bool ActionInstant::isDone()const
{
	return true;
}
void ActionInstant::update(float time)
{
}
void ActionInstant::step(float dt)
{
	update(1);
}
//---------------------------FlipX------------------------------
FlipX*FlipX::create(bool x)
{
	auto flipX = new FlipX();
	if(flipX && flipX->initWithFlipX(x))
		flipX->autorelease();
	else
		SDL_SAFE_DELETE(flipX);
	return flipX;
}
bool FlipX::initWithFlipX(bool x)
{
	_flipX = x;
	return true;
}
void FlipX::update(float time)
{
	static_cast<Sprite*>(_target)->setFlipX(_flipX);
}
FlipX*FlipX::clone()const
{
	return FlipX::create(_flipX);
}
FlipX*FlipX::reverse()const
{
	return FlipX::create(!_flipX);
}
//---------------------------FlipY----------------------------------
FlipY*FlipY::create(bool y)
{
	auto flip = new FlipY();
	if(flip&&flip->initWithFlipY(y))
		flip->autorelease();
	else
		SDL_SAFE_DELETE(flip);
	return flip;
}
bool FlipY::initWithFlipY(bool y)
{
	_flipY = y;
	return true;
}
void FlipY::update(float time)
{
	static_cast<Sprite*>(_target)->setFlipY(_flipY);
}
FlipY*FlipY::clone()const
{
	return FlipY::create(_flipY);
}
FlipY*FlipY::reverse()const
{
	return FlipY::create(!_flipY);
}
//---------------------------------Show-----------------------------------
Show*Show::create()
{
	auto show = new Show();
	show->autorelease();
	return show;
}
void Show::update(float time)
{
	_target->setVisible(true);
}
Show*Show::clone()const
{
	return Show::create();
}
ActionInstant*Show::reverse()const
{
	return Hide::create();
}
//------------------------------Hide----------------------------
Hide*Hide::create()
{
	auto hide = new Hide();
	hide->autorelease();
	return hide;
}
void Hide::update(float time)
{
	_target->setVisible(false);
}
Hide*Hide::clone()const
{
	return Hide::create();
}
ActionInstant*Hide::reverse()const
{
	return Show::create();
}
//---------------------------------ToggleVisibility---------------------------------------
ToggleVisibility*ToggleVisibility::create()
{
	auto toggle = new ToggleVisibility();
	toggle->autorelease();
	return toggle;
}
void ToggleVisibility::update(float time)
{
	_target->setVisible(!_target->isVisible());
}
ToggleVisibility*ToggleVisibility::clone()const
{
	return ToggleVisibility::create();
}
ToggleVisibility*ToggleVisibility::reverse()const
{
	return ToggleVisibility::create();
}
//-----------------------------Place------------------------------------
Place*Place::create(const Point& pos)
{
	auto place = new Place();
	if(place && place->initWithPosition(pos))
		place->autorelease();
	else
		SDL_SAFE_DELETE(place);
	return place;
}
bool Place::initWithPosition(const Point& pos)
{
	_position = pos;
	return true;
}
void Place::update(float time)
{
	_target->setPosition(_position);
}
Place*Place::clone()const
{
	return Place::create(_position);
}
Place*Place::reverse()const
{
	//此函数没有反序
	return Place::clone();
}
//--------------------------------------Callback------------------------------------------
CallFunc*CallFunc::create(const std::function<void()>&callback)
{
	auto callfunc = new CallFunc();
	if(callfunc && callfunc->initWithCallback(callback))
		callfunc->autorelease();
	else
		SDL_SAFE_DELETE(callfunc);
	return callfunc;
}
bool CallFunc::initWithCallback(const std::function<void()>&callback)
{
	_callback = callback;
	return true;
}
void CallFunc::setCallback(const std::function<void()>& callback)
{
	_callback = callback;
}
void CallFunc::update(float time)
{
	_callback();
}
CallFunc*CallFunc::clone()const
{
	return CallFunc::create(_callback);
}
CallFunc*CallFunc::reverse()const
{
	return CallFunc::clone();
}
//-------------------------------CallFuncN---------------------------------
CallFuncN*CallFuncN::create(const std::function<void (Node*)>& callback)
{
	auto callfunc = new CallFuncN();
	if(callfunc && callfunc->initWithCallFuncN(callback))
		callfunc->autorelease();
	else
		SDL_SAFE_DELETE(callfunc);
	return callfunc;
}
bool CallFuncN::initWithCallFuncN(const std::function<void (Node*)>& callback)
{
	_callback = callback;
	return true;
}
void CallFuncN::setCallback(const std::function<void (Node*)>& callback)
{
	_callback = callback;
}
void CallFuncN::update(float time)
{
	_callback(_target);
}
CallFuncN* CallFuncN::clone()const
{
	return CallFuncN::create(_callback);
}
CallFuncN*CallFuncN::reverse()const
{
	return CallFuncN::clone();
}
//---------------------------RemoveSelf-----------------------------
bool RemoveSelf::init()
{
	return true;
}
void RemoveSelf::update(float time)
{
	//todo
	//_target->autorelease();
	_target->removeFromParent();
}
RemoveSelf*RemoveSelf::clone()const
{
	return RemoveSelf::create();
}
RemoveSelf*RemoveSelf::reverse()const
{
	return nullptr;
}
//---------------------------------------ColorMod---------------------------
ColorMod::ColorMod()
	:_r(255),_g(255),_b(255)
{
}
ColorMod*ColorMod::create(Uint8 r,Uint8 g,Uint8 b)
{
	auto action = new ColorMod();
	if(action && action->init(r,g,b))
		action->autorelease();
	else
		SDL_SAFE_DELETE(action);
	return action;
}
bool ColorMod::init(Uint8 r,Uint8 g,Uint8 b)
{
	if(ActionInstant::init())
	{
		_r = r;
		_g = g;
		_b = b;
		return true;
	}
	return false;
}
void ColorMod::update(float time)
{
	_target->setColorMod(_r,_g,_b);
}
ColorMod*ColorMod::clone()const
{
	return ColorMod::create(_r,_g,_b);
}
ColorMod*ColorMod::reverse()const
{
	return nullptr;
}
NS_SDL_END
