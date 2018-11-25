#ifndef __SDL_FiniteTimeAction_H__
#define __SDL_FiniteTimeAction_H__
#include "SEAction.h"
NS_SDL_BEGIN
class FiniteTimeAction:public Action
{
protected:
	float _duration;//动画持续值
public:
	FiniteTimeAction():_duration(0.f){}
	virtual ~FiniteTimeAction(){}
	//获得动画的时长
	inline float getDuration()const{return _duration;}
	inline void setDuration(float duration){_duration = duration;}
	//获得反向播放动画
	virtual FiniteTimeAction*reverse()const=0;
	//复制对象
	virtual FiniteTimeAction*clone()const=0;
};
NS_SDL_END
#endif
