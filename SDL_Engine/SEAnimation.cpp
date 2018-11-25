#include "SEAnimation.h"
#include "SESpriteFrame.h"
#include "SEAnimationFrame.h"
NS_SDL_BEGIN
Animation::Animation():
	_duration(0.f)
	,_loops(1)
	,_restoreOriginalFrame(false)
{
}
Animation::~Animation()
{
	for(unsigned int i=0;i<_frames.size();i++)
	{
		_frames.at(i)->release();
	}
	_frames.clear();
}
Animation*Animation::createWithSpriteFrames(const std::vector<SpriteFrame*>&spriteFrames,float delay,unsigned int loops)
{
	auto animation = new Animation();
	if(animation && animation->initWithSpriteFrames(spriteFrames,delay,loops))
		animation->autorelease();
	else
		SDL_SAFE_DELETE(animation);
	return animation;
}
Animation*Animation::createWithAnimationFrames(const std::vector<AnimationFrame*>&animationFrames,unsigned int loops)
{
	auto animation = new Animation();
	if(animation && animation->initWithAnimationFrames(animationFrames,loops))
		animation->autorelease();
	else
		SDL_SAFE_DELETE(animation);
	return animation;
}
bool Animation::initWithSpriteFrames(const std::vector<SpriteFrame*>&spriteFrames,float delay,unsigned int loops)
{
	_loops = loops;

	for(unsigned int i=0;i<spriteFrames.size();i++)
	{
		//保持引用
		auto frame = AnimationFrame::create(spriteFrames.at(i),delay);
		frame->retain();
		_frames.push_back(frame);
		//持续时间
		_duration += delay;
	}
	return true;
}
bool Animation::initWithAnimationFrames(const std::vector<AnimationFrame*>&animationFrames,unsigned int loops)
{
	_loops = loops;
	_frames = animationFrames;
	//获取总时间
	for(auto frame:animationFrames)
	{
		frame->retain();
		_duration += frame->getDelayUnits();
	}
	return true;
}
NS_SDL_END
