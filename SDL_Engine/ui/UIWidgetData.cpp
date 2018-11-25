#include "UIWidgetData.h"
NS_SDL_BEGIN

namespace ui
{
UIWidgetData::UIWidgetData()
	:_widgetType(kWidgetType_None)
	,_scale(1.f,1.f)
	,_anchorPoint(0.5f,0.5f)
	,_tag(-1)
	,_opacity(255)
{
}
UIWidgetData::~UIWidgetData()
{
	for(auto iter = _cldDataList.begin();iter != _cldDataList.end();)
	{
		auto data = *iter;
		data->release();
		iter = _cldDataList.erase(iter);
	}
}
bool UIWidgetData::init()
{
	return true;
}
void UIWidgetData::addCldData(UIWidgetData*cld)
{
	cld->retain();
	_cldDataList.push_back(cld);
}
std::vector<UIWidgetData*>& UIWidgetData::getCldDataList()
{
	return _cldDataList;
}
void UIWidgetData::addProperty(const std::string&name,const std::string&value)
{
	_properties.insert(std::make_pair(name,value));
}
const std::map<std::string,std::string>& UIWidgetData::getProperties()
{
	return _properties;
}
//end namespace ui
}
NS_SDL_END
