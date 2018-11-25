#include "UIWidgetDataSetting.h"
#include "UIWidgetData.h"

NS_SDL_BEGIN
namespace ui
{
bool UIWidgetDataSetting::init()
{
	return true;
}
void UIWidgetDataSetting::setBaseData(UIWidgetData*widgetData,const std::string&name,const std::string&value)
{
	if(name == "position")
		widgetData->setPosition(this->convertStrToPoint(value));
	else if (name == "size")
		widgetData->setSize(this->convertStrToSize(value));
	else if(name == "scale")
		widgetData->setScale(this->convertStrToPoint(value));
	else if(name == "anchor-point")
		widgetData->setAnchorPoint(this->convertStrToPoint(value));
	else if(name == "name")
		widgetData->setName(value);
	else if(name == "tag")
		widgetData->setTag(SDL_atoi(value.c_str()));
	else if(name == "opacity")
		widgetData->setOpacity(SDL_atoi(value.c_str()));
}
void UIWidgetDataSetting::setExteraData(UIWidgetData*widgetData,const std::string&name,const std::string&value)
{
	widgetData->addProperty(name,value);
}
Point UIWidgetDataSetting::convertStrToPoint(const std::string&text)
{
	Point point;
	SDL_sscanf(text.c_str(),"%f,%f",&point.x,&point.y);
	return point;
}

Size UIWidgetDataSetting::convertStrToSize(const std::string& text)
{
	Size size;
	SDL_sscanf(text.c_str(), "%f,%f", &size.width, &size.height);
	return size;
}
//end namespace ui
}
NS_SDL_END
