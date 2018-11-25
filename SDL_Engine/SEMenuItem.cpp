#include "SEMenuItem.h"
#include "SELabelTTF.h"
#include "SESprite.h"
NS_SDL_BEGIN

MenuItem::MenuItem():
	_callback(nullptr)
	,_bEnabled(false)
	,_bSelected(false)
	,_bSwallowed(false)
{
}

MenuItem::~MenuItem()
{
}

MenuItem*MenuItem::create(ccMenuCallback callback)
{
	auto item = new MenuItem();
	if(item && item->initWithCallback(callback))
		item->autorelease();
	else
		SDL_SAFE_DELETE(item);
	return item;
}

bool MenuItem::initWithCallback(ccMenuCallback callback)
{
	_callback = callback;
	_bEnabled = true;
	_bSelected = false;

	return true;
}

Rect MenuItem::rect() const
{
	Rect rect;

	rect.origin = _position;
	rect.size = Size(_contentSize.width, _contentSize.height);

	return rect;
}

void MenuItem::setCallback(const ccMenuCallback&callback)
{
	_callback = callback;

	_bEnabled = true;
	_bSelected = false;

	this->updateImagesVisibility();
}

bool MenuItem::isEnabled() const
{
	return _bEnabled;
}

void MenuItem::setEnabled(bool enabled)
{
	_bEnabled = enabled;
	
}

bool MenuItem::isSelected()const
{
	return _bSelected;
}

void MenuItem::activate()
{
	if(_bEnabled)
	{
		if(_bSelected && _callback)
		{
			_callback(this);
		}
	}
}

void MenuItem::selected()
{
	_bSelected = true;
}

void MenuItem::unselected()
{
	_bSelected = false;
}

void MenuItem::setSwallowed(bool swallow)
{
	_bSwallowed = swallow;
}

bool MenuItem::isSwallowed()const
{
	return _bSwallowed;
}

void MenuItem::updateImagesVisibility()
{
	//不做任何事情
}
//---------------------------MenuItemSprite----------------------
MenuItemSprite::MenuItemSprite()
	:_normalImage(nullptr)
	,_selectedImage(nullptr)
	,_disabledImage(nullptr)
{
}

MenuItemSprite::~MenuItemSprite()
{
}

void MenuItemSprite::updateImagesVisibility()
{
	if(_bEnabled)
	{
		if(_normalImage) _normalImage->setVisible(true);
		if(_selectedImage) _selectedImage->setVisible(false);
		if(_disabledImage) _disabledImage->setVisible(false);
	}
	else
	{
		if(_disabledImage)
		{
			if(_normalImage) _normalImage->setVisible(false);
			if(_selectedImage) _selectedImage->setVisible(false);
			if(_disabledImage) _disabledImage->setVisible(true);
		}
		else
		{
			if(_normalImage) _normalImage->setVisible(true);
			if(_selectedImage) _selectedImage->setVisible(false);
		}
	}
}

MenuItemSprite*MenuItemSprite::create(Node*normalSprite,Node*selectedSprite)
{
	auto menuItem = new MenuItemSprite();
	if(menuItem && menuItem->initWithNormalSprite(normalSprite,selectedSprite))
		menuItem->autorelease();
	else
		SDL_SAFE_DELETE(menuItem);
	return menuItem;
}

MenuItemSprite*MenuItemSprite::create(Node*normalSprite,Node*selectedSprite,Node*disabledSprite)
{
	auto menuItem = new MenuItemSprite();
	if(menuItem && menuItem->initWithNormalSprite(normalSprite,selectedSprite,disabledSprite))
		menuItem->autorelease();
	else
		SDL_SAFE_DELETE(menuItem);
	return menuItem;
}

MenuItemSprite*MenuItemSprite::create(Node*normalSprite,Node*selectedSprite,Node*disabledSprite,ccMenuCallback callback)
{
	auto menuItem = new MenuItemSprite();
	if(menuItem && menuItem->initWithNormalSprite(normalSprite,selectedSprite,disabledSprite,callback))
		menuItem->autorelease();
	else
		SDL_SAFE_DELETE(menuItem);
	return menuItem;
}

bool MenuItemSprite::initWithNormalSprite(Node*normalSprite,Node*selectedSprite)
{
	setNormalImage(normalSprite);
	setSelectedImage(selectedSprite);
	return true;
}

