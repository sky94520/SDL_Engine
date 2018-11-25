#ifndef __SDL_UIWidgetManager_H__
#define __SDL_UIWidgetManager_H__
#include<string>
#include<memory>
#include "../SEObject.h"
#include "../../rapidxml/rapidxml.hpp"

NS_SDL_BEGIN
class Node;
namespace ui
{
class UIWidgetData;
class UIWidgetDataSetting;
class UIWidgetSystem;

class UIWidgetManager:public Object
{
private:
	UIWidgetDataSetting*_widgetDataSetting;//属性设置器
	UIWidgetSystem*_widgetSystem;

	static UIWidgetManager*_pInstance;
private:
	UIWidgetManager();
	~UIWidgetManager();
public:
	static UIWidgetManager*getInstance()
	{
		if(_pInstance == nullptr)
		{
			_pInstance = new UIWidgetManager();
		}
		return _pInstance;
	}
	static void purge();

	Node*createWidgetsWithXml(const std::string&xmlPath);
private:
	UIWidgetData*createWidgetData(rapidxml::xml_node<>*node);
	//设置属性
	void setWidgetBaseData(rapidxml::xml_node<>*node,UIWidgetData*widgetData);
	void setWidgetProperties(rapidxml::xml_node<>*properties,UIWidgetData*widgetData);
};
//end namespace ui
}
NS_SDL_END
#endif
