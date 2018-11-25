#ifndef __SDL_Widget_H__
#define __SDL_Widget_H__
#include<functional>

#include "../SENode.h"
#include "../SEPoint.h"
#include "../SERect.h"
NS_SDL_BEGIN
class Object;
class Touch;
class EventListenerTouchOneByOne;
namespace ui
{
/*从哪加载资源*/
enum class TextureResType
{
	LOCAL,//直接加载图片
	XML,//从xml文件加载
};
enum class TouchEventType
{
	BEGAN,
	MOVED,
	ENDED,
	CANCELLED,
};
typedef std::function<void (Object*)> ccWidgetClickCallback;
typedef std::function<void (Object*,TouchEventType)> ccWidgetTouchCallback;

class Widget:public Node
{
protected:
	TextureResType _resType;
	ccWidgetClickCallback _clickCallback;//回调函数
	ccWidgetTouchCallback _touchCallback;
	bool _bTouchEnabled;//是否接受触碰
	bool _swallowTouches;//是否屏蔽触屏
	bool _shouldCallback;//是否应该响应事件
	EventListenerTouchOneByOne*_touchListener;
public:
	Widget();
	~Widget();

	void addClickEventListener(const ccWidgetClickCallback&callback);
	void addTouchEventListener(const ccWidgetTouchCallback&callback);

	virtual void setTouchEnabled(bool isEnable);
	bool getTouchEnabled()const{return _bTouchEnabled;}

	void setSwallowTouches(bool swallow);
	bool isSwallowTouches()const;

	virtual Node* getVirtualRenderer();
	virtual bool onTouchBegan(Touch*touch,SDL_Event*event);
	virtual void onTouchMoved(Touch*touch,SDL_Event*event);
	virtual void onTouchEnded(Touch*touch,SDL_Event*event);
	virtual void onTouchCancelled(Touch*touch,SDL_Event*event);
protected:
	//hook
	virtual void touchBeganHook(Touch*touch,SDL_Event*event){}
	virtual void touchMovedInHook(Touch*touch,SDL_Event*event){}
	virtual void touchMovedOutHook(Touch*touch,SDL_Event*event){}
	virtual void touchEndedHook(Touch*touch,SDL_Event*event){}
	virtual void touchCancelledHook(Touch*touch,SDL_Event*event){}
	//进行碰撞测试 默认是与widget包围盒碰撞
	virtual bool hitTest(Touch*touch);
};
//end namespace ui
}
NS_SDL_END
#endif
