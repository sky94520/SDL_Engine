#ifndef __SDL_AbstractCheckButton_H__
#define __SDL_AbstractCheckButton_H__
#include<string>
#include "UIWidget.h"
NS_SDL_BEGIN
class Sprite;
namespace ui
{
class AbstractCheckButton:public Widget
{
protected:
	Sprite*_backgroundSprite;
	Sprite*_backgroundSelectedSprite;
	Sprite*_crossSprite;
	Sprite*_backgroundDisabledSprite;
	Sprite*_crossDisabledSprite;
	//当前处于的状态
	bool _selected;
public:
	static const int INTERVAL_TAG;
public:
	AbstractCheckButton();
	virtual ~AbstractCheckButton();

	bool init(const std::string&background
			 ,const std::string&backgroundSelected
			 ,const std::string&cross
			 ,const std::string&backgroundDisabled
			 ,const std::string&crossDisabled
			 ,TextureResType resType);
	bool init(const std::string&background
			 ,const std::string&cross
			 ,TextureResType resType);

	void loadTextureBackground(const std::string&background);
	void loadTextureBackgroundSelected(const std::string&backgroundSelected);
	void loadTextureCross(const std::string&cross);
	void loadTextureBackgroundDisabled(const std::string&backDisabled);
	void loadTextureCrossDisabled(const std::string&crossDisabled);

	bool isSelected()const;
	void setSelected(bool selected);

	Sprite* getCrossSprite()const;

	virtual void setTouchEnabled(bool enable);
private:
	//void initSprites();
};
}//end namespace ui
NS_SDL_END
#endif