bool MenuItemSprite::initWithNormalSprite(Node*normalSprite,Node*selectedSprite,Node* disabledSprite)
{
	setNormalImage(normalSprite);
	setSelectedImage(selectedSprite);
	setDisabledImage(disabledSprite);

	return true;
}

bool MenuItemSprite::initWithNormalSprite(Node*normalSprite,Node*selectedSprite,Node*disabledSprite,ccMenuCallback callback)
{
	MenuItem::initWithCallback(callback);
	setNormalImage(normalSprite);
	setSelectedImage(selectedSprite);
	setDisabledImage(disabledSprite);

	return true;
}

void MenuItemSprite::setNormalImage(Node*normalSprite)
{
	if(_normalImage != normalSprite)
	{
		if(normalSprite)
		{
			//删除原来的sprite
			if(_normalImage)
				this->removeChild(_normalImage);
		}
		_normalImage = normalSprite;
		this->addChild(_normalImage,3);
		//设置content size
		setContentSize(_normalImage->getContentSize());
		//设置位置
		Size size = this->getContentSize();
		_normalImage->setPosition(size.width/2,size.height/2);

		this->updateImagesVisibility();
	}
}

void MenuItemSprite::setSelectedImage(Node*selectedSprite)
{
	if(_selectedImage != selectedSprite)
	{
		if(selectedSprite)
		{
			if(_selectedImage)
				this->removeChild(_selectedImage);
		}
		_selectedImage = selectedSprite;
		this->addChild(_selectedImage,2);

		Size size = this->getContentSize();
		_selectedImage->setPosition(size.width/2,size.height/2);

		this->updateImagesVisibility();
	}
}

void MenuItemSprite::setDisabledImage(Node*disabledSprite)
{
	if(_disabledImage != disabledSprite)
	{
		//如果不为空
		if(disabledSprite)
		{
			if(_disabledImage)
				this->removeChild(_disabledImage);
			_disabledImage = disabledSprite;
			this->addChild(disabledSprite,1);

			Size size = this->getContentSize();
			_disabledImage->setPosition(size.width/2,size.height/2);
	
			this->updateImagesVisibility();
		}
	}
}

void MenuItemSprite::setEnabled(bool enabled)
{
	MenuItem::setEnabled(enabled);
	updateImagesVisibility();
}

void MenuItemSprite::selected()
{
	MenuItem::selected();
	if(_bEnabled && _normalImage)
	{
		if(_disabledImage)
			_disabledImage->setVisible(false);
		if(_selectedImage)
		{
			_normalImage->setVisible(false);
			_selectedImage->setVisible(true);
		}
		else
			_normalImage->setVisible(true);
	}

}

void MenuItemSprite::unselected()
{
	MenuItem::unselected();
	if(_bEnabled && _normalImage)
	{
		_normalImage->setVisible(true);
		if(_selectedImage)
			_selectedImage->setVisible(false);
		if(_disabledImage)
			_disabledImage->setVisible(false);
	}
}

MenuItemImage::MenuItemImage()
	:MenuItemSprite()
{
}

MenuItemImage::~MenuItemImage()
{
}

MenuItemImage*MenuItemImage::create(const std::string& normalImage,const std::string&selectedImage,const std::string&disableImage)
{
	auto menuItem = new MenuItemImage();
	if(menuItem && menuItem->initWithImage(normalImage,selectedImage,disableImage,nullptr))
		menuItem->autorelease();
	else
		SDL_SAFE_DELETE(menuItem);
	return menuItem;
}

MenuItemImage*MenuItemImage::create(const std::string& normalImage,const std::string&selectedImage,const std::string&disableImage,const ccMenuCallback&callback)
{
	auto menuItem = new MenuItemImage();
	if(menuItem && menuItem->initWithImage(normalImage,selectedImage,disableImage,callback))
		menuItem->autorelease();
	else
		SDL_SAFE_DELETE(menuItem);
	return menuItem;
}

bool MenuItemImage::initWithImage(const std::string& normalImage,const std::string&selectedImage,const std::string&disableImage,const ccMenuCallback&callback)
{
	Sprite*normalSprite = Sprite::create(normalImage);
	Sprite*selectedSprite = Sprite::create(selectedImage);
	Sprite*disableSprite = Sprite::create(disableImage);

	return MenuItemSprite::initWithNormalSprite(normalSprite,selectedSprite,disableSprite,callback);
}

