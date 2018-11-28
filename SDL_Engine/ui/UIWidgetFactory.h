#ifndef __SDL_UIWidgetFactory_H__
#define __SDL_UIWidgetFactory_H__
#include<map>
#include<string>

#include "../SEObject.h"
#include "../SEColor.h"
#include "../SERect.h"

#include "UIWidgetData.h"

NS_SDL_BEGIN
class Node;
namespace ui
{
class UIWidgetData;

class UIWidgetFactory:public Object
{
public:
	CREATE_FUNC(UIWidgetFactory);
	Node* createWidgetByData(UIWidgetData*widgetData);
	bool init();
protected:
	//抽象方法，子类负责创建控件
	virtual Node* createWidget(UIWidgetData*widgetData);
private:
	Node* createNode(UIWidgetData*widgetData);
	Node* createLayer(UIWidgetData*widgetData);
	Node* createLayerColor(UIWidgetData*widgetData);
	Node* createSprite(UIWidgetData*widgetData);

	Node* createLabelTTF(UIWidgetData*widgetData);
	Node* createLabelAtlas(UIWidgetData*widgetData);
	Node* createLabelDotChar(UIWidgetData*widgetData);
	Node* createLabelBMFont(UIWidgetData*widgetData);

	Node* createXml(UIWidgetData*widgetData);
	Node* createButton(UIWidgetData*widgetData);
	Node* createCheckBox(UIWidgetData*widgetData);
	Node* createProgressTime(UIWidgetData*widgetData);
	Node* createRadioButton(UIWidgetData* widgetData);
	Node* createLoadingBar(UIWidgetData* widgetData);
	Node* createScale9Sprite(UIWidgetData* widgetData);

	Node* createSlider(UIWidgetData* widgetData);
	Node* createEditBox(UIWidgetData* widgetData);
private:
	/**
	@brief 反序列化 从字符串中解析得到Rect
	@str 形如 "20,20,20,20"的格式的文本
	@return 返回一个Rect,如果字符串不可用，返回结果未知
	*/
	Rect getRectFromString(const std::string& str);
	Color4B getColor4BFromString(const std::string& str);
	Size getSizeFromString(const std::string& str);
	Point getPointFromString(const std::string& str);
};
//end namespace ui
}
NS_SDL_END
#endif
