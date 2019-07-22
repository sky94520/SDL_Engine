#ifndef __SDL_ActionInterval_H__
#define __SDL_ActionInterval_H__

#include<vector>
#include<algorithm>

#include "SEFiniteTimeAction.h"
#include "SEPoint.h"
#include "SEColor.h"
#include "SEEventCustom.h"
#include "SEAnimationFrame.h"

NS_SDL_BEGIN
class Animation;
class SpriteFrame;

class ActionInterval: public FiniteTimeAction
{
protected:
	//流逝时间
	float _elapsed;
	//是否第一次计算时间
	bool _bFirstTick;
public:
	ActionInterval();
	virtual ~ActionInterval();
	inline float getElapsed()const{return _elapsed;}
	//初始化，设置一个固定时长的动画
	bool initWithDuration(float d);
	//动画是否播放结束
	virtual bool isDone()const;
	virtual void step(float dt);
	
	virtual void startWithTarget(Node*target);
	virtual ActionInterval*clone()const;
	//创建一个反向的动画
	virtual ActionInterval*reverse()const;
public:
	//振幅 
	void setAmplitudeRate(float amp);
	float getAmplitudeRate()const;
};
//-----------------------------MoveTo--------------------------------------
class MoveTo:public ActionInterval
{
protected:
	//目标位置
	Point _endPosition;
	//起始位置
	Point _startPosition;
	//移动的偏移
	Point _deltaPosition;
public:
	MoveTo(){}
	virtual ~MoveTo(){}
	static MoveTo* create(float duration,const Point&position);
	bool initWithDuration(float d,const Point&position);
	virtual void startWithTarget(Node*target);
	virtual void update(float time);
	//反向动画
	virtual MoveTo* reverse()const;
	virtual MoveTo*clone()const;
};
//--------------------------------MoveBy------------------------------------
class MoveBy:public MoveTo
{
public:
	MoveBy();
	virtual ~MoveBy();
	static MoveBy*create(float duration,const Point&relativePosition);
	bool initWithDuration(float duration,const Point&relativePosition);
	void startWithTarget(Node*target);
	//override
	virtual MoveBy*clone()const;
	virtual MoveBy*reverse()const;
};
//-------------------------------Animate------------------------------------
class Animate:public ActionInterval
{
private:
	//演员原来的图片
	SpriteFrame*_originalFrame;
	//下一帧
	int _nextFrame;
	//保存着Animation实例
	Animation*_animation;
	//当前的执行次数
	unsigned int _executedLoops;
	//每帧的持续时间
	std::vector<float>* _splitTimes;
	EventCustom * _frameDisplayedEvent;
	AnimationFrame::DisplayedEventInfo _frameDisplayedEventInfo;
public:
	Animate();
	virtual ~Animate();
	static Animate*create(Animation*animation);

	bool initWithAnimation(Animation*animation);
	unsigned int getNextFrameIndex()const{return _nextFrame;}
	//获取内部animation
	Animation*getAnimation(){return _animation;}
	//override
	virtual void startWithTarget(Node*target);
	virtual void update(float time);
	virtual void stop();
	//获得反向动画
	virtual Animate*clone()const;
	virtual Animate*reverse()const;
};
//------------------------------ScaleTo-------------------------------------
class ScaleTo:public ActionInterval
{
protected:
	float _startScaleX;
	float _startScaleY;

	float _endScaleX;
	float _endScaleY;

	float _deltaX;
	float _deltaY;
public:
	ScaleTo():_startScaleX(1.f),_startScaleY(1.f),_endScaleX(1.f)
		,_endScaleY(1.f),_deltaX(0.f),_deltaY(0.f){}
	virtual ~ScaleTo(){}
	static ScaleTo*create(float duration,float scale);
	static ScaleTo*create(float duration,float scaleX,float scaleY);
	bool initWithScaleTo(float duration,float scaleX,float scaleY);
	//override
	virtual void startWithTarget(Node*target);

