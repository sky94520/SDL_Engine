#ifndef __SDL_Button_H__
#define __SDL_Button_H__
#include<string>
#include "UIWidget.h"
NS_SDL_BEGIN
class Sprite;
namespace ui
{
class Button:public Widget
{
private:
	Sprite*_normalSprite;
	Sprite*_selectedSprite;
	Sprite*_disabledSprite;
public:
	Button();
	~Button();
	static Button*create(const std::string& normalImage,const std::string&selectedImage=""
		,const std::string&disableImage="",TextureResType resType=TextureResType::LOCAL);
	bool init(const std::string&normalImage,const std::string&selectedImage
		,const std::string&disabledImage,TextureResType resType);

	void loadNormalImage(const std::string&normalImage);
	void loadSelectedImage(const std::string&selectedImage);
	void loadDisabledImage(const std::string&disabledImage);
	//override
	void setOpacity(Uint8 opacity);
	Uint8 getOpacity()const;

	virtual void setTouchEnabled(bool enable);
	virtual Node* getVirtualRenderer();
protected:
	virtual void touchBeganHook(Touch*touch,SDL_Event*event);
	virtual void touchMovedInHook(Touch*touch,SDL_Event*event);
	virtual void touchMovedOutHook(Touch*touch,SDL_Event*event);
	virtual void touchEndedHook(Touch*touch,SDL_Event*event);
	virtual void touchCancelledHook(Touch*touch,SDL_Event*event);
private:
	void selected();
	void unselected();
};
}//namespace ui
NS_SDL_END
#endif