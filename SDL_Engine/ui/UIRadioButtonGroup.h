#ifndef __SDL_UIRadioButtonGroup_H__
#define __SDL_UIRadioButtonGroup_H__
#include<functional>
#include "UIWidget.h"
#include "../SEVector.h"

NS_SDL_BEGIN
namespace ui
{
class RadioButton;
/*只是起一个管理的作用*/
class RadioButtonGroup:public Widget
{
	friend class RadioButton;
public:
	enum class EventType
	{
		SELECTED_CHANGE,
	};
	typedef std::function<void(RadioButton*,int,EventType)> ccRadioButtonGroupCallback;
private:
	Vector<RadioButton*> _radioButtons;
	RadioButton*_selectedButton;
	ccRadioButtonGroupCallback _radioButtonGroupCallback;
public:
	RadioButtonGroup();
	~RadioButtonGroup();
	CREATE_FUNC(RadioButtonGroup);

	void addEventListener(const ccRadioButtonGroupCallback&callback);
	//获取选中的索引
	int getSelectedIndex()const;
	RadioButton* getSelectedButton()const;
	//设置选中
	void setSelectedButton(unsigned int index);
	void setSelectedButton(RadioButton*radioButton);
	//取消选中
	void unselectedButton();
	//设置选中且不发送事件
	void setSelectedButtonWithoutEvent(unsigned int index);
	void setSelectedButtonWithoutEvent(RadioButton*radioButton);
	//添加单选按钮
	void addRadioButton(RadioButton*radioButton);
	//移除单选按钮
	void removeRadioButton(RadioButton*radioButton);
	void removeAllRadioButtons();
	//根据索引获取对应单选按钮
	RadioButton*getRadioButtonByIndex(int index);
	//获取单选按钮的数量
	int getNumberOfRadioButtons()const;
	//获取全部单选按钮
	Vector<RadioButton*>& getRadioButtonList();
};
}//end namespace ui
NS_SDL_END
#endif