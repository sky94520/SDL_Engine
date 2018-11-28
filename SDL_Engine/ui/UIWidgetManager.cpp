#include "UIWidgetManager.h"
#include "UIWidgetData.h"
#include "UIWidgetDataSetting.h"
#include "UIWidgetSystem.h"

#include "../SEFileUtils.h"

NS_SDL_BEGIN
namespace ui
{
UIWidgetManager*UIWidgetManager::_pInstance = nullptr;

UIWidgetManager::UIWidgetManager()
	:_widgetDataSetting(nullptr),_widgetSystem(nullptr)
{
	_widgetDataSetting = UIWidgetDataSetting::create();
	_widgetSystem = UIWidgetSystem::create();

	SDL_SAFE_RETAIN(_widgetDataSetting);
	SDL_SAFE_RETAIN(_widgetSystem);
}
UIWidgetManager::~UIWidgetManager()
{
	SDL_SAFE_RELEASE_NULL(_widgetDataSetting);
	SDL_SAFE_RELEASE_NULL(_widgetSystem);
}
void UIWidgetManager::purge()
{
	SDL_SAFE_RELEASE_NULL(_pInstance);
}
Node*UIWidgetManager::createWidgetsWithXml(const std::string&xmlPath)
{
	std::unique_ptr<char> text_ptr = std::move(FileUtils::getInstance()->getUniqueDataFromFile(xmlPath));
	//开始解析
	rapidxml::xml_document<> doc;
	doc.parse<0>(text_ptr.get());
	rapidxml::xml_node<>*root = doc.first_node();
	//读取xml并生成节点树
	auto widgetData =  this->createWidgetData(root);
	//创建组件
	auto node = _widgetSystem->createWidgetsByData(widgetData);
	//清除指针
	text_ptr.reset();
	return node;
}
//-------------------private----------------------------------
UIWidgetData*UIWidgetManager::createWidgetData(rapidxml::xml_node<>*node)
{
	UIWidgetData*widgetData = UIWidgetData::create();

		//设置基础属性
	this->setWidgetBaseData(node,widgetData);

	for(auto child=node->first_node();child;child = child->next_sibling())
	{
		if(SDL_strcmp(child->name(),"properties") == 0)//设置额外属性
			this->setWidgetProperties(child,widgetData);
		else
			widgetData->addCldData(this->createWidgetData(child));//递归调用
	}
	return widgetData;
}
void UIWidgetManager::setWidgetBaseData(rapidxml::xml_node<>*node,UIWidgetData*widgetData)
{
	//设置类型
	std::string name = node->name();
	if(name == "Node")
		widgetData->setWidgetType(kWidgetType_Node);
	else if(name == "Layer")
		widgetData->setWidgetType(kWidgetType_Layer);
	else if(name == "LayerColor")
		widgetData->setWidgetType(kWidgetType_LayerColor);
	else if(name == "Sprite")
		widgetData->setWidgetType(kWidgetType_Sprite);
	else if(name == "LabelTTF")
		widgetData->setWidgetType(kWidgetType_LabelTTF);
	else if(name == "LabelAtlas")
		widgetData->setWidgetType(kWidgetType_LabelAtlas);
	else if(name == "LabelDotChar")
		widgetData->setWidgetType(kWidgetType_LabelDotChar);
	else if (name == "LabelBMFont")
		widgetData->setWidgetType(kWidgetType_LabelBMFont);
	else if(name == "Xml")
		widgetData->setWidgetType(kWidgetType_Xml);
	else if(name == "Button")
		widgetData->setWidgetType(kWidgetType_Button);
	else if (name == "CheckBox")
		widgetData->setWidgetType(kWidgetType_CheckBox);
	else if (name == "RadioButton")
		widgetData->setWidgetType(kWidgetType_RadioButton);
	else if (name == "LoadingBar")
		widgetData->setWidgetType(kWidgetType_LoadingBar);
	else if (name == "Scale9Sprite")
		widgetData->setWidgetType(kWidgetType_Scale9Sprite);
	else if (name == "ProgressTimer")
		widgetData->setWidgetType(kWidgetType_ProgressTimer);
	else if (name == "Slider")
		widgetData->setWidgetType(kWidgetType_Slider);
	else if (name == "EditBox")
		widgetData->setWidgetType(kWidgetType_EditBox);
	//设置基础属性
	for(auto attr = node->first_attribute();attr;attr = attr->next_attribute())
	{
		_widgetDataSetting->setBaseData(widgetData,attr->name(),attr->value());
	}
}
void UIWidgetManager::setWidgetProperties(rapidxml::xml_node<>*properties,UIWidgetData*widgetData)
{
	for(auto property = properties->first_node();property;property = property->next_sibling())
	{
		std::string name = property->first_attribute("name")->value();
		std::string value = property->first_attribute("value")->value();

		_widgetDataSetting->setExteraData(widgetData,name,value);
	}
}
//end namespace ui
}
NS_SDL_END
