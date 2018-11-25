#ifndef __SDL_UIWidgetSystem_H__
#define __SDL_UIWidgetSystem_H__

#include "../SEObject.h"

NS_SDL_BEGIN
class Node;

namespace ui
{
class UIWidgetData;
class UIWidgetFactory;

class UIWidgetSystem:public Object
{
private:
	UIWidgetFactory*_widgetFactory;
public:
	UIWidgetSystem();
	~UIWidgetSystem();
	CREATE_FUNC(UIWidgetSystem);
	bool init();
	//创建组件
	Node*createWidgetsByData(UIWidgetData*widgetData);
};
//end namespace ui
}
NS_SDL_END
#endif
