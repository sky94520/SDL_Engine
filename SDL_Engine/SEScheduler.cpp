#include "SEScheduler.h"
#include "SENode.h"
NS_SDL_BEGIN
Scheduler::Scheduler()
{
}
Scheduler::~Scheduler()
{
	for(auto iterMap=_updateSchedule.begin();iterMap != _updateSchedule.end();)
	{
		auto list = iterMap->second;
		for(auto itList=list->begin();itList!=list->end();)
		{
			SchedulerEntity*schedule = *itList;
			if(schedule)
			{
				itList = list->erase(itList);
				schedule->release();
			}
			else
				++itList;
		}
		iterMap = _updateSchedule.erase(iterMap);
		delete list;
	}
}
void Scheduler::update(float dt)
{
	for(auto mapIter:_updateSchedule)
	{
		auto list = mapIter.second;

		for(auto listIter=list->begin();listIter!=list->end();)
		{
			SchedulerEntity*schedule = *listIter;
			if(schedule->_bDeleteSelf)
			{
				listIter = list->erase(listIter);
				schedule->release();

				continue;
			}
			if(!schedule->_paused)
			{
				schedule->execute(dt);
			}
			++listIter;
		}
	}
}
void Scheduler::schedule(const ccScheduleCallback&callback,Object*target,const std::string& key,float interval,unsigned int repeat,float delay,bool paused)
{
	SchedulerEntity*entity = new SchedulerEntityCallback(callback,(Node*)target,key,interval,repeat,delay,paused);
	//寻找是否有相应的Node
	auto iter = _updateSchedule.find(target);
	std::list<SchedulerEntity*>* list = nullptr;
	if(iter == _updateSchedule.end())
	{
		list = new std::list<SchedulerEntity*>();
		//插入到map中
		_updateSchedule.insert(std::make_pair(target,list));
	}
	else
	{
		list = iter->second;
	}
	list->push_back(entity);
}
void Scheduler::schedule(SEL_SCHEDULE selector,Object*target,float interval,unsigned int repeat,float delay,bool paused)
{
	SchedulerEntity*entity = new SchedulerEntitySelector(selector,target,interval,repeat,delay,paused);
	//寻找是否有相应的Node
	auto iter = _updateSchedule.find(target);
	std::list<SchedulerEntity*>* list = nullptr;
	if(iter == _updateSchedule.end())
	{
		list = new std::list<SchedulerEntity*>();
		//插入到map中
		_updateSchedule.insert(std::make_pair(target,list));
	}
	else
	{
		list = iter->second;
	}
	list->push_back(entity);
}
void Scheduler::unschedule(Object*target,const std::string& key)
{
	auto mapIter = _updateSchedule.find(target);
	if(mapIter != _updateSchedule.end())
	{
		auto list = mapIter->second;
		for(auto listIter=list->begin();listIter != list->end();++listIter)
		{
			SchedulerEntityCallback* schedule = static_cast<SchedulerEntityCallback*>(*listIter);
			if(strcmp(schedule->_key.c_str(),key.c_str())==0)
			{
				/*list->erase(listIter);
				schedule->release();*/
				schedule->_bDeleteSelf = true;
				return;
			}

		}
	}
}
void Scheduler::unschedule(Object*target,SEL_SCHEDULE selector)
{
	auto mapIter = _updateSchedule.find(target);
	if(mapIter != _updateSchedule.end())
	{
		auto list = mapIter->second;
		for(auto listIter=list->begin();listIter != list->end();++listIter)
		{
			SchedulerEntitySelector* schedule = static_cast<SchedulerEntitySelector*>(*listIter);
			if(schedule->_selector == selector)
			{
				schedule->_bDeleteSelf = true;
				return;
			}

		}
	}
}
void Scheduler::resumeScheduleForTarget(Object*node)
{
	auto mapIter = _updateSchedule.find(node);
	if(mapIter != _updateSchedule.end())
	{
		std::list<SchedulerEntity*>* list = mapIter->second;
		for(auto iterList=list->begin();iterList!=list->end();++iterList)
		{
			SchedulerEntity*schedule = *iterList;
			schedule->_paused = false;
		}
	}
}
void Scheduler::pauseScheduleForTarget(Object*node)
{
	auto mapIter = _updateSchedule.find(node);
	if(mapIter != _updateSchedule.end())
	{
		std::list<SchedulerEntity*>* list = mapIter->second;
		for(auto iterList=list->begin();iterList!=list->end();++iterList)
		{
			SchedulerEntity*schedule = *iterList;
			schedule->_paused = true;
		}
	}
}
void Scheduler::unschedulerForTarget(Object*node)
{
	auto iterMap = _updateSchedule.find(node);
	if(iterMap != _updateSchedule.end())
	{
		std::list<SchedulerEntity*>* list = iterMap->second;
		//循环删除list ScheduleEntity
		for(auto listIter = list->begin();listIter!=list->end();++listIter)
		{
			(*listIter)->release();
		}
		delete list;
		_updateSchedule.erase(iterMap);
	}
}
//-------------------------------SchedulerEntityCallback-------------------------------------
void SchedulerEntityCallback::execute(float dt)
{
	if(_paused)
		return;
	//如果到达重复次数 退出 
	if(_executeTimes >= _repeat)
	{
		_bDeleteSelf = true;
		return;
	}
	_elapsed += dt;
	if(_elapsed > _delay)
	{
		if(_elapsed - _delay >= _interval*(_executeTimes+1))
		{
			//每帧都运行
			if(_interval != 0.f)
				this->_callback(_elapsed - _delay - _interval*_executeTimes);
			else
				this->_callback(dt);

			_executeTimes++;
		}
	}
}
//---------------------------------SchedulerEntitySelector--------------------------------
void SchedulerEntitySelector::execute(float dt)
{
	if(_paused)
		return;
	//如果到达重复次数 退出 
	if(_executeTimes >= _repeat)
	{
		_bDeleteSelf = true;
		return;
	}
	_elapsed += dt;
	if(_elapsed > _delay)
	{
		if(_elapsed - _delay >= _interval*(_executeTimes+1))
		{
			if(_interval != 0.f)
				(_target->*_selector)(_elapsed - _delay - _interval*_executeTimes);
			else
				(_target->*_selector)(dt);
			_executeTimes++;
		}
	}
}
NS_SDL_END
