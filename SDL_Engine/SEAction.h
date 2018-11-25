#ifndef __SDL_Action_H__
#define __SDL_Action_H__
#include "SEObject.h"

NS_SDL_BEGIN
class Node;
class ActionInterval;

class Action:public Object,public Clonable
{
protected:
	Node* _originalTarget;
	Node* _target;
	//用户标记，-1表示无效
	int _tag;
public:
	Action();
	virtual ~Action();

	//获得演员
	inline Node*getTarget(){return _target;}
	//设置演员
	inline void setTarget(Node*pTarget){_target = pTarget;}
	//最初演员
	inline Node*getOriginalTarget(){return _originalTarget;}
	inline void setOriginalTarget(Node*pTarget){_originalTarget = pTarget;}
	//标记
	int getTag()const;
	void setTag(int tag);

	//动画是否完成
	virtual bool isDone()const;
	//设置将要演示本动画的节点，在动作开始前调用
	virtual void startWithTarget(Node*pTarget);
	//停止动画并使演员置空
	virtual void stop();
	//每一帧传入时间流逝值
	virtual void step(float dt);
	//传入进度参数
	virtual void update(float time);

	virtual Action*clone()const=0;
};
//----------------------------Speed--------------------
class Speed:public Action
{
private:
	float _speed;//current speed
	ActionInterval*_pInnerAction;
public:
	Speed();
	~Speed();
	static Speed*create(ActionInterval*action,float speed);
	bool initWithAction(ActionInterval*action,float speed);

	float getSpeed()const{return _speed;}
	void setSpeed(float speed){_speed = speed;}

	virtual bool isDone()const;
	virtual void startWithTarget(Node*pTarget);
	virtual void stop();
	virtual void step(float dt);
	virtual Action*clone()const;
};
NS_SDL_END
#endif
