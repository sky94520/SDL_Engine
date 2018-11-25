#ifndef __SDL_RadioButton_H__
#define __SDL_RadioButton_H__
#include "UIAbstractCheckButton.h"
NS_SDL_BEGIN
namespace ui
{
class RadioButtonGroup;
class RadioButton:public AbstractCheckButton
{
	friend class RadioButtonGroup;
public:
	enum class EventType
	{
		SELECTED,
		UNSELECTED,
	};
public:
	typedef std::function<void(Object*,EventType)> ccRadioButtonCallback;
private:
	ccRadioButtonCallback _radioButtonEventCallback;
	RadioButtonGroup*_group;
public:
	RadioButton();
	~RadioButton();
	static RadioButton*create(const std::string&background
			 ,const std::string&cross
			 ,TextureResType resType = TextureResType::LOCAL);
	static RadioButton*create(const std::string&background
			,const std::string&backgroundSelected
			,const std::string&cross
			,const std::string&backgroundDisabled
			,const std::string&crossDisabled
			,TextureResType resType = TextureResType::LOCAL);

	void addEventListener(const ccRadioButtonCallback&callback);
protected:
	virtual void touchBeganHook(Touch*touch,SDL_Event*event);
	virtual void touchMovedInHook(Touch*touch,SDL_Event*event);
	virtual void touchMovedOutHook(Touch*touch,SDL_Event*event);
	virtual void touchEndedHook(Touch*touch,SDL_Event*event);
	virtual void touchCancelledHook(Touch*touch,SDL_Event*event);
private:
	void updateStatus(bool selected);
	void selected();
	void unselected();
	//发送事件给本radio button并且设置相应状态
	void dispatchSelectChangeEvent(bool selected);
};
}//end namespace ui
NS_SDL_END
#endif