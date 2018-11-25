#ifndef __SDL_ActionManager_H__
#define __SDL_ActionManager_H__

#include<algorithm>
#include<vector>
#include "SEObject.h"
#include "SEVector.h"
NS_SDL_BEGIN
class Action;
class Node;
struct _hashElement;

class ActionManager:public Object
{
private:
	//保存着全部的动画
	std::vector<_hashElement*> _tHashElements;
	_hashElement* _target;
	//当前播放的
	_hashElement* _currentTarget;
	//当前的结点是否回收
	bool _currentTargetSalvaged;
public:
	ActionManager();
	virtual ~ActionManager();

	//删除某一动画管理器
	void deleteHashElement(_hashElement*element);
	void deleteHashElementForTarget(Node*target);
	//暂停指定目标的所有动画
	void pauseTarget(Node*target);
	//恢复指定目标的动画
	void resumeTarget(Node*target);
	//移除某演员的全部动画
	void removeAllActionsFromTarget(Node*target);
	//移除全部动画
	void removeAllActions();
	//将指定的动画从动画管理器中删除
	void removeAction(Action*action);
	//以tag获取动画
	Action*getActionByTag(int tag,const Node*target)const;
	//以tag移除动画
	void removeActionByTag(int tag,const Node*target);
	//添加元素
	void addAction(Action*action,Node*node,bool paused);
	//最重要的函数
	void update(float dt);
};
NS_SDL_END
#endif
