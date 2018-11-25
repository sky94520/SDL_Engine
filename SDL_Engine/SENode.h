#ifndef __SDL_Node_H__
#define __SDL_Node_H__
#include<vector>
#include<string>
#include<assert.h>
#include<algorithm>

#include "SEObject.h"
#include "SEPoint.h"
#include "SESize.h"
#include "SERect.h"
#include "SEMath.h"
#include "SEColor.h"
#include "SEScheduler.h"

NS_SDL_BEGIN
/*作为扩充类，以后扩展*/
class Renderer;
class EventDispatcher;
class ActionManager;
class Action;

class Node : public Object
{
protected:
	//子孩子
	std::vector<Node*> _children;
	//the node's name,you can find it by the name
	std::string _name;
	//the tag，如果没赋值，值为-1
	int _tag;
	//the parent
	Node*_parent;
	//is visible
	bool _visible;
	/*local z orderlocalZOrder the node will sort by it，越大越先被绘制*/
	int _localZOrder;
	//global z order
	float _globalZOrder;
	//judge the child is sorted
	bool _reorderChildDirty;
	//check the node whether run
	bool _running;
	//position
	/*这里说明，position是相对坐标*/
	Point _position;
	//content size
	Size _contentSize;
	
	//rotation scope [0,360],if the angle not between the scope,t%360
	float _rotation;
	//scale
	float _scaleX;
	float _scaleY;
	//anchor point scope[0.0,1.0]
	Point _anchorPoint;
	bool _flipX;
	bool _flipY;
	//不透明度
	Uint8 _realOpacity;
	Uint8 _displayedOpacity;
	bool _cascadeOpacity;

	Color3B _colorMod;
	//级联
	//设置缩放级联
	bool _cascadeScale;
	//用户数据,需要自己手动删除
	void* _userdata;
	Object* _userObject;
	//事件监听器
	EventDispatcher* _eventDispatcher;
	ActionManager* _actionManager;
	Scheduler* _scheduler;
protected:
	Node();
	//友元
	friend class Director;
public:
	virtual ~Node();
	CREATE_FUNC(Node);
	bool init();
	virtual void update(float dt);
	//设置父母
	Node* getParent() const { return _parent; }
	void setParent(Node* parent);
	//add child
	void addChild(Node* child);
	//child localZOrder
	void addChild(Node* child,int localZOrder);
	void addChild(Node* child,int localZOrder,int tag);
	void removeChild(Node* child,bool cleanup = false);
	//从父亲处删除自己
	void removeFromParent();
	//不建议在程序中使用
	void removeAllChildren();

	void cleanup();
	//在每次构造函数后调用
	virtual void onEnter();
	//在每次退出前调用
	virtual void onExit();
	//
	void resume();
	void pause();
	//暂停此节点下的定时器和动作
	void pauseSchedulerAndActions();
	//恢复此节点的定时器和动作
	void resumeSchedlerAndActions();
	virtual void visit();
	//留给子类进行重写
	virtual void draw();

	bool isVisible();
	void setVisible(bool isVisible);
	//位置 
	void setPositionX(float x);
	void setPositionY(float y);
	void setPosition(const Point& pos);
	void setPosition(float x,float y);
	//获得相对坐标
	Point getPosition() const;
	float getPositionX()const;
	float getPositionY()const;
	//获得绝对坐标
	Point getWorldPosition()const;
	//获取相对于该节点的位置，不受锚点影响
	Point convertToNodeSpace(const Point& worldPoint)const;
	//受锚点影响
	Point convertToNodeSpaceAR(const Point& worldPoint)const;
	Point convertToWorldSpace(const Point& nodePoint)const;
	Point convertToWorldSpaceAR(const Point& nodePoint);

	Size getContentSize() const;
	void setContentSize(const Size& size);
	//获取左上角为原点的一个矩形(缩放级联)
	Rect getBoundingBox() const;
	float getRotation();
	//角度
	void setRotation(float rotation);
	//缩放
	float getScaleX();
	void setScaleX(float scaleX);
	float getScaleY();
	void setScaleY(float scaleY);
	void setScale(float scaleX,float scaleY);
	Point getScale()const;
	Point getWorldScale()const;

	Point getAnchorPoint()const;
	void setAnchorPoint(const Point& anchorPoint);

	void setName(std::string name);
	std::string getName() const;
	void setTag(int tag);
	int getTag();
	//find the node by name or tag
	Node* getChildByName(std::string name);
	Node* getChildByTag(int tag);
	//T
	template<typename T>
	T getChildByName(std::string name) { return dynamic_cast<T>(getChildByName(name)); }
	template<typename T>
	T getChildByTag(int tag) { return dynamic_cast<T>(getChildByTag(tag)); }
	//获取所有的孩子
	const std::vector<Node*>& getChildren() { return _children; }
	//排序
	void sortAllChildren();
	static bool nodeComparisonLess(Node* n1, Node* n2);
	//set & get localZOrder
	int getLocalZOrder() const;
	void setLocalZOrder(int localZOrder);

	float getGlobalZOrder() const; 
	void setGlobalZOrder(float globalZOrder);
	//return whether or not the node is 'running'
	virtual bool isRunning() const{return _running;}
	//翻转
	bool isFlipX();
	bool isFlipY();
	void setFlipX(bool flipX);
	void setFlipY(bool flipY);
	//设置不透明度
	void setOpacity(Uint8 opacity);
	Uint8 getOpacity() const;
	Uint8 getDisplayedOpacity() const;

	void updateCascadeOpacity();
	void disableCascadeOpacity();
	void updateDisplayedOpacity(Uint8 parentOpacity);

	void setCascadeOpacityEnabled(bool enabled);
	bool isCascadeOpacityEnabled() const;
	//色彩模式
	void setColorMod(const Color3B&color);
	void setColorMod(Uint8 r,Uint8 g,Uint8 b);
	Color3B getColorMod()const;
	//级联操作
	void setCascadeScaleEnabled(bool enabled);
	bool isCascadeScaleEnabled()const;
	//设置node所带的用户数据
	void setUserData(void*userdata);
	void* getUserData()const;

	void setUserObject(Object*sender);
	Object* getUserObject()const;
	//------------------------------Action相关---------------------------------------
	void runAction(Action*action);
	void stopAllActions();
	Action*getActionByTag(int tag);
	void stopActionByTag(int tag);
	void stopAction(Action*action);
	//---------------------------------计时器相关----------------------------
	//添加update到计时器
	void scheduleUpdate();
	void unscheduleUpdate();

	void schedule(const ccScheduleCallback&callback,const std::string& key,float interval=0.f,unsigned int repeat=SDL_REPEAT_FOREVER,float delay=0.f);
	void schedule(SEL_SCHEDULE selector,float interval,unsigned int repeat,float delay=0.f);
	
	void unschedule(const std::string& key);
	void unschedule(SEL_SCHEDULE selector);
	//只执行一次的schedule,在delay秒之后执行
	void scheduleOnce(const ccScheduleCallback& callback,const std::string&key,float delay=0.f);
	void scheduleOnce(SEL_SCHEDULE selector,float delay = 0.f);
	void unscheduleAllCallbacks();
	//------------------------------------删除所有监听器----------------------------
	void removeAllListeners();
};
NS_SDL_END
#endif