void MenuItemImage::setNormalImage(const std::string&normalImage)
{
	MenuItemSprite::setNormalImage(Sprite::create(normalImage));
}

void MenuItemImage::setSelectedImage(const std::string&selectedImage)
{
	MenuItemSprite::setSelectedImage(Sprite::create(selectedImage));
}

void MenuItemImage::setDisabledImage(const std::string&disableImage)
{
	MenuItemSprite::setDisabledImage(Sprite::create(disableImage));
}
//-----------------------------MenuItemLabel-------------------------------
MenuItemLabel::MenuItemLabel()
	:_label(nullptr)
	 ,_originalScale(1.f)
{
}

MenuItemLabel::~MenuItemLabel()
{
}

MenuItemLabel*MenuItemLabel::create(Node*label,ccMenuCallback callback)
{
	auto item = new MenuItemLabel();
	if(item && item->init(label,callback))
		item->autorelease();
	else
		SDL_SAFE_DELETE(item);
	return item;
}

bool MenuItemLabel::init(Node*label,ccMenuCallback callback)
{
	MenuItem::initWithCallback(callback);
	//添加
	this->addChild(label);
	_label = label;
	Size s = _label->getContentSize();
	setContentSize(s);
	_label->setPosition(s.width/2,s.height/2);
	return true;
}

void MenuItemLabel::selected()
{
	MenuItem::selected();
	_label->setScaleX(_originalScale*1.5f);
	_label->setScaleY(_originalScale*1.5f);
}

void MenuItemLabel::unselected()
{
	MenuItem::unselected();
	_label->setScaleX(_originalScale);
	_label->setScaleY(_originalScale);
}
//-------------------------------MenuItemToggle---------------------------------
MenuItemToggle*MenuItemToggle::createWithCallback(const ccMenuCallback&callback,const Vector<MenuItem*>&items)
{
	auto menuItem = new MenuItemToggle();
	if(menuItem && menuItem->initWithCallback(callback,items))
		menuItem->autorelease();
	else
		SDL_SAFE_DELETE(menuItem);
	return menuItem;
}

MenuItemToggle*MenuItemToggle::createWithCallback(const ccMenuCallback&callback,MenuItem*item1,...)
{
	Vector<MenuItem*> items;
	va_list args;
	va_start(args,item1);
	if(item1)
	{
		items.push_back(item1);
		MenuItem*i = va_arg(args,MenuItem*);
		while(i)
		{
			items.push_back(i);
			i = va_arg(args,MenuItem*);
		}
	}
	va_end(args);
	return createWithCallback(callback,items);
}

bool MenuItemToggle::initWithCallback(const ccMenuCallback&callback,const Vector<MenuItem*>&items)
{
	if(MenuItem::initWithCallback(callback))
	{
		_subItems = items;
		setSelectedIndex(0);
		return true;
	}
	return false;
}

void MenuItemToggle::setSelectedIndex(unsigned int index)
{
	//切换状态
	if(index != _selectedIndex && _subItems.size() > 0)
	{
		_selectedIndex = index;
		MenuItem*currentItem = (MenuItem*)getChildByTag(1000);
		if(currentItem)
			currentItem->removeFromParent();
		MenuItem*item = _subItems.at(_selectedIndex);
		this->addChild(item);
		item->setTag(1000);
		this->setContentSize(item->getContentSize());
		//this->setPosition(item->getPosition());
	}
}

void MenuItemToggle::activate()
{
	if(_bEnabled)
	{
		if(_bSelected && _callback)
		{
			unsigned int index = (_selectedIndex + 1)%_subItems.size();
			setSelectedIndex(index);
			_callback(this);
		}
	}
}

void MenuItemToggle::selected()
{
	MenuItem::selected();
	_subItems.at(_selectedIndex)->selected();
}

void MenuItemToggle::unselected()
{
	MenuItem::unselected();
	_subItems.at(_selectedIndex)->unselected();
}

void MenuItemToggle::setEnabled(bool enabled)
{
	if(isEnabled() == enabled)
		return ;
	MenuItem::setEnabled(enabled);
	//子孩子不可点击
	for(auto item:_subItems)
	{
		item->setEnabled(enabled);
	}
}
NS_SDL_END