	virtual void update(float);
	virtual ScaleTo*clone()const;
	virtual ScaleTo*reverse()const;
};
//------------------------------ScaleBy----------------------------------------
class ScaleBy:public ScaleTo
{
public:
	static ScaleBy*create(float duration,float scale);
	static ScaleBy*create(float duration,float scaleX,float scaleY);
	//重载
	bool initWithDuration(float duration,float scaleX,float scaleY);
	void startWithTarget(Node*target);
	//override
	virtual ScaleBy*clone()const;
	virtual ScaleBy*reverse()const;
};
//-------------------------RotateTo--------------------------------
class RotateTo:public ActionInterval
{
protected:
	float _startRotation;
	float _endRotation;
	float _deltaRotation;
public:
	RotateTo();
	virtual ~RotateTo(){}
	static RotateTo*create(float duration,float rotate);
	bool initWithDuration(float duration,float rotate);
	void startWithTarget(Node*target);
	virtual void update(float time);
	virtual RotateTo*clone()const;
	virtual RotateTo*reverse()const;
};
//------------------------RotateBy-----------------------------------
class RotateBy:public RotateTo
{
public:
	static RotateBy*create(float duration,float rotation);
	bool initWithDuration(float duration,float rotation);
	void startWithTarget(Node*target);
	//override
	virtual RotateBy*clone()const;
	virtual RotateBy*reverse()const;
};
//------------------------Blink------------------------------------
class Blink:public ActionInterval
{
private:
	//闪烁次数
	int _times;
	//初始状态
	bool _originalState;
public:
	Blink():_times(0),_originalState(true){}
	virtual ~Blink();
	static Blink*create(float duration,int times);
	bool initWithDuration(float duration,int times);
	void startWithTarget(Node*target);
	//override
	void update(float time);
	Blink*clone()const;
	Blink*reverse()const;
};
//-------------------------FadeTo-----------------------------------
class FadeTo:public ActionInterval
{
protected:
	Uint8 _fromOpacity;
	Uint8 _toOpacity;
public:
	FadeTo():_fromOpacity(0),_toOpacity(0){}
	virtual ~FadeTo(){}
	static FadeTo*create(float duration,Uint8 opacity);
	bool initWithDuration(float duration,Uint8 opacity);
	//override
	virtual void startWithTarget(Node*target);
	virtual void update(float time);
	virtual FadeTo*clone()const;
	virtual FadeTo*reverse()const;
};
//--------------------------FadeIn淡入------------------------------------
class FadeIn:public FadeTo
{
public:
	static FadeIn*create(float duration);
	virtual void startWithTarget(Node*target);
	virtual FadeIn*clone()const;
	virtual FadeTo*reverse()const;
};
//-----------------------------FadeOut------------------------------------
class FadeOut:public FadeTo
{
public:
	static FadeOut*create(float duration);
	virtual void startWithTarget(Node*target);
	virtual FadeOut*clone()const;
	virtual FadeTo*reverse()const;
};
//------------------------------TintTo-----------------------------------------
class TintTo:public ActionInterval
{
protected:
	Color3B _fromColor;
	Color3B _toColor;
public:
	TintTo():_fromColor(255,255,255),_toColor(255,255,255){}
	virtual ~TintTo(){}
	static TintTo*create(float duration,Uint8 r,Uint8 g,Uint8 b);
	bool initWithDuration(float duration,Uint8 r,Uint8 g,Uint8 b);
	
	virtual void startWithTarget(Node*target);
	virtual void update(float time);
	virtual TintTo*clone()const;
	virtual TintTo*reverse()const;
};
class TintBy:public ActionInterval
{
private:
	short _fromR;
	short _fromG;
	short _fromB;
	short _deltaR;
	short _deltaG;
	short _deltaB;
public:
	TintBy():_fromR(255),_fromG(255),_fromB(255)
		,_deltaR(0),_deltaG(0),_deltaB(0){}
	static TintBy*create(float duration,short r,short g,short b);
	bool initWithDuration(float duration,short r,short g,short b);
	
	
	virtual void startWithTarget(Node*target);
	virtual void update(float time);
	virtual TintBy*clone()const;
	virtual TintBy*reverse()const;
};
//--------------------------------DelayTime--------------------------------
class DelayTime : public ActionInterval
{
public:
	static DelayTime*create(float duration);
	virtual void update(float time);
	virtual DelayTime*clone()const;
	virtual DelayTime*reverse()const;
};
//-----------------------------Sequence------------------------------------
class Sequence:public ActionInterval
{
private:
	FiniteTimeAction*_actions[2];
	//两个动画的分界进度
	float _split;
	//上一帧播放的动画数组中的索引，
	int _last;
public:
	Sequence();
	virtual ~Sequence();
	static FiniteTimeAction*create(FiniteTimeAction*action1,...);
	static FiniteTimeAction*create(const std::vector<FiniteTimeAction*>&actions);
	static Sequence*createWithTwoActions(FiniteTimeAction*action1,FiniteTimeAction*action2);

