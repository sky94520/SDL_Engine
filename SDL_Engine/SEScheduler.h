#ifndef __SDL_Scheduler_H__
#define __SDL_Scheduler_H__
#include<list>
#include<map>
#include<string>
#include<functional>

#include "SEObject.h"
NS_SDL_BEGIN
class Node;

typedef std::function<void (float)> ccScheduleCallback;

class SchedulerEntity:public Object
{
protected:
	float _interval;
	//总执行次数
	unsigned int _repeat;
	//以秒为单位
	float _delay;
	//当前执行次数
	unsigned int _executeTimes;
	bool _paused;
	//当前经过的秒数
	float _elapsed;
	//优先级
	int _priority;
	//当调用这个时自动从Scheduler中删除自己
	bool _bDeleteSelf;
	//定时器保证能每过interval或执行
	//Timer _timer;
public:
	friend class Scheduler;
public:
	SchedulerEntity(float interval,unsigned int repeat,float delay,bool paused,int priority=0)
		:_interval(interval),_repeat(repeat),_delay(delay)
		,_executeTimes(0),_paused(paused),_elapsed(0.f)
		,_priority(priority),_bDeleteSelf(false)
	{
	}
	~SchedulerEntity()
	{
		//printf("schedule resleased!\n");
	}
	virtual void execute(float dt)
	{
	}
};
class SchedulerEntityCallback:public SchedulerEntity
{
private:
	Node*_target;
	ccScheduleCallback _callback;
	std::string _key;
public:
	friend class Scheduler;
public:
	SchedulerEntityCallback(const ccScheduleCallback&callback,Node*target,const std::string&key
		,float interval,unsigned int repeat,float delay,bool paused,int priority=0)
		:SchedulerEntity(interval,repeat,delay,paused,priority)
		,_target(target)
		,_callback(callback)
		,_key(key)
		{
		}
	virtual void execute(float dt);
};
class SchedulerEntitySelector : public SchedulerEntity
{
private:
	Object*_target;
	SEL_SCHEDULE _selector;
public:
	SchedulerEntitySelector(SEL_SCHEDULE selector,Object*target,float interval,
		unsigned int repeat,float delay,bool paused,int priority=0)
		:SchedulerEntity(interval,repeat,delay,paused,priority)
		,_target(target)
		,_selector(selector)
		{
		}
public:
	friend class Scheduler;
	virtual void execute(float dt);
};
//---------------------------------定时器-----------------------------
class Scheduler:public Object
{
private:
	std::map<Object*,std::list<SchedulerEntity*>*> _updateSchedule;
public:
	Scheduler();
	virtual ~Scheduler();

	void update(float dt);
	//注册
	void schedule(const ccScheduleCallback&callback,Object*target,const std::string& key,float interval,unsigned int repeat,float delay,bool paused);
	void schedule(SEL_SCHEDULE selector,Object*target,float interval,unsigned int repeat,float delay,bool paused);
	//删除注册
	void unschedule(Object*target,const std::string& key);
	void unschedule(Object*target,SEL_SCHEDULE selector);
	
	void resumeScheduleForTarget(Object*node);
	void pauseScheduleForTarget(Object*node);

	void unschedulerForTarget(Object*node);
};

NS_SDL_END
#endif
