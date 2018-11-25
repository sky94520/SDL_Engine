#ifndef __SDL_Menu_H__
#define __SDL_Menu_H__
#include<vector>
#include<stdarg.h>
#include "SELayer.h"
NS_SDL_BEGIN
class MenuItem;
class Menu : public Layer
{
public:
	static const int DEFAULT_PADDING;
public:
	enum class State
	{
		WAITING,//正在等待Touch事件
		TRACLING_TOUCH//已经接收到事件
	};
protected:
	//是否接收事件
	bool _enabled;
	//当前点击项目
	std::vector<MenuItem*> _selectedItems;
	//当前状态
	State _state;
public:
	Menu();
	virtual ~Menu();
	CREATE_FUNC(Menu);
	static Menu*create(MenuItem*item,...);

	static Menu*createWithArray(const std::vector<MenuItem*>& menuItem);

	bool init();
	bool initWithArray(const std::vector<MenuItem*>& menuItem);

	void setEnabled(bool enabled);
	bool isEnabled() const;
	//垂直排序
	void alignItemsVertically();
	void alignItemsVerticallyWithPadding(float padding);

	virtual bool onTouchBegan(Touch*touch,SDL_Event*event);
	virtual void onTouchMoved(Touch*touch,SDL_Event*event);
	virtual void onTouchEnded(Touch*touch,SDL_Event*event);
	virtual void onTouchCancelled(Touch*touch,SDL_Event*event);
private:
	//通过touch查找受点击的item
	std::vector<MenuItem*> getItemForTouch(Touch*touch);
};
NS_SDL_END
#endif
