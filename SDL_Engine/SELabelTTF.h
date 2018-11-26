#ifndef __LabelTTF_H__
#define __LabelTTF_H__
#include<string>

#include "SENode.h"
#include "SEColor.h"
#include "SELabel.h"
NS_SDL_BEGIN
class Sprite;
/*内部保存了一个Sprite来进行渲染，不保存Font,每次setString时都会重新生成*/
class LabelTTF:public Label
{
private:
	std::string _ttfPath;//字体路径
	int _size;//字体大小
	Color4B _color;//字体颜色
	Uint32 _wrapLine;//换行时每行的宽度
	Sprite*_sprite;//进行渲染
public:
	LabelTTF();
	~LabelTTF();

	static LabelTTF*create(const std::string&text,int size=20,const Color4B&color=Color4B(0,0,0),const std::string&ttfPath="fonts/Marker Felt.ttf");
	static LabelTTF*createWithWrapLine(const std::string&text,Uint32 wrapLine,int size=20,const Color4B&color=Color4B(0,0,0),const std::string&ttfPath="fonts/Marker Felt.ttf");

	bool init(const std::string&text,int size,const Color4B&color,const std::string&ttfPath);
	bool initWithWrapLine(const std::string&text,Uint32 wrapLine,int size,const Color4B&color,const std::string&ttfPath);
	//设置行宽度，会重新生成
	void setWrapLine(Uint32 wrapLine);
	//设置颜色
	void setColor(const Color4B& color);
	Color4B getColor() const;
	//设置字符串，会重新生成
	virtual void setString(const std::string&label);
	virtual std::string getString()const;
private:
	void updateInnerSprite();
};
NS_SDL_END
#endif
