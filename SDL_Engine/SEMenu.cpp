#include "SEMenu.h"
#include "SETouch.h"
#include "SEMenuItem.h"
#include "SEEventListenerTouchOneByOne.h"
#include "SEEventDispatcher.h"
NS_SDL_BEGIN

const int Menu::DEFAULT_PADDING = 5;

Menu::Menu()
	:_enabled(false)
	,_state(State::WAITING)
{
}

Menu::~Menu()
{
	//printf("menu已释放\n");
}

Menu*Menu::create(MenuItem*item,...)
{
	std::vector<MenuItem*> items;
	va_list args;
	va_start(args,item);
	//如果item有值
	if(item)
	{
		items.push_back(item);
		
		MenuItem*i = va_arg(args,MenuItem*);
		while(i)
		{
			items.push_back(i);
			i = va_arg(args,MenuItem*);
		}
	}
	return Menu::createWithArray(items);
}

Menu*Menu::createWithArray(const std::vector<MenuItem*>& items)
{
	auto menu = new Menu();
	if(menu && menu->initWithArray(items))
		menu->autorelease();
	else
		SDL_SAFE_DELETE(menu);
	return menu;
}

bool Menu::init()
{
	//设置为可用
	_enabled = true;
	//为该menu添加事件监听器
	auto listener = EventListenerTouchOneByOne::create();

	listener->onTouchBegan = SDL_CALLBACK_2(Menu::onTouchBegan,this);
	listener->onTouchMoved = SDL_CALLBACK_2(Menu::onTouchMoved,this);
	listener->onTouchEnded = SDL_CALLBACK_2(Menu::onTouchEnded,this);
	listener->onTouchCancelled = SDL_CALLBACK_2(Menu::onTouchCancelled,this);
	//设置优先级
	listener->setPriority(0);
	//添加到事件监听器中
	_eventDispatcher->addEventListener(listener,this);
	listener->setSwallowTouches(true);

	return true;
}

bool Menu::initWithArray(const std::vector<MenuItem*>& items)
{
	if(!Layer::init() || !Menu::init())
		return false;

	//循环加入项目
	for(unsigned int i=0;i<items.size();i++)
	{
		this->addChild(items.at(i),i);
	}

	return true;
}

void Menu::setEnabled(bool enable)
{
	_enabled = enable;
	
	for(auto child:_children)
	{
		static_cast<MenuItem*>(child)->setEnabled(enable);
	}
}

bool Menu::isEnabled() const
{
	return _enabled;
}

void Menu::alignItemsVertically()
{
	this->alignItemsVerticallyWithPadding(DEFAULT_PADDING);
}

void Menu::alignItemsVerticallyWithPadding(float padding)
{
	float height = -padding;

	for (const auto& child : _children)
		height += child->getContentSize().height * child->getScaleY() + padding;

	float y = height / 2.f;
	
	for (const auto& child : _children)
	{
		child->setPosition(0.f,y + child->getContentSize().height * child->getScaleY() / 2.f);
		y += child->getContentSize().height * child->getScaleY() + padding;
	}
	
}

bool Menu::onTouchBegan(Touch*touch,SDL_Event*event)
{
	//不可用则跳过
	if(_state != State::WAITING|| !_visible || !_enabled)
		return false;
	auto selectedItems = getItemForTouch(touch);

	/*if(selectedItems.empty())
		return false;*/
	for(auto item:selectedItems)
	{
		_state = State::TRACLING_TOUCH;
		_selectedItems.push_back(item);

		SDL_SAFE_RETAIN(item);
		item->selected();

		if(item->isSwallowed())
			return true;
	}

	return false;
}
void Menu::onTouchMoved(Touch*touch,SDL_Event*event)
{
	//获取当前item
	auto currentItems = getItemForTouch(touch);

	for(auto currentItem:currentItems)
	{
		//点击的项目是否在_selectedItems中
		auto iter = std::find(_selectedItems.begin(),_selectedItems.end(),currentItem);
		//未找到
		if(iter == _selectedItems.end())
		{
			currentItem->selected();
		}
		else
		{
			MenuItem* item = *iter;
			_selectedItems.erase(iter);
			SDL_SAFE_RELEASE(item);
		}
	}
	//所有在_selectedItems的，都是没有触摸到的
	for(auto item:_selectedItems)
	{
		item->unselected();
		SDL_SAFE_RELEASE(item);
	}

	_selectedItems = currentItems;

	for(auto item:_selectedItems)
	{
		SDL_SAFE_RETAIN(item);
	}

}
void Menu::onTouchEnded(Touch*touch,SDL_Event*event)
{
	//this->retain();
	for(auto item:_selectedItems)
	{
		item->activate();
		item->unselected();
		SDL_SAFE_RELEASE(item);
	}
	//TODO
	_selectedItems.clear();

	_state = State::WAITING;
	//this->release();?
}

void Menu::onTouchCancelled(Touch*touch,SDL_Event*event)
{
}

std::vector<MenuItem*> Menu::getItemForTouch(Touch*touch)
{
	std::vector<MenuItem*> items;

	Point loc = touch->getLocation();
	for(auto child=_children.begin();child!=_children.end();++child)
	{
		MenuItem* item = static_cast<MenuItem*>(*child);
		//过滤不可见和不可用的按钮
		if (!item->isEnabled() || !item->isVisible())
			continue;
		Rect rect = item->rect();
		Point position = item->getWorldPosition();
		rect.origin.x = position.x;
		rect.origin.y = position.y;

		if(rect.containsPoint(loc))
			items.push_back(item);
	}
	return items;
}
NS_SDL_END
