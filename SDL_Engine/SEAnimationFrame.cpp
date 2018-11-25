#include "SEAnimationFrame.h"
#include "SESpriteFrame.h"
NS_SDL_BEGIN
AnimationFrame::AnimationFrame()	
	:_spriteFrame(nullptr)
	,_delayUnits(0.f)
{
}
AnimationFrame::~AnimationFrame()
{
	SDL_SAFE_RELEASE(_spriteFrame);
}
AnimationFrame*AnimationFrame::create(SpriteFrame*spriteFrame,float delayUnit)
{
	auto animationFrame = new AnimationFrame();
	if(animationFrame && animationFrame->init(spriteFrame,delayUnit))
		animationFrame->autorelease();
	else
		SDL_SAFE_DELETE(animationFrame);
	return animationFrame;
}
bool AnimationFrame::init(SpriteFrame*spriteFrame,float delayUnit)
{
	_delayUnits = delayUnit;
	setSpriteFrame(spriteFrame);
	return true;
}
void AnimationFrame::setSpriteFrame(SpriteFrame*spriteFrame)
{
	SDL_SAFE_RETAIN(spriteFrame);
	SDL_SAFE_RELEASE(_spriteFrame);
	_spriteFrame = spriteFrame;
}
SpriteFrame*AnimationFrame::getSpriteFrame()const
{
	return _spriteFrame;
}
void AnimationFrame::setDelayUnits(float delayUnits)
{
	_delayUnits = delayUnits;
}
float AnimationFrame::getDelayUnits()const
{
	return _delayUnits;
}
NS_SDL_END
