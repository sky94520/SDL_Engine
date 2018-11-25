#ifndef __SDL_NotificationCenter_H__
#define __SDL_NotificationCenter_H__
#include<map>
#include<algorithm>
#include<vector>
#include<string>

#include "SEVector.h"
#include "SEObject.h"
NS_SDL_BEGIN
class NotificationObserver : public Object
{
public:
	Object*target;
	SEL_CallFuncO selector;
	std::string name;
public:
	NotificationObserver():target(nullptr),selector(nullptr)
	{}
	NotificationObserver(Object*target,SEL_CallFuncO selector,const std::string&name):target(target),selector(selector),name(name)
	{}

};
class NotificationCenter:public Object
{
private:
	static NotificationCenter*_pInstance;
	Vector<NotificationObserver*> _observers;
	NotificationCenter();
	~NotificationCenter();
public:
	static NotificationCenter*getInstance()
	{
		if(_pInstance == NULL)
			_pInstance = new NotificationCenter();
		return _pInstance;
	}
	static void purge();
	//添加观察者
	void addObserver(Object*target,SEL_CallFuncO selector,const std::string&msgName);
	//移除观察者
	void removeObserver(Object*target,const std::string&msgName);
	//发送消息
	void postNotification(const std::string&msgName,Object*data);
};
NS_SDL_END
#endif
