#ifndef __SDL_LabelDotChar_H__
#define __SDL_LabelDotChar_H__
#include <string>
#include <cstring>
#include <algorithm>

#include "SENode.h"
#include "SESize.h"
#include "SEColor.h"
#include "SESprite.h"
#include "SELabel.h"

NS_SDL_BEGIN
class LabelDotChar:public Label
{
private:
	int _pxsize;//点阵的大小
	Sprite*_sprite;
	Color3B _color;
public:
	LabelDotChar();
	~LabelDotChar();
	static LabelDotChar*create(const std::string&text,unsigned int pxsize,const Color3B&color);
	bool init(const std::string&text,unsigned int pxsize,const Color3B&color);

	virtual void setString(const std::string&label);
	virtual void draw();
private:
	Size getSizeByText(const std::string&text);
};
NS_SDL_END
#endif
