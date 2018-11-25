#include "UIWidgetFactory.h"
#include "UIWidgetData.h"
#include "UICheckBox.h"

#include "UIWidgetManager.h"
#include "UIWidget.h"
#include "UIButton.h"
#include "UIRadioButton.h"

#include "../SENode.h"
#include "../SELayer.h"
#include "../SELabelTTF.h"
#include "../SESprite.h"
#include "../SELabelAtlas.h"
#include "../SELayerColor.h"
#include "../SELabelDotChar.h"
#include "../SELabelBMFont.h"
#include "../SEProgressTimer.h"
#include "../SELoadingBar.h"
#include "../SESDLNS.h"
#include "../SEScale9Sprite.h"

NS_SDL_BEGIN
namespace ui
{
Node*UIWidgetFactory::createWidgetByData(UIWidgetData*widgetData)
{
	Node*widget = createWidget(widgetData);

	return widget;
}
bool UIWidgetFactory::init()
{
	return true;
}
Node*UIWidgetFactory::createWidget(UIWidgetData* widgetData)
{
	Node*widget = nullptr;
	WidgetType type = widgetData->getWidgetType();

	switch(type)
	{
	case kWidgetType_Node:widget = this->createNode(widgetData);break;
	case kWidgetType_Layer:widget = this->createLayer(widgetData);break;
	case kWidgetType_LayerColor:widget = this->createLayerColor(widgetData);break;
	case kWidgetType_Sprite:widget = this->createSprite(widgetData);break;
	case kWidgetType_LabelTTF:widget = this->createLabelTTF(widgetData);break;
	case kWidgetType_LabelAtlas:widget = this->createLabelAtlas(widgetData);break;
	case kWidgetType_LabelDotChar:widget = this->createLabelDotChar(widgetData);break;
	case kWidgetType_LabelBMFont:widget = this->createLabelBMFont(widgetData);break;
	case kWidgetType_Xml:widget = this->createXml(widgetData);break;
	case kWidgetType_Button:widget = this->createButton(widgetData);break;
	case kWidgetType_CheckBox:widget = this->createCheckBox(widgetData);break;
	case kWidgetType_RadioButton:widget = this->createRadioButton(widgetData);break;
	case kWidgetType_LoadingBar:widget = this->createLoadingBar(widgetData);break;
	case kWidgetType_Scale9Sprite: widget = this->createScale9Sprite(widgetData); break;
	case kWidgetType_ProgressTimer: widget = this->createProgressTime(widgetData); break;
	}
	if(widget)
	{
		widget->setPosition(widgetData->getPosition());
		widget->setScale(widgetData->getScale().x,widgetData->getScale().y);
		widget->setAnchorPoint(widgetData->getAnchorPoint());
		widget->setTag(widgetData->getTag());
		widget->setName(widgetData->getName());
		widget->setOpacity(widgetData->getOpacity());

		if (widgetData->getSize() != Size::ZERO)
		{
			widget->setContentSize(widgetData->getSize());
		}
	}
	return widget;
}
Node*UIWidgetFactory::createNode(UIWidgetData*widgetData)
{
	Node* node = Node::create();

	return node;
}
Node*UIWidgetFactory::createLayer(UIWidgetData*widgetData)
{
	Layer* layer = Layer::create();

	return layer;
}
Node*UIWidgetFactory::createLayerColor(UIWidgetData*widgetData)
{
	Rect r;
	Color4B color;
	auto& properties = widgetData->getProperties();
	for(auto property:properties)
	{
		std::string name = property.first;
		std::string value = property.second;
		if(name == "rect")
			r = getRectFromString(value);
		else if(name == "color")
			color = Color4B(value);
	}
	LayerColor*layer = LayerColor::create(color,r);
	return layer;
}
Node*UIWidgetFactory::createSprite(UIWidgetData*widgetData)
{
	std::string filename;
	TextureResType resType = TextureResType::LOCAL;
	Rect rect;
	bool bRect = false;

	auto& properties = widgetData->getProperties();
	for(auto property:properties)
	{
		std::string name = property.first;
		std::string value = property.second;
		if(name == "filename")
		{
			filename = value;
		}
		else if(name == "res-type")
		{
			if(value == "LOCAL") resType = TextureResType::LOCAL;
			else if(value == "XML") resType = TextureResType::XML;
		}
		else if (name == "rect")
		{
			rect = RectFromString(value);
			bRect = true;
		}
	}
	Sprite* sprite = nullptr;
	//文件名为空时创建一个空的sprite
	if (filename.empty())
	{
		sprite = Sprite::create();
	}
	else
	{
		switch (resType)
		{
		case TextureResType::LOCAL:
		{
			if (bRect)
				sprite = Sprite::create(filename,rect);
			else
				sprite = Sprite::create(filename);
		}break;
		case TextureResType::XML:
			sprite = Sprite::createWithSpriteFrameName(filename);break;
		}
	}

	return sprite;
}
Node*UIWidgetFactory::createLabelTTF(UIWidgetData*widgetData)
{
	LabelTTF*label = nullptr;
	std::string text;
	int fontSize = 20;
	Color4B color;
	std::string path = "fonts/Marker Felt.ttf";
	int wrapLine = 0;

	auto& properties = widgetData->getProperties();
	bool bWrapLine = false;

	for(auto property:properties)
	{
		std::string name = property.first;
		std::string value = property.second;

		if(name == "text")
			text = value;
		else if(name == "font-size")
			fontSize = SDL_atoi(value.c_str());
		else if (name == "font-color")
			color = this->getColor4BFromString(value.c_str());
		else if (name == "font-path")
			path = value;
		else if (name == "wrap-line")
		{
			bWrapLine = true;
			wrapLine = SDL_atoi(value.c_str());
		}
	}
	if (bWrapLine)
	{
		label = LabelTTF::createWithWrapLine(text,wrapLine,fontSize,color,path);
	}
	else
	{
		label = LabelTTF::create(text,fontSize,color,path);
	}

	return label;
}
Node*UIWidgetFactory::createLabelAtlas(UIWidgetData*widgetData)
{
	std::string text;
	Color4B colorKey;
	bool bColorKey = false;
	int itemWidth = 0;
	int itemHeight = 0;
	int startChar = 0;
	std::string filename = "";
	float kerning = 1.f;

	auto& properties = widgetData->getProperties();
	for(auto property:properties)
	{
		std::string name = property.first;
		const char* value = property.second.c_str();
		if(name == "text")
			text = value;
		else if(name == "color-key")
		{
			bColorKey = true;
			colorKey = Color4B(value);
		}
		else if(name == "item-width")
			itemWidth = SDL_atoi(value);
		else if(name == "item-height")
			itemHeight = SDL_atoi(value);
		else if(name == "filename")
			filename = value;
		else if(name == "start-char")
			startChar = SDL_atoi(value);
		else if(name == "kerning")
			kerning = (float)SDL_atof(value);
	}
	LabelAtlas*label = nullptr;
	if(bColorKey)
		label = LabelAtlas::createWithColorKey(text,filename,itemWidth,itemHeight,colorKey,startChar);
	else
		label = LabelAtlas::create(text,filename,itemWidth,itemHeight,startChar);
	if(kerning < 1.f)
		label->setFontKerning(kerning);
	return label;
}
Node*UIWidgetFactory::createLabelDotChar(UIWidgetData*widgetData)
{
	//必须属性
	std::string text;
	int pxsize = 0;
	Color3B color;

	auto& properties = widgetData->getProperties();
	for(auto property:properties)
	{
		std::string name = property.first;
		const char* value = property.second.c_str();
		if(name == "text")
			text = value;
		else if(name == "pxsize")
			pxsize = SDL_atoi(value);
		else if(name == "color")
		{
			color = Color3B(value);
		}
	}
	auto label = LabelDotChar::create(text,pxsize,color);
	return label;
}

Node* UIWidgetFactory::createLabelBMFont(UIWidgetData*widgetData)
{
	std::string text;
	std::string fontFile;
	int width = 0;
	Color3B color(255, 255, 255);

	auto& properties = widgetData->getProperties();

	for(auto property : properties)
	{
		std::string name = property.first;
		const char* value = property.second.c_str();

		if(name == "text")
			text = value;
		else if(name == "font-path")
			fontFile = value;
		else if (name == "width")
			width = SDL_atoi(value);
		else if(name == "font-color")
			color = Color3B(value);
	}
	LabelBMFont* label = LabelBMFont::create(text, fontFile, width, color);

	return label;
}

Node*UIWidgetFactory::createXml(UIWidgetData*widgetData)
{
	auto& properties = widgetData->getProperties();
	std::string filename;
	for(auto property:properties)
	{
		std::string name = property.first;
		const char*value = property.second.c_str();
		if(name == "filename")
			filename = value;
	}
	//todo
	//加载xml文件
	Node*node = UIWidgetManager::getInstance()->createWidgetsWithXml(filename);
	return node;
}
Node*UIWidgetFactory::createButton(UIWidgetData*widgetData)
{
	std::string normalImage;
	std::string selectedImage;
	std::string disabledImage;
	TextureResType resType = TextureResType::LOCAL;

	auto& properties = widgetData->getProperties();
	for(auto property:properties)
	{
		std::string name = property.first;
		std::string value = property.second.c_str();
		if(name == "res-type")
		{
			if(value == "LOCAL") resType = TextureResType::LOCAL;
			else if(value == "XML") resType = TextureResType::XML;
		}
		else if(name == "normal-image")
			normalImage = value;
		else if(name == "selected-image")
			selectedImage = value;
		else if(name == "disabled-image")
			disabledImage = value;
	}
	Button*btn = Button::create(normalImage,selectedImage,disabledImage,resType);
	return btn;
}

Node*UIWidgetFactory::createCheckBox(UIWidgetData*widgetData)
{
	std::string background;
	std::string cross;
	TextureResType resType = TextureResType::LOCAL;

	auto& properties = widgetData->getProperties();
	for(auto property:properties)
	{
		std::string name = property.first;
		std::string value = property.second.c_str();
		if(name == "res-type")
		{
			if(value == "LOCAL") resType = TextureResType::LOCAL;
			else if(value == "XML") resType = TextureResType::XML;
		}
		else if(name == "background")
			background = value;
		else if(name == "cross")
			cross = value;
	}
	//todo
	CheckBox*checkBox = CheckBox::create(background,cross,resType);
	
	return checkBox;
}

Node* UIWidgetFactory::createRadioButton(UIWidgetData* widgetData)
{
	std::string background;
	std::string backgroundSelected;
	std::string cross;
	std::string backgroundDisabled;
	std::string crossDisabled;
	TextureResType resType = TextureResType::LOCAL;

	auto& properties = widgetData->getProperties();
	
	for(auto property:properties)
	{
		std::string name = property.first;
		std::string value = property.second.c_str();

		if(name == "res-type")
		{
			if(value == "LOCAL") resType = TextureResType::LOCAL;
			else if(value == "XML") resType = TextureResType::XML;
		}
		else if(name == "background")
			background = value;
		else if(name == "cross")
			cross = value;
		else if (name == "background-disabled")
			backgroundDisabled = value;
	}
	RadioButton* radioBtn = RadioButton::create(background,backgroundSelected
		,cross,backgroundDisabled,crossDisabled,resType);

	return radioBtn;
}

Node* UIWidgetFactory::createLoadingBar(UIWidgetData* widgetData)
{
	std::string filename;
	TextureResType resType = TextureResType::LOCAL;
	float percent = 0.f;
	LoadingBarDir dir = LoadingBarDir::kLoadingBar_Left;

	auto& properties = widgetData->getProperties();
	
	for(auto property:properties)
	{
		std::string name = property.first;
		std::string value = property.second.c_str();

		if(name == "res-type")
		{
			if(value == "LOCAL") resType = TextureResType::LOCAL;
			else if(value == "XML") resType = TextureResType::XML;
		}
		else if(name == "filename")
			filename = value;
		else if (name == "percent")
		{
			percent = (float)SDL_atof(value.c_str());
		}
		else if (name == "dir")
		{
			int nDir = SDL_atoi(value.c_str());
			dir = static_cast<LoadingBarDir>(nDir);
		}
	}
	Sprite* sprite = nullptr;
	LoadingBar* loadingBar = nullptr;

	switch (resType)
	{
	case SDL::ui::TextureResType::LOCAL:
		sprite = Sprite::create(filename);
		break;
	case SDL::ui::TextureResType::XML:
		sprite = Sprite::createWithSpriteFrameName(filename);
		break;
	default:
		break;
	}
	loadingBar = LoadingBar::create(sprite,percent,dir);

	return loadingBar;
}

Node* UIWidgetFactory::createScale9Sprite(UIWidgetData* widgetData)
{
	std::string filename;
	TextureResType resType = TextureResType::LOCAL;
	Size preferredSize;
	Rect capInsets;

	auto& properties = widgetData->getProperties();

	for (auto property : properties)
	{
		std::string name = property.first;
		std::string value = property.second;

		if (name == "res-type")
		{
			if (value == "LOCAL") resType = TextureResType::LOCAL;
			else if (value == "XML") resType = TextureResType::XML;
		}
		else if (name == "filename")
		{
			filename = value;
		}
		else if (name == "preferred-size")
		{
			preferredSize = this->getSizeFromString(value);
		}
		else if (name == "cap-insets")
		{
			capInsets = this->getRectFromString(value);
		}
	}
	//先创建精灵
	Sprite* sprite = nullptr;
	Scale9Sprite* ret = nullptr;

	switch (resType)
	{
	case SDL::ui::TextureResType::LOCAL:
		sprite = Sprite::create(filename);
		break;
	case SDL::ui::TextureResType::XML:
		sprite = Sprite::createWithSpriteFrameName(filename);
		break;
	default:
		break;
	}
	ret = Scale9Sprite::create(sprite, capInsets);
	ret->setPreferredSize(preferredSize);

	return ret;
}

Node*UIWidgetFactory::createProgressTime(UIWidgetData*widgetData)
{
	std::string filename;
	TextureResType resType = TextureResType::LOCAL;

	ProgressTimer::Type type;
	float percentage = 0.f;
	Point midpoint;
	Point barChangeRate;

	auto& properties = widgetData->getProperties();

	for (auto property : properties)
	{
		std::string name = property.first;
		std::string value = property.second;

		if (name == "res-type")
		{
			if (value == "LOCAL") resType = TextureResType::LOCAL;
			else if (value == "XML") resType = TextureResType::XML;
		}
		else if (name == "filename")
		{
			filename = value;
		}
		else if (name == "type")
		{
			if (value == "BAR") type = ProgressTimer::Type::BAR;
			else if (value == "RADIAL") type = ProgressTimer::Type::RADIAL;
		}
		else if (name == "percentage")
		{
			percentage = SDL_atof(value.c_str());
		}
		else if (name == "midpoint")
		{
			midpoint = this->getPointFromString(value);
		}
		else if (name == "bar-change-rate")
		{
			barChangeRate = this->getPointFromString(value);
		}
	}
	//先创建精灵
	Sprite* sprite = nullptr;
	ProgressTimer* ret = nullptr;

	switch (resType)
	{
	case SDL::ui::TextureResType::LOCAL:
		sprite = Sprite::create(filename);
		break;
	case SDL::ui::TextureResType::XML:
		sprite = Sprite::createWithSpriteFrameName(filename);
		break;
	default:
		break;
	}
	
	ret = ProgressTimer::create(sprite);
	ret->setType(type);
	ret->setMidpoint(midpoint);

	if (type == ProgressTimer::Type::BAR)
	{
		ret->setBarChangeRate(barChangeRate);
	}
	ret->setPercentage(percentage);

	return ret;
}

Rect UIWidgetFactory::getRectFromString(const std::string&str)
{
	Rect ret;
	SDL_sscanf(str.c_str(),"%f,%f,%f,%f",&ret.origin.x,&ret.origin.y,&ret.size.width,&ret.size.height);
	return ret;
}

Color4B UIWidgetFactory::getColor4BFromString(const std::string& str)
{
	Color4B color;
	int r = 0,g = 0,b = 0,a = 0;

	SDL_sscanf(str.c_str(),"%d,%d,%d,%d",&r,&g,&b,&a);

	color = Color4B(r,g,b,a);
	return color;
}

Size UIWidgetFactory::getSizeFromString(const std::string& str)
{
	Size size;
	SDL_sscanf(str.c_str(), "%f,%f", &size.width, &size.height);

	return size;
}

Point UIWidgetFactory::getPointFromString(const std::string& str)
{
	Point point;
	SDL_sscanf(str.c_str(), "%f,%f", &point.x, &point.y);

	return point;
}
//end namespace ui
}
NS_SDL_END
