#ifndef __SDL_LabelAtlas_H__
#define __SDL_LabelAtlas_H__
#include<string>
#include "SENode.h"
#include "SETexture.h"
#include "SEProtocols.h"
NS_SDL_BEGIN
class Sprite;
class LabelAtlas:public Node,LabelProtocol
{
private:
	//图片的路径名字
	std::string _filePath;
	//每个字符占的宽度 高度
	int _widthPerGlyph;
	int _heightPerGlyph;
	//起始字符
	unsigned int _startChar;
	//要显示的字符
	std::string _text;
	//设置字距，默认为1
	float _fontKerning;
	//显示的精灵
	Sprite*_sprite;
public:
	LabelAtlas();
	virtual ~LabelAtlas();
	/*
	param:text 要显示的文本
	param:charMapFile 图片的路径
	param:itemWidth itemHeight 单个大小
	 param:startCharMap 为ascii码值
	*/
	static LabelAtlas*create(const std::string&text,const std::string&charMapFile,unsigned int itemWidth,unsigned int itemHeight,unsigned int startChar=0);
	static LabelAtlas*createWithColorKey(const std::string&text,const std::string&charMapFile,unsigned int itemWidth,unsigned int itemHeight,const Color4B&color,unsigned int startChar=0);
	bool init(const std::string&text,const std::string&charMapFile,unsigned int itemWidth,unsigned int itemHeight,unsigned int startChar);
	bool initWithColorKey(const std::string&text,const std::string&charMapFile,unsigned int itemWidth,unsigned int itemHeight,const Color4B&color,unsigned int startChar=0);
	//override
	virtual void draw();
	void setString(const std::string&label);
	std::string getString()const;
	float getFontKerning()const;
	/*设置字距，如果存在字体背景，可能会出现字块的重合
	,这个函数会影响content size，请注意使用规范.废弃的，不建议使用*/
	void setFontKerning(float kerning);
	/**获取内部精灵*/
	Sprite*getInnerSprite()const;
};
NS_SDL_END
#endif
