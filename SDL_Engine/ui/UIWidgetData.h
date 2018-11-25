#ifndef __SDL_UIWidgetData_H__
#define __SDL_UIWidgetData_H__
#include<map>
#include<vector>
#include<string>

#include "../SEObject.h"
#include "../SEPoint.h"
#include "../SESize.h"

NS_SDL_BEGIN

namespace ui
{
//控件类型
enum WidgetType
{
	kWidgetType_None,
	kWidgetType_Node,
	kWidgetType_Layer,
	kWidgetType_LayerColor,
	kWidgetType_Sprite,
	kWidgetType_LabelTTF,
	kWidgetType_LabelAtlas,
	kWidgetType_LabelDotChar,
	kWidgetType_LabelBMFont,
	kWidgetType_Xml,/*引用另一个xml文件*/
	kWidgetType_Button,
	kWidgetType_CheckBox,
	kWidgetType_RadioButton,
	kWidgetType_LoadingBar,
	kWidgetType_Scale9Sprite,
	kWidgetType_ProgressTimer,
};
class UIWidgetData:public Object
{
	SDL_SYNTHESIZE(WidgetType,_widgetType,WidgetType);
	SDL_SYNTHESIZE(Point,_position,Position);
	SDL_SYNTHESIZE(Size,_size,Size);
	SDL_SYNTHESIZE(Point,_scale,Scale);
	SDL_SYNTHESIZE(Point,_anchorPoint,AnchorPoint);
	SDL_SYNTHESIZE(int,_tag,Tag);
	SDL_SYNTHESIZE(Uint8, _opacity, Opacity);
	SDL_SYNTHESIZE(std::string,_name,Name);
private:
	std::vector<UIWidgetData*> _cldDataList;
	std::map<std::string,std::string> _properties;//特殊属性
public:
	UIWidgetData();
	~UIWidgetData();
	CREATE_FUNC(UIWidgetData);
	bool init();
	//添加节点数据
	void addCldData(UIWidgetData*cld);
	std::vector<UIWidgetData*>& getCldDataList();

	void addProperty(const std::string&name,const std::string&value);
	const std::map<std::string,std::string>& getProperties();
};
//end namespace ui
}
NS_SDL_END
#endif
