#ifndef __SDL_Animation_H__
#define __SDL_Animation_H__
#include<vector>
#include "SEObject.h"
NS_SDL_BEGIN
class SpriteFrame;
class AnimationFrame;

class Animation:public Object
{
private:
	float _duration;
	//存放精灵帧
	std::vector<AnimationFrame*> _frames;
	//总循环次数
	unsigned int _loops;
	//是否保存原来的图片 默认为false
	bool _restoreOriginalFrame;
public:
	Animation();
	virtual ~Animation();
	static Animation*createWithSpriteFrames(const std::vector<SpriteFrame*>& spriteFrames,float delay=0,unsigned int loops=1);
	static Animation*createWithAnimationFrames(const std::vector<AnimationFrame*>&animationFrames,unsigned int loops = 1);

	bool initWithSpriteFrames(const std::vector<SpriteFrame*>&spriteFrames,float delay,unsigned int loops);
	bool initWithAnimationFrames(const std::vector<AnimationFrame*>&animationFrames,unsigned int loops = 1);

	void setLoops(unsigned int loops){_loops = loops;}
	unsigned int getLoops()const{return _loops;}

	//获得总共的持续时间
	float getDuration()const{return _duration;}
	int getTotalFrames()const{return _frames.size();}

	bool getRestoreOriginalFrame()const{return _restoreOriginalFrame;}
	void setRestoreOriginalFrame(bool restoreOriginalFrame){_restoreOriginalFrame = restoreOriginalFrame;}

	const std::vector<AnimationFrame*>&getFrames()const
	{
		return _frames;
	}
	void setFrames(const std::vector<AnimationFrame*> frames)
	{
		_frames.clear();
		_frames = frames;
	}
};
NS_SDL_END
#endif
