#include "SEActionManager.h"
#include "SENode.h"
#include "SEAction.h"

NS_SDL_BEGIN
typedef struct _hashElement
{
	//保存的动画
	Vector<Action*> actions;
	//演员
	Node* target;
	//动画在动画集中的索引
	unsigned int actionIndex;
	//当前执行的动画
	Action* currentAction;
	//当前动画是否被收回
	bool currentActionSalvaged;
	//是否停止播放
	bool paused;
public:
	_hashElement():target(nullptr),actionIndex(-1),currentAction(nullptr)
		,currentActionSalvaged(false),paused(true)
	{}
}tHashElement;
ActionManager::ActionManager():_target(nullptr),
	_currentTarget(nullptr),_currentTargetSalvaged(false)
{
}
ActionManager::~ActionManager()
{
	removeAllActions();
	//_tHashElements.clear();
}

void ActionManager::deleteHashElement(tHashElement*element)
{
	//查找
	auto iter =std::find(_tHashElements.begin(),_tHashElements.end(),element);
	if(iter != _tHashElements.end())
	{
		_tHashElements.erase(iter);
		//删除动画集
		element->actions.clear();

		element->target->release();
		//释放自己
		delete element;
	}
}
void ActionManager::deleteHashElementForTarget(Node*target)
{
	auto iter = std::find_if(_tHashElements.begin(),_tHashElements.end(),[target](tHashElement*ele)
	{
		return target == ele->target;
	});
	if(iter != _tHashElements.end())
	{
		tHashElement*element = *iter;
		deleteHashElement(element);
	}
}
void ActionManager::pauseTarget(Node*target)
{
	auto iter = std::find_if(_tHashElements.begin(),_tHashElements.end(),[target](tHashElement*ele)
	{
		return ele->target == target;
	});
	if(iter != _tHashElements.end())
	{
		auto element = *iter;
		element->paused = true;
	}
}
void ActionManager::resumeTarget(Node*target)
{
	auto iter = std::find_if(_tHashElements.begin(),_tHashElements.end(),[target](tHashElement*ele)
	{
		return ele->target == target;
	});
	if(iter != _tHashElements.end())
	{
		auto element = *iter;
		element->paused = false;
	}
}
void ActionManager::removeAllActionsFromTarget(Node*target)
{
	//找到这一演员所在的Element
	auto iter = std::find_if(_tHashElements.begin(),_tHashElements.end(),[target](tHashElement*ele)
	{
		return ele->target == target;
	});
	//存在该ele
	if(iter != _tHashElements.end())
	{
		tHashElement*element = *iter;
		//判断是否是当前运行的动画
		if(element == _currentTarget)
		{
			_currentTargetSalvaged = true;
		}else
		{
			deleteHashElement(element);
		}
	}
}
void ActionManager::removeAllActions()
{
	for(unsigned int i=0;i<_tHashElements.size();i++)
	{
		auto element = _tHashElements.at(i);
		removeAllActionsFromTarget(element->target);
	}
}
void ActionManager::removeAction(Action*action)
{
	//有效性判断
	if(action==nullptr)
		return;
	//获得该action对应的 演员
	Node*originalTarget = action->getOriginalTarget();
	//找到该演员对应的动画管理器
	tHashElement*element = nullptr;
	auto iter = std::find_if(_tHashElements.begin(),_tHashElements.end(),[originalTarget](tHashElement*e)
	{
		return e->target == originalTarget;
	});
	//找到
	if(iter != _tHashElements.end())
	{
		element = *iter;
		//停止该动画
		action->stop();
		element->actions.eraseObject(action);
	}
}
Action*ActionManager::getActionByTag(int tag,const Node*target)const
{
	if(target)
	{
		//找到target相应的tHashElement
		auto iter = std::find_if(_tHashElements.begin(),_tHashElements.end(),[&](tHashElement*element)
		{
			return element->target == target;
		});
		//
		if(iter != std::end(_tHashElements))
		{
			//这个必须这样使用
			auto&actions = (*iter)->actions;
			auto actionIter = std::find_if(actions.begin(),actions.end(),[&](Action*action)
			{
				return action->getTag() == tag;
			});
			if(actionIter != (*iter)->actions.end())
			{
				return *actionIter;
			}
		}
	}
	return nullptr;
}
void ActionManager::removeActionByTag(int tag,const Node*target)
{
	if(target)
	{
		//找到target相应的tHashElement
		auto iter = std::find_if(_tHashElements.begin(),_tHashElements.end(),[&](tHashElement*element)
		{
			return element->target == target;
		});
		//
		if(iter != std::end(_tHashElements))
		{
			auto& actions = (*iter)->actions;
			auto actionIter = std::find_if(actions.begin(),actions.end(),[&](Action*action)
			{
				return action->getTag() == tag;
			});
			if(actionIter != actions.end())
			{
				auto action = *actionIter;
				action->stop();
				actions.erase(actionIter);
			}
		}
	}
}
void ActionManager::addAction(Action*action,Node*target,bool paused)
{
	SDLASSERT(action!=NULL,"");
	SDLASSERT(target!=NULL,"");
	tHashElement*elem = nullptr;
	//找该target对应的element
	auto iter = std::find_if(_tHashElements.begin(),_tHashElements.end(),[target](tHashElement*ele)->bool
	{
		return target == ele->target;
	});
	//没找到，则新建
	if(iter == _tHashElements.end())
	{
		elem = new tHashElement();
		elem->paused = paused;
		
		elem->target = target;
		_tHashElements.push_back(elem);

		target->retain();
	}
	else
		elem = *iter;
	//为element的Vector动画分配空间
	elem->actions.push_back(action);
	//初始化
	action->startWithTarget(target);
}
void ActionManager::update(float dt)
{
	//遍历整个表
	for(unsigned int i=0;i<_tHashElements.size();i++)
	{
		_currentTarget = _tHashElements.at(i);
		_currentTargetSalvaged = false;
		//当前节点正在运行
		if(!_currentTarget->paused)
		{
			//遍历当前项的所有动画集
			for(_currentTarget->actionIndex=0;_currentTarget->actionIndex < _currentTarget->actions.size();++_currentTarget->actionIndex)
			{
				//获得遍历时的动画
				_currentTarget->currentAction = _currentTarget->actions.at(_currentTarget->actionIndex);
				//如果此动画为空，跳过此次循环
				if(_currentTarget->currentAction == nullptr)
					continue;
				//回收标记设置为false
				_currentTarget->currentActionSalvaged = false;
				//更新当前的动画播放
				_currentTarget->currentAction->step(dt);
				//如果当前项的回收标记为true，进行释放处理
				if(_currentTarget->currentActionSalvaged)
				{
					_currentTarget->currentAction->release();
				}//如果动画已经做完，则停止动画
				else if(_currentTarget->currentAction->isDone())
				{
					_currentTarget->currentAction->stop();
					//删除动画
					auto otherAction = _currentTarget->currentAction;
					_currentTarget->currentAction = nullptr;
					//移除该动画
					removeAction(otherAction);
				}
				_currentTarget->currentAction = nullptr;
			}//end for
		}//end for
		//如果当前表处于回收状态并且动画集为空，删除
		if(_currentTargetSalvaged /*&& _currentTarget->currentActionSalvaged*/)
		{
			//移除该element 
			deleteHashElement(_currentTarget);
		}
	}
	//将当前变量置空
	_currentTarget = nullptr;
}
NS_SDL_END
