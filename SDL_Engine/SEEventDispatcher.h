#ifndef __SDL_EventDispatcher_H__
#define __SDL_EventDispatcher_H__
#include<map>
#include<vector>
#include<string>
#include<functional>
#include<algorithm>

#include "SDL.h"
#include "SEObject.h"
#include "SEEventCustom.h"

NS_SDL_BEGIN
class Node;
class Touch;
class EventListener;
class EventListenerCustom;

class EventDispatcher:public Object
{
private:
	std::map<std::string,std::vector<EventListener*>* > _listeners;
	//是否需要对listener进行排序
	bool _reorderListenersDirty;
public:
	EventDispatcher();
	~EventDispatcher();
public:
	void addEventListener(EventListener*listener,Node*node);
	void addEventCustomListener(const std::string&eventName,const std::function<void(EventCustom*)>&onEventCustom,Node*node);
	
	bool removeEventListener(EventListener*listener);
	//删除
	void removeEventListenerForTarget(Node*node,bool recursive=false);
	//启动node对应的listeners
	void resumeEventListenersForTarget(Node*node,bool recursive = false);
	void pauseEventListenersForTarget(Node*node,bool recursive = false);
	//发送按键事件
	void dispatchKeyboardEvent(SDL_Event& event);
	//发送按键状态事件
	void dispatchKeyboardStateEvent(SDL_Event& event);
	//发送鼠标事件
	void dispatchMouseEvent(SDL_Event& event);
	//发送触碰事件
	void dispatchTouchEvent(const std::vector<Touch*>& touches,SDL_Event& event);
	//发送文本输入事件
	void dispatchTextInputEvent(SDL_Event& event);
	//发送用户自定义事件
	void dispatchCustomEvent(const std::string&eventName,void*userData = nullptr);
	void dispatchCustomEvent(EventCustom*eventCustom);
private:
	void dispatchEventToTouchOneByOne(const std::vector<Touch*>& touches,SDL_Event*event);
	void dispatchEventToTouchAllAtOnce(const std::vector<Touch*>& touches,SDL_Event*event);

	std::vector<EventListener*>* getListeners(const std::string&listenerID);
	void sortEventListeners();
	static bool sorted(EventListener*e1,EventListener*e2);
	friend class EventListener;
};
NS_SDL_END
#endif
