#include "SELabelAtlas.h"
#include "SEDirector.h"
#include "SESurface.h"
#include "SETextureCache.h"
#include "SEFileUtils.h"
#include "SESprite.h"
NS_SDL_BEGIN
LabelAtlas::LabelAtlas()
	:_widthPerGlyph(0)
	,_heightPerGlyph(0)
	,_startChar(0)
	,_fontKerning(1.f)
	,_sprite(nullptr)
{
}

LabelAtlas::~LabelAtlas()
{
}

LabelAtlas*LabelAtlas::create(const std::string&text,const std::string&charMapFile,unsigned int itemWidth,unsigned int itemHeight,unsigned int startChar)
{
	auto label = new LabelAtlas();

	if(label && label->init(text,charMapFile,itemWidth,itemHeight,startChar))
		label->autorelease();
	else
		SDL_SAFE_DELETE(label);

	return label;
}

LabelAtlas*LabelAtlas::createWithColorKey(const std::string&text,const std::string&charMapFile,unsigned int itemWidth,unsigned int itemHeight,const Color4B&color,unsigned int startChar)
{
	auto label = new LabelAtlas();
	if(label && label->initWithColorKey(text,charMapFile,itemWidth,itemHeight,color,startChar))
		label->autorelease();
	else
		SDL_SAFE_DELETE(label);
	return label;
}

bool LabelAtlas::init(const std::string&text,const std::string&charMapFile,unsigned int itemWidth,unsigned int itemHeight,unsigned int startChar)
{
	_filePath = charMapFile;
	_widthPerGlyph = itemWidth;
	_heightPerGlyph = itemHeight;
	_startChar = startChar;

	//加载资源
	auto texture = Director::getInstance()->getTextureCache()->addImage(charMapFile,charMapFile);
	
	if(texture == nullptr)
		return false;

	this->setString(text);
	return true;
}

bool LabelAtlas::initWithColorKey(const std::string&text,const std::string&charMapFile,unsigned int itemWidth,unsigned int itemHeight,const Color4B&color,unsigned int startChar)
{
	_filePath = charMapFile;
	_widthPerGlyph = itemWidth;
	_heightPerGlyph = itemHeight;
	_startChar = startChar;
	//只有没有对应的texture时，才会添加
	if(Director::getInstance()->getTextureCache()->getTextureForKey(charMapFile) == nullptr)
	{
		auto fullpath = FileUtils::getInstance()->fullPathForFilename(charMapFile);
		if(fullpath.empty())
			return false;
		//加载Surface并设置关键色
		Surface*surface = Surface::create(fullpath);
		Uint32 colorKey = surface->mapRGBA(color.r,color.g,color.b,color.a);
		surface->setColorKey(colorKey);
		//创建texture
		Renderer* renderer = Director::getInstance()->getRenderer();
		Texture* texture = Texture::createWithSurface(renderer, surface);
		//添加到TextureCache
		Director::getInstance()->getTextureCache()->addTexture(texture,charMapFile);
	}
	this->setString(text);
	return true;
}

void LabelAtlas::draw()
{
	//留给Director的一个小小改变
	if(_parent == nullptr)
		_sprite->draw();
}

void LabelAtlas::setString(const std::string&text)
{
	if(_text == text || text.empty())
		return;
	_text = text;
	Renderer*renderer = Director::getInstance()->getRenderer();
	Texture*texture = Director::getInstance()->getTextureCache()->getTextureForKey(_filePath);
	//获取资源大小
	int textureWidth=0,textureHeight=0;
	texture->query(NULL,NULL,&textureWidth,&textureHeight);
	
	int rowItemsNum = textureWidth/_widthPerGlyph;
	//每一列的字符数
	//int columnItemsNum = textureHeight/_heightPerGlyph;
	//资源大小
	int width = int((text.size() - 1)*_widthPerGlyph*_fontKerning + _widthPerGlyph);
	int height = _heightPerGlyph;
	
	Texture*bg = nullptr;
	//如果精灵存在，则删除
	if(_sprite != nullptr)
		_sprite->removeFromParent();
	//创建精灵
	_sprite = Sprite::createWithTexture(SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,width,height);
	_sprite->setPosition(0.5f*width,0.5f*height);
	//根据当前大小重新设置
	this->setContentSize(Size(width,height));
	this->addChild(_sprite);
	//设置混合模式
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
	SDL_Rect destRect = {0,0,_widthPerGlyph,_heightPerGlyph};
	for(unsigned int i=0;i<text.size();i++)
	{
		unsigned int ch = text.at(i);
		if(ch < _startChar)
			continue;
		SDL_Rect srcRect = {0,0,_widthPerGlyph,_heightPerGlyph};
		srcRect.x = (ch-_startChar)%rowItemsNum *_widthPerGlyph;
		srcRect.y = (ch-_startChar)/rowItemsNum * _heightPerGlyph;

		renderer->copy(texture,&srcRect,&destRect);
		
		destRect.x += int(_fontKerning*_widthPerGlyph);
	}
	//设置为默认渲染目标
	renderer->setTarget(nullptr);
	//恢复
	renderer->setDrawColor(oldColor.r,oldColor.g,oldColor.b,oldColor.a);
	renderer->setDrawBlendMode(oldBlendMode);
}
std::string LabelAtlas::getString()const
{
	return _text;
}
float LabelAtlas::getFontKerning()const
{
	return _fontKerning;
}
void LabelAtlas::setFontKerning(float kerning)
{
	if(_fontKerning != kerning)
	{
		_fontKerning = kerning;
		auto temp = _text;
		_text.clear();
		this->setString(temp);
	}
}

Sprite*LabelAtlas::getInnerSprite()const
{
	return _sprite;
}
NS_SDL_END
