#ifndef __EventCustom_H__
#define __EventCustom_H__
#include<string>
#include<functional>

class EventCustom
{
private:
	std::string _eventName;
	void*_userData;
public:
	EventCustom(const std::string&eventName)
		:_eventName(eventName),_userData(nullptr)
	{}
	~EventCustom(){}

	std::string getEventName()const{ return _eventName;}
	void setEventName(const std::string&eventName){_eventName = eventName;}
	void*getUserData()const{return _userData;}
	void setUserData(void*userData){_userData = userData;}
};
#endif