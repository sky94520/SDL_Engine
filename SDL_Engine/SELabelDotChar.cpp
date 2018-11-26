#include "SELabelDotChar.h"
#include "SEDirector.h"
#include "SEStringUtils.h"
#include "SEValue.h"
#include "extensions/SDL_PxString.h"

NS_SDL_BEGIN
LabelDotChar::LabelDotChar()
	:_pxsize(0)
	,_sprite(nullptr)
{
}

LabelDotChar::~LabelDotChar()
{
}

LabelDotChar*LabelDotChar::create(const std::string&text,unsigned int pxsize,const Color3B&color)
{
	auto label = new LabelDotChar();
	if(label && label->init(text,pxsize,color))
		label->autorelease();
	else
		SDL_SAFE_DELETE(label);
	return label;
}

bool LabelDotChar::init(const std::string&text,unsigned int pxsize,const Color3B&color)
{
	_pxsize = pxsize;
	_color = color;
	this->setString(text);
	return true;
}

void LabelDotChar::setString(const std::string&text)
{
	if(_text == text)
		return;
	_text = text;

	if (strlen(text.c_str()) == 0)
	{
		if (_sprite != nullptr)
			_sprite->setVisible(false);
		return ;
	}
	else if (_sprite != nullptr)
	{
		_sprite->setVisible(true);
	}
	//获取区域大小
	Size size = this->getSizeByText(text);
	Size oldSize = this->getContentSize();

	Renderer* renderer = Director::getInstance()->getRenderer();
	Texture* bg = nullptr;

	if(_sprite != nullptr)
		_sprite->removeFromParent();

	_sprite = Sprite::createWithTexture(SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,(int)size.width,(int)size.height);
	_sprite->setPosition(size.width / 2 , size.height / 2);
	this->addChild(_sprite);
	//重新设置大小
	this->setContentSize(size);
	
	bg = _sprite->getTexture();
	bg->setBlendMode(SDL_BLENDMODE_BLEND);
	//设置bg为当前渲染目标
	SDL_BlendMode oldBlendMode = SDL_BLENDMODE_NONE;
	renderer->getDrawBlendMode(&oldBlendMode);

	renderer->setDrawBlendMode(SDL_BLENDMODE_NONE);
	renderer->setTarget(bg);
	auto oldColor = renderer->getDrawColor();
	//这个是任何一个颜色都无所谓，反正是透明颜色
	renderer->setDrawColor(0,0,0,0);
	renderer->clear();
	//渲染
	SDL_Color color = {_color.r,_color.g,_color.b,255};

	ASCII_px_6x8::DrawPxString(renderer->getSDL_Renderer(),text.c_str(),text.size(),0,0,_pxsize,color);
	//设置为默认渲染目标
	renderer->setTarget(nullptr);
	//恢复
	renderer->setDrawColor(oldColor.r,oldColor.g,oldColor.b,oldColor.a);
	renderer->setDrawBlendMode(oldBlendMode);
}

void LabelDotChar::draw()
{
	//留给Director的一个小小改变
	if(_parent == nullptr)
		_sprite->draw();
}

Size LabelDotChar::getSizeByText(const std::string&text)
{
	ValueVector vec = StringUtils::split(text,"\n");
	//获取分割字符串的最大值
	int width = -1;
	int height = vec.size();

	for(auto iter = vec.begin();iter != vec.end();++iter)
	{
		int size = (*iter).asString().size();
		if(size > width)
			width = size;
	}
	//根据当前 pxsize确定大小
	return Size(width*_pxsize*6.f,height*_pxsize*8.f);
}
NS_SDL_END
