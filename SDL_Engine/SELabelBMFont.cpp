#include "SELabelBMFont.h"
#include "SESprite.h"
#include "SEBMFontConfiguration.h"
#include "SEDirector.h"
#include "SETextureCache.h"
#include "SEStringUtils.h"

NS_SDL_BEGIN

LabelBMFont::LabelBMFont()
	:_pConfiguration(nullptr)
	,_width(0)
	,_color(255, 255, 255)
{
	_cascadeOpacity = true;
}

LabelBMFont::~LabelBMFont()
{
	SDL_SAFE_RELEASE_NULL(_pConfiguration);
}

LabelBMFont* LabelBMFont::create(const std::string& text, const std::string& fontFile)
{
	auto label = new LabelBMFont();

	if (label && label->init(text, fontFile))
		label->autorelease();
	else
		SDL_SAFE_DELETE(label);

	return label;
}

LabelBMFont* LabelBMFont::create(const std::string& text, const std::string& fontFile, int width, const Color3B& color)
{
	auto label = new LabelBMFont();

	if (label && label->init(text, fontFile, width, color))
		label->autorelease();
	else
		SDL_SAFE_DELETE(label);

	return label;
}

bool LabelBMFont::init(const std::string& text, const std::string& fontFile)
{
	//加载字体文件
	_pConfiguration = BMFontConfiguration::createWithFile(fontFile);
	SDL_SAFE_RETAIN(_pConfiguration);
	//设置字符串
	this->setString(text);

	return true;
}

bool LabelBMFont::init(const std::string& text, const std::string& fontFile, int width, const Color3B& color)
{
	//加载字体文件
	_pConfiguration = BMFontConfiguration::createWithFile(fontFile);
	SDL_SAFE_RETAIN(_pConfiguration);

	_width = width;
	_color = color;
	//设置字符串
	this->setString(text);

	return true;
}

Color3B LabelBMFont::getColor() const
{
	return _color;
}

void LabelBMFont::setColor(const Color3B& color)
{
	for (auto it = _children.begin(); it != _children.end(); it++)
	{
		auto node = *it;

		node->setColorMod(color);
	}
	_color = color;
}

void LabelBMFont::setString(const std::string& text)
{
	if (_text == text)
		return;
	_text = text;
	//隐藏当前全部精灵
	for (auto child : _children)
	{
		child->setVisible(false);
	}
	//重新创建精灵
	this->createFontChars();
}

void LabelBMFont::createFontChars()
{
	if (_text.empty())
		return;

	float nextFontPositionX = 0.f;  //下一个字的横向绘制位置  
	float nextFontPositionY = 0.f;   //下一个字的纵向绘制位置 
	//unsigned int quantityOfLines = 1;
	unsigned int totalHeight = 0;//字符串的总高度
	float longestLine = 0.f;        // 最长的一行的宽度
	Size tmpSize;

	//取得utf-8的字符串长度
	int utf8len = StringUtils::cc_utf8_strlen(_text.c_str(), -1);
	if (utf8len == 0)
		return;
	//将字符串转化为utf-16
	unsigned short* pUniStr = new unsigned short[utf8len + 1];
	pUniStr[utf8len] = 0;

	const char* p = _text.c_str();
	for (int i = 0;i < utf8len;i++)
	{
		pUniStr[i] = StringUtils::cc_utf8_get_char(p);
		p = cc_utf8_next_char(p);
	}
	//std::u16string str(pUniStr);

	//auto stringLen = str.length();
	unsigned int stringLen = StringUtils::cc_wcslen(pUniStr);
	//由行高乘行数最得要显示的字符串占用的总高度 */ 
    totalHeight = _pConfiguration->_commonHeight;
	//遍历所有的字
	for (unsigned int i = 0;i < stringLen; i++)
	{
		unsigned short c = pUniStr[i];
		//遇到换行符则进行换行处理
		if (c == '\n')
		{
			nextFontPositionX = 0;
			nextFontPositionY += _pConfiguration->_commonHeight;
			totalHeight += _pConfiguration->_commonHeight;
			continue;
		}
		//查找当前节点的映射信息
		auto it = _pConfiguration->_pBitmapFontMap->find(c);
		//获取当前字的映射信息
		auto& fontDef = it->second;
		Rect rect = Rect(fontDef.x, fontDef.y, fontDef.width, fontDef.height);
		//取得字相关精灵
		Sprite* fontChar = this->getChildByTag<Sprite*>(i);
		auto texture = Director::getInstance()->getTextureCache()->addImage(_pConfiguration->_atlasNames[fontDef.page]);
		//新建
		if (fontChar == nullptr)
		{
			fontChar = Sprite::createWithTexture(texture, rect);

			this->addChild(fontChar,0, i);
		}
		else
		{
			fontChar->initWithTexture(texture, rect, false, rect.origin, rect.size);
			fontChar->setVisible(true);
			fontChar->setOpacity(255);
		}
		//避免超过
		if (_width != 0 && nextFontPositionX + fontDef.xadvance > _width)
		{
			nextFontPositionX = 0;
			nextFontPositionY += _pConfiguration->_commonHeight;
			totalHeight += _pConfiguration->_commonHeight;
		}
		//计算字的位置,注意：因为锚点设的精灵正中心，所以位置应该是左下角位置加上大小的一半再进行相关偏移和间距调整计算。
		fontChar->setPosition( nextFontPositionX + fontDef.xoffset + fontDef.width / 2.0f ,
			(float) nextFontPositionY + fontDef.yoffset + fontDef.height / 2.0f  );
		// 更新绘制下一个字的横向位置
		nextFontPositionX += fontDef.xadvance; 
		//设置色彩，  
	        fontChar->setColorMod(_color);
		//取得最长的一行的宽度  
		if (longestLine < nextFontPositionX)  
		{  
			longestLine = nextFontPositionX;  
		}
	}
	//设置当前字符串在屏幕上占用的矩形位置  
	tmpSize.width  = (float) longestLine;  
	tmpSize.height = (float) totalHeight;  
	//调用基类CCNode的函数设置为原始大小  
	this->setContentSize(tmpSize);
	//释放字符串
	delete[] pUniStr;
}

NS_SDL_END
