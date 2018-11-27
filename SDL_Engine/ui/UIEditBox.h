#ifndef __SDL_EditBox_H__
#define __SDL_EditBox_H__

#include <string>
#include "UIWidget.h"

NS_SDL_BEGIN
class Scale9Sprite;
class Label;
class LabelDotChar;
class EventListenerKeyboard;
class EventListenerTextInput;

namespace ui
{

class EditBox;
enum class EditBoxEndAction
{
	UNKNOWN,
	RETURN,
};
class EditBoxDelegate
{
public:
	virtual ~EditBoxDelegate(){}
	//when an edit box gains focus call this function
	virtual void editBoxEditingDidBegin(EditBox*) {}
	//when this edit box lose focus call this function
	virtual void editBoxEditingEndWithAction(EditBox*, EditBoxEndAction) {}
	//when the text of this edit box changed
	virtual void editBoxTextChanged(EditBox*, const std::string&) {}
	//the key return pressed
	virtual void editBoxReturn(EditBox*) = 0;
};

class EditBox : public Widget
{
public:
	enum class InputMode
	{
		ANY, /*default*/
		/**
		 * The user is allowed to enter an integer value
		 */
		NUMERIC, 
		/**
		 * The user is allowed to enter a real number value
		 */
		DECIMAL,
	};
	enum class InputFlag
	{
		SENSITIVE,
		PASSWORD,
	};
public:
	EditBox();
	~EditBox();

	/**
	 * create a editbox, it allows the user input text
	 * @param size the content size
	 * @param bg the background of the widget
	 * @label the label show the text of inputing
	 * @return EditBox or nullptr
	 */
	static EditBox* create(const Size& size, Scale9Sprite* bg, Label* label);
	bool init(const Size& size, Scale9Sprite* bg, Label* label);

	void setTextInputEnabled(bool enabled);
	bool getTextInputEnabled() const;

	/**
	 * get/set the text of the edit box
	 */
	const std::string& getText() const;
	void setText(const std::string& text);

	/**
	 * get/set the max length
	 */ 
	unsigned int getMaxLength() const;
	void setMaxLength(unsigned int len);

	/**
	 * get/set the input mode
	 */
	InputMode getInputMode() const;
	void setInputMode(InputMode mode);

	/**
	 * get/set the input flag
	 */
	InputFlag getInputFlag() const;
	void setInputFlag(InputFlag flag);

	void setDelegate(EditBoxDelegate* pDelegate);

	Label* getLabel();
	void setLabel(Label* label);
protected:
	virtual bool hitTest(Touch*touch);

	virtual void touchBeganHook(Touch*touch,SDL_Event*event);
	virtual void touchMovedInHook(Touch*touch,SDL_Event*event);
	virtual void touchMovedOutHook(Touch*touch,SDL_Event*event);
	virtual void touchEndedHook(Touch*touch,SDL_Event*event);
	virtual void touchCancelledHook(Touch*touch,SDL_Event*event);
private:
	void onKeyPressed(SDL_Keycode keycode, SDL_Event* event);
	void onTextInput(char text[32], SDL_Event* event);

	void showCursor();
	void hideCursor();
	//更新光标的位置
	void updateCursor();
	//判断是否是整数
	bool isNumeric(char c);
	//判断是否是实数
	bool isDecimal(char c);
	//设置label显示的字符
	void updateLabelString();
private:
	Scale9Sprite* _bg;

	//show text
	Label* _label;
	//cursor and the position
	LabelDotChar* _cursor;
	unsigned int _index;

	//max length,default is max
	unsigned int _maxLength;
	std::string _text;
	InputMode _mode;
	InputFlag _flag;
	EditBoxDelegate* _delegate;
	bool _textInputEnabled;
	EventListenerKeyboard* _keyboardListener;
	EventListenerTextInput* _inputListener;
};
}//end namespace ui
NS_SDL_END
#endif
