#ifndef __SDL_MenuItem_H__
#define __SDL_MenuItem_H__
#include<functional>

#include "SENode.h"
#include "SERect.h"
#include "SEColor.h"
#include "SEVector.h"
NS_SDL_BEGIN
typedef std::function<void (Object*)> ccMenuCallback;

class MenuItem : public Node
{
protected:
	//回调函数
	ccMenuCallback _callback;
	//是否可用
	bool _bEnabled;
	//是否点击
	bool _bSelected;
	//是否吞并时间
	bool _bSwallowed;
public:
	MenuItem();
	virtual ~MenuItem();
	static MenuItem*create(ccMenuCallback callback);
	bool initWithCallback(ccMenuCallback callback);
	//返回按钮区域
	Rect rect() const;
	void setCallback(const ccMenuCallback&callback);
	//是否可用
	bool isEnabled() const;
	virtual void setEnabled(bool enabled);
	//是否点击
	bool isSelected() const;
	//吞并事件
	void setSwallowed(bool swallow);
	bool isSwallowed()const;
	//activate
	virtual void activate();
	//
	virtual void selected();
	virtual void unselected();
protected:
	virtual void updateImagesVisibility();//hook
};
//------------------MenuItemSprite----------------------------
class MenuItemSprite : public MenuItem
{
protected:
	Node*_normalImage;
	Node*_selectedImage;
	Node*_disabledImage;
protected:
	virtual void updateImagesVisibility();
public:
	MenuItemSprite();
	virtual ~MenuItemSprite();
	static MenuItemSprite*create(Node*normalSprite,Node*selectedSprite);
	static MenuItemSprite*create(Node*normalSprite,Node*selectedSprite,Node*disabledSprite);
	static MenuItemSprite*create(Node*normalSprite,Node*selectedSprite,Node*disabledSprite,ccMenuCallback callback);

	bool initWithNormalSprite(Node*normalSprite,Node*selectedSprite);
	bool initWithNormalSprite(Node*normalSprite,Node*selectedSprite,Node* disabledSprite);
	bool initWithNormalSprite(Node*normalSprite,Node*selectedSprite,Node* disabledSprite,ccMenuCallback callback);
	
	void setNormalImage(Node*normalSprite);
	void setSelectedImage(Node*selectedSprite);
	void setDisabledImage(Node*disabledImage);
	//
	virtual void setEnabled(bool enable);
	//override
	virtual void selected();
	virtual void unselected();
};
//--------------------------------MenuItemImage--------------------------------
class MenuItemImage : public MenuItemSprite
{
public:
	MenuItemImage();
	virtual ~MenuItemImage();
	static MenuItemImage*create(const std::string& normaiImage,const std::string&selectedImage,const std::string&disableImage="");
	static MenuItemImage*create(const std::string& normaiImage,const std::string&selectedImage,const std::string&disableImage,const ccMenuCallback& callback);
	bool initWithImage(const std::string&normalSprite,const std::string& selectedSprite,const std::string& disabledSprite,const ccMenuCallback& callback);
	//覆盖	
	void setNormalImage(const std::string& normalImage);
	void setSelectedImage(const std::string& selectedSprite);
	void setDisabledImage(const std::string& disabledImage);
};
//--------------------------------MenuItemLabel--------------------------------
class MenuItemLabel:public MenuItem
{
private:
	Node*_label;
	Color4B _colorBackup;
	float _originalScale;
	Color4B _dsiabledColor;
public:
	MenuItemLabel();
	virtual ~MenuItemLabel();
	static MenuItemLabel*create(Node*label,ccMenuCallback callback=nullptr);
	//static MenuItemLabel*create(std::string text,ccMenuCallback callback);
	bool init(Node*label,ccMenuCallback callback);
	//override
	virtual void selected();
	virtual void unselected();

};
//----------------------------------MenuItemToggle--------------------------
class MenuItemToggle:public MenuItem
{
private:
	unsigned int _selectedIndex;
	Vector<MenuItem*> _subItems;
public:
	MenuItemToggle():_selectedIndex(-1){}
	virtual ~MenuItemToggle(){}

	static MenuItemToggle* createWithCallback(const ccMenuCallback&callback,MenuItem*item1,...);
	static MenuItemToggle* createWithCallback(const ccMenuCallback&callback,const Vector<MenuItem*>&items);
	bool initWithCallback(const ccMenuCallback&callback,const Vector<MenuItem*>& items);
	
	unsigned int getSelectedIndex()const{return _selectedIndex;}
	void setSelectedIndex(unsigned int index);

	MenuItem*getSelectedItem()const{return _subItems.at(_selectedIndex);}
	const Vector<MenuItem*>& getSubItems()const{return _subItems;}
	//override
	virtual void activate();
	virtual void selected();
	virtual void unselected();
	virtual void setEnabled(bool enabled);
};
NS_SDL_END
#endif