	bool initWithTwoActions(FiniteTimeAction*action1,FiniteTimeAction*action2);
	//override
	virtual void startWithTarget(Node*targer);
	virtual void stop();
	virtual void update(float time);
	virtual Sequence*clone()const;
	virtual Sequence*reverse()const;
};
//----------------------------Spawn----------------------------------------------
class Spawn:public ActionInterval
{
private:
	//将要组合的第一个动画
	FiniteTimeAction*_actionOne;
	FiniteTimeAction*_actionTwo;
public:
	Spawn():_actionOne(nullptr),_actionTwo(nullptr){}
	virtual ~Spawn();

	static FiniteTimeAction*create(FiniteTimeAction*action1,...);
	static Spawn*createWithTwoActions(FiniteTimeAction*action1,FiniteTimeAction*action2);
	bool initWithTwoActions(FiniteTimeAction*action1,FiniteTimeAction*action2);
	
	virtual void startWithTarget(Node*target);
	virtual void stop();
	virtual void update(float time);
	virtual Spawn*clone()const;
	virtual Spawn*reverse()const;
};
//---------------------------JumpBy---------------------------------
class JumpBy : public ActionInterval
{
protected:
	//开始位置
	Point _startPosition;
	//偏移位置
	Point _deltaPosition;
	//跳跃高度
	float _height;
	//跳跃次数
	int _jumps;
public:
	JumpBy():_startPosition(Point::ZERO)
		,_deltaPosition(Point::ZERO),_height(0),_jumps(0){}
	virtual ~JumpBy(){}
	static JumpBy*create(float duration,const Point&position,float height,int jumps);
	bool initWithDuration(float duration,const Point&position,float height,int jumps);
	
	virtual void startWithTarget(Node*target);
	virtual void update(float time);
	virtual JumpBy*clone()const;
	virtual JumpBy*reverse()const;
};
//------------------------------JumpTo--------------------------------------
class JumpTo : public JumpBy
{
public:
	static JumpTo*create(float duration,const Point&position,float height,int jumps);

	virtual void startWithTarget(Node*target);
	virtual JumpTo*clone()const;
	virtual JumpTo*reverse()const;
};
//---------------------------------Repeat------------------------------------
class Repeat : public ActionInterval
{
private:
	//总次数
	unsigned int _totalTimes;
	//当前次数
	unsigned int _curTimes;
	//下一次要切换的时间
	float _nextDt;
	//是否是瞬时动画
	bool _bActionInstant;
	FiniteTimeAction*_pInnerAction;
public:
	Repeat();
	virtual ~Repeat();
	static Repeat*create(FiniteTimeAction*action,unsigned int times);
	bool initWithAction(FiniteTimeAction*action,unsigned int times);
	
	virtual bool isDone()const;
	virtual void startWithTarget(Node*target);
	virtual void stop();
	virtual void update(float dt);
	virtual Repeat*clone()const;
	virtual Repeat*reverse()const;
};
//-----------------------------RepeatForever--------------------------------
class RepeatForever:public ActionInterval
{
private:
	ActionInterval*_pInnerAction;
public:
	RepeatForever():_pInnerAction(nullptr){}
	virtual ~RepeatForever()
	{
		SDL_SAFE_RELEASE(_pInnerAction);
	}
	static RepeatForever*create(ActionInterval*action);
	bool initWithAction(ActionInterval*action);
	void startWithTarget(Node*target);
	virtual bool isDone()const
	{
		return false;
	}
	virtual void step(float dt);
	virtual RepeatForever*clone()const;
	virtual RepeatForever*reverse()const;
};
//----------------------------BezierBy-----------------------------
typedef struct _ccBezierConfig
{
	Point endPosition;
	Point controlPoint_1;
	Point controlPoint_2;
}ccBezierConfig;

class BezierBy:public ActionInterval
{
protected:
	ccBezierConfig _config;
	Point _startPosition;
	Point _previousPosition;
public:
	BezierBy(){}
	~BezierBy(){}
	static BezierBy*create(float t,const ccBezierConfig&config);
	bool initWithDuration(float d,const ccBezierConfig&config);
	//override
	virtual void startWithTarget(Node*target);
	virtual void update(float dt);
	virtual BezierBy*clone()const;
	virtual BezierBy*reverse()const;
};
//---------------------------------BezierTo------------------------------
class BezierTo:public BezierBy
{
protected:
	ccBezierConfig _toConfig;
public:
	static BezierTo*create(float t,const ccBezierConfig&config);
	bool initWithDuration(float d,const ccBezierConfig&config);
	//override
	virtual void startWithTarget(Node*target);
	virtual BezierTo*clone()const;
	virtual BezierTo*reverse()const;
};
NS_SDL_END
#endif
