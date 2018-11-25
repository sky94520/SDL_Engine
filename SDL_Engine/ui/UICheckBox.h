#ifndef __SDL_CheckBox_H__
#define __SDL_CheckBox_H__
#include<functional> 
#include "UIAbstractCheckButton.h"
NS_SDL_BEGIN
class Object;
namespace ui
{
class CheckBox:public AbstractCheckButton
{
public:
	enum class EventType
	{
		SELECTED,
		UNSELECTED,
	};
public:
	typedef std::function<void(Object*,EventType)> ccCheckBoxCallback;
private:
	ccCheckBoxCallback _checkBoxEventCallback;
public:
	CheckBox();
	~CheckBox();
	static CheckBox*create(const std::string&background
			 ,const std::string&cross
			 ,TextureResType resType = TextureResType::LOCAL);
	static CheckBox*create(const std::string&background
			,const std::string&backgroundSelected
			,const std::string&cross
			,const std::string&backgroundDisabled
			,const std::string&crossDisabled
			,TextureResType resType = TextureResType::LOCAL);

	void addEventListener(const ccCheckBoxCallback&callback);
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
};
}//end namespace ui
NS_SDL_END
#endif