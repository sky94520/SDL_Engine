#ifndef __SDL_EventDispatcher_H__
#define __SDL_EventDispatcher_H__
#include <map>
#include <unordered_map>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>

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
	std::map<std::string,std::vector<EventListener*>* > _listenerMap;
	std::unordered_map<Node*, std::vector<EventListener*>*> _nodeListenersMap;
	//待添加列表
	std::vector<EventListener*> _toAddedListeners;
	//待删除列表
	std::vector<EventListener*> _toRemovedListeners;

	//是否正在分发事件
	bool _inDispatch;
	//是否需要对listener进行排序
	bool _reorderListenersDirty;
public:
	EventDispatcher();
	~EventDispatcher();
public:
	void addEventListener(EventListener*listener,Node*node);
	/**
	 * 强制添加监听器
	 * @param listener 要添加的监听器
	 * @param node 监听器对应的节点
	 */
	void forceAddEventListener(EventListener* listener, Node* node);

	void addEventCustomListener(const std::string&eventName,const std::function<void(EventCustom*)>&onEventCustom,Node*node);
	
	void removeEventListener(EventListener*listener);
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

	/* 更新对应的类型的监听器
	 * @param listenerID 事件对应的类型
	 */
	void updateListeners(const std::string& listenerID);

	bool isInDispatch() const { return _inDispatch; };
	void setInDispatch(bool var) { _inDispatch = var; }
private:
	void dispatchEventToTouchOneByOne(const std::vector<Touch*>& touches,SDL_Event*event);
	void dispatchEventToTouchAllAtOnce(const std::vector<Touch*>& touches,SDL_Event*event);

	std::vector<EventListener*>* getListeners(const std::string&listenerID);
	void sortEventListeners();
	static bool sorted(EventListener*e1,EventListener*e2);
	void cleanToRemovedListeners();
	//仅仅负责维护_nodeListeners
	void associateNodeAndEventListener(Node* node, EventListener* listener);
	void dissociateNodeAndEventListener(Node* node, EventListener* listener);

	friend class EventListener;
};
NS_SDL_END
#endif
