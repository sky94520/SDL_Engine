#include "SENode.h"
#include "SERenderer.h"
#include "SEDirector.h"
#include "SEEventDispatcher.h"
#include "SEActionManager.h"
NS_SDL_BEGIN
Node::Node()
	:_name("")
	,_tag(-1)
	,_parent(NULL)
	,_visible(true) 
	,_localZOrder(0)
	,_globalZOrder(0.f)
	,_reorderChildDirty(false)
	,_running(false)
	,_position(Point::ZERO)
	,_contentSize(Size::ZERO)
	,_rotation(0.f)
	,_scaleX(1.f)
	,_scaleY(1.f)
	,_anchorPoint(0.5f,0.5f)
	,_flipX(false)
	,_flipY(false)
	,_realOpacity(255)
	,_displayedOpacity(255)
	,_cascadeOpacity(false)
	,_colorMod(255,255,255)
	,_cascadeScale(false)
	,_userdata(nullptr)
	,_userObject(nullptr)
	,_eventDispatcher(nullptr)
	,_actionManager(nullptr)
	,_scheduler(nullptr)
{
	_eventDispatcher = Director::getInstance()->getEventDispatcher();
	_eventDispatcher->retain();

	_actionManager = Director::getInstance()->getActionManager();
	_actionManager->retain();

	_scheduler = Director::getInstance()->getScheduler();
	_scheduler->retain();
}
Node::~Node()
{
	SDL_SAFE_RELEASE(_eventDispatcher);
	SDL_SAFE_RELEASE(_actionManager);
	SDL_SAFE_RELEASE(_scheduler);

	SDL_SAFE_RELEASE_NULL(_userObject);

	removeAllChildren();
}
bool Node::init()
{
	return true;
}
void Node::setParent(Node*parent)
{
	if(_parent)
	{
		this->retain();
		_parent->removeChild(this);
	}
	_parent = parent;
	_parent->addChild(this);
}
void Node::addChild(Node*child)
{
	this->addChild(child,child->_localZOrder,child->_tag);
}
void Node::addChild(Node*child,int localZOrder)
{
	this->addChild(child,localZOrder,child->_tag);
}
void Node::addChild(Node*child,int localZOrder,int tag)
{
	//保证只被加载一次
	if(child&&child->_parent==NULL)
	{
		_children.push_back(child);
		//add the reference
		child->retain();
		child->setParent(this);
		if(_running)
			child->onEnter();
		//child->setPosition(child->_position);
		child->setLocalZOrder(localZOrder);
		child->setTag(tag);
		_reorderChildDirty = true;
	}
}
void Node::removeChild(Node*child,bool cleanup)
{
	auto iter = std::find_if(_children.begin(),_children.end(),[child](Node*c)
	{
		return child == c;
	});
	if(iter == _children.end())
		return;
	if(cleanup)
	{
		child->cleanup();
	}
	child->_parent = nullptr;
	child->onExit();
	_children.erase(iter);

	child->release();
}
void Node::removeFromParent()
{
	if(_parent)
	{
		_parent->removeChild(this,true);
	}
}
void Node::removeAllChildren()
{
	for(auto it=_children.begin();it!=_children.end();)
	{
		auto node=*it;
		it = _children.erase(it);

		node->_parent = nullptr;

		node->onExit();

		node->release();
	}
}
void Node::cleanup()
{
	this->stopAllActions();
	this->unscheduleAllCallbacks();
	this->removeAllListeners();

	for( const auto &child: _children)
        child->cleanup();
}
void Node::onEnter()
{
	for(const auto&child:_children)
		child->onEnter();
	this->resume();
	_running = true;
}
void Node::onExit()
{
	for(const auto&child:_children)
		child->onExit();
	this->pause();
	_running=false;
}
void Node::resume()
{
	_eventDispatcher->resumeEventListenersForTarget(this);
	_actionManager->resumeTarget(this);
	_scheduler->resumeScheduleForTarget(this);
}
void Node::pause()
{
	_eventDispatcher->pauseEventListenersForTarget(this);
	_actionManager->pauseTarget(this);
	_scheduler->pauseScheduleForTarget(this);
}
void Node::pauseSchedulerAndActions()
{
	_actionManager->pauseTarget(this);
	_scheduler->pauseScheduleForTarget(this);
}
void Node::resumeSchedlerAndActions()
{
	_actionManager->resumeTarget(this);
	_scheduler->resumeScheduleForTarget(this);
}
void Node::update(float dt)
{
}
void Node::visit()
{
	//if the node is invisible,return directly.
	if (!_visible)
		return;

	if(_children.size() > 0)
	{
		sortAllChildren();

		unsigned int i = 0;
		//draw the localZOrder < 0
		for (;i < _children.size();i++)
		{
			auto child = _children.at(i);
			
			if (child != nullptr && child->_localZOrder < 0)
				child->visit();
			else
				break;
		}
		//draw self
		this->draw();
		//draw the localZOrder >= 0
		for(;i < _children.size();i++)
		{
			auto child = _children.at(i);

			child->visit();
		} 
	}
	else
	{
		this->draw();
	}
}
void Node::draw()
{
}
bool Node::isVisible()
{
	return _visible;
}
void Node::setVisible(bool isVisible)
{
	_visible = isVisible;
}
void Node::setPositionX(float x)
{
	_position.x = x;
}
void Node::setPositionY(float y)
{
	_position.y = y;
}
void Node::setPosition(const Point& pos)
{
	_position.x = pos.x;
	_position.y = pos.y;
}
void Node::setPosition(float x,float y)
{
	_position.x = x;
	_position.y = y;
}
Point Node::getPosition() const
{
	//return _position;
	return _position;
}
float Node::getPositionX()const
{
	return _position.x;
}
float Node::getPositionY()const
{
	return _position.y;
}
Point Node::getWorldPosition()const
{
	Point point(_position);

	Point parentAnchor;
	Size parentSize;
	for(Node*node = _parent;node!=nullptr;node = node->getParent())
	{
		parentAnchor = node->getAnchorPoint();
		parentSize = node->getContentSize();
		point += node->getPosition() - Point(parentAnchor.x*parentSize.width,parentAnchor.y*parentSize.height);
	}
	point.x -= _anchorPoint.x*_contentSize.width;
	point.y -= _anchorPoint.y*_contentSize.height;

	return point;
}
Point Node::convertToNodeSpace(const Point&worldPos)const
{
	Point pos = this->getWorldPosition();
	pos = worldPos - pos;
	return pos;
}
Point Node::convertToNodeSpaceAR(const Point&worldPos)const
{
	Point pos = this->convertToNodeSpace(worldPos);
	Size s = this->getContentSize();
	Point anchorPoint = this->getAnchorPoint();

	pos -= Point(s.width*anchorPoint.x,s.height*anchorPoint.y);

	return pos;
}
Point Node::convertToWorldSpace(const Point&nodePoint)const
{
	Point pos = this->getWorldPosition();
	pos += nodePoint;
	return pos;
}
Point Node::convertToWorldSpaceAR(const Point&nodePoint)
{
	Point pos = this->convertToWorldSpace(nodePoint);
	Size s = this->getContentSize();
	Point anchorPoint = this->getAnchorPoint();

	pos -= Point(s.width*anchorPoint.x,s.height*anchorPoint.y);
	
	return pos;
}
Size Node::getContentSize() const
{
	return _contentSize;
}
void Node::setContentSize(const Size&size)
{
	_contentSize = size;
}
Rect Node::getBoundingBox() const
{
	//是否级联
	float scaleX = _scaleX;
	float scaleY = _scaleY;
	if(_cascadeScale)
	{
		for(Node*node=_parent;node!=nullptr;node = node->getParent())
		{
			scaleX *= node->getScaleX();
			scaleY *= node->getScaleY();
		}
	}
	Point pos = _position;
	pos -= Point(_anchorPoint.x*_contentSize.width*scaleX,_anchorPoint.y*_contentSize.height*scaleY);
	Size size = Size(_contentSize.width*_scaleX,_contentSize.height*_scaleY);
	Rect rect=Rect(pos.x,pos.y,size.width,size.height);

	return rect;
}
float Node::getRotation()
{
	return _rotation;
}
void Node::setRotation(float rotation)
{
	if(_rotation == rotation)
		return ;
	
	_rotation = rotation;
}
float Node::getScaleX()
{
	return _scaleX;
}
void Node::setScaleX(float scaleX)
{
	_scaleX = scaleX;
}
float Node::getScaleY()
{
	return _scaleY;
}
void Node::setScaleY(float scaleY)
{
	_scaleY = scaleY;
}
void Node::setScale(float scaleX,float scaleY)
{
	_scaleX = scaleX;
	_scaleY = scaleY;
}
Point Node::getScale()const
{
	return Point(_scaleX,_scaleY);
}
Point Node::getWorldScale()const
{
	Point scale(_scaleX,_scaleY);
	for(Node*node = _parent;node!=nullptr;node = node->getParent())
	{
		scale.x *= node->getScaleX();
		scale.y *= node->getScaleY();
	}
	return scale;
}
Point Node::getAnchorPoint()const
{
	return _anchorPoint;
}
void Node::setAnchorPoint(const Point&anchorPoint)
{
	if(anchorPoint.x < 0 || anchorPoint.x > 1.f || anchorPoint.y < 0 || anchorPoint.y > 1.f)
		return;
	_anchorPoint.x = anchorPoint.x;
	_anchorPoint.y = anchorPoint.y;
}
void Node::setName(std::string name)
{
	_name = name;
}
std::string Node::getName() const
{
	return _name;
}
void Node::setTag(int tag)
{
	_tag = tag;
}
int Node::getTag()
{
	return _tag;
}
Node*Node::getChildByName(std::string name)
{
	SDLASSERT(name.length()!=0,"Invalid name");
	if(!_children.empty())
	{
		for(auto child=_children.begin();child != _children.end();)
		{
			if((*child)->getName() == name)
				return *child;
			child++;
		}
	}
	return nullptr;
}
Node*Node::getChildByTag(int tag)
{
	SDLASSERT(tag >= 0,"Invalid name");
	if(!_children.empty())
	{
		for(auto child=_children.begin();child != _children.end();)
		{
			if((*child)->getTag() == tag)
				return *child;
			child++;
		}
	}
	return nullptr;
}
void Node::sortAllChildren()
{
	if(_reorderChildDirty)
	{
		std::stable_sort(std::begin(_children),std::end(_children),nodeComparisonLess);
		_reorderChildDirty = false;
	}
}
bool Node::nodeComparisonLess(Node* n1, Node* n2)
{
	return n1->_localZOrder < n2->_localZOrder;
}
int Node::getLocalZOrder() const
{
	return this->_localZOrder;
}
void Node::setLocalZOrder(int localZOrder)
{
	_localZOrder = localZOrder;

	if (_parent != nullptr)
		this->_parent->_reorderChildDirty = true;
}

float Node::getGlobalZOrder() const
{
	return _globalZOrder;
}

void Node::setGlobalZOrder(float globalZOrder)
{
	_globalZOrder = globalZOrder;
}

bool Node::isFlipX()
{
	return _flipX;
}
bool Node::isFlipY()
{
	return _flipY;
}
void Node::setFlipX(bool flipX)
{
	_flipX = flipX;
}
void Node::setFlipY(bool flipY)
{
	_flipY = flipY;
}
//--------不透明度------------
void Node::setOpacity(Uint8 opacity)
{
	_realOpacity = _displayedOpacity = opacity;

	updateCascadeOpacity();
}

Uint8 Node::getOpacity() const
{
	return _realOpacity;
}

Uint8 Node::getDisplayedOpacity() const
{
	return _displayedOpacity;;
}

void Node::updateCascadeOpacity()
{
	Uint8 parentOpacity = 255;

	if (_parent != nullptr && _parent->isCascadeOpacityEnabled())
	{
		parentOpacity = _parent->getDisplayedOpacity();
	}
	this->updateDisplayedOpacity(parentOpacity);
}

void Node::disableCascadeOpacity()
{
   _displayedOpacity = _realOpacity;
    
    for(const auto& child : _children)
    {
        child->updateDisplayedOpacity(255);
    }
}

void Node::updateDisplayedOpacity(Uint8 parentOpacity)
{
	_displayedOpacity = Uint8( _realOpacity * parentOpacity / 255.f);

	if (_cascadeOpacity)
	{
		for (auto child : _children)
		{
			child->updateDisplayedOpacity(_displayedOpacity);
		}
	}
}

void Node::setCascadeOpacityEnabled(bool enabled)
{
	if (_cascadeOpacity == enabled)
		return;

	_cascadeOpacity = enabled;

	if (_cascadeOpacity)
		updateCascadeOpacity();
	else
		disableCascadeOpacity();
}

bool Node::isCascadeOpacityEnabled() const
{
	return _cascadeOpacity;
}
//---------------------Color mod-------------------
void Node::setColorMod(const Color3B&color)
{
	_colorMod = color;
}
void Node::setColorMod(Uint8 r,Uint8 g,Uint8 b)
{
	_colorMod.r = r;
	_colorMod.g = g;
	_colorMod.b = b;
}
Color3B Node::getColorMod()const
{
	return _colorMod;
}
//------------------------------------------------
void Node::setCascadeScaleEnabled(bool enabled)
{
	_cascadeScale = enabled;
}
bool Node::isCascadeScaleEnabled()const
{
	return _cascadeScale;
}
void Node::setUserData(void*userdata)
{
	_userdata = userdata;
}
void*Node::getUserData()const
{
	return _userdata;
}

void Node::setUserObject(Object*sender)
{
	if (sender == _userObject)
		return ;

	SDL_SAFE_RETAIN(sender);
	SDL_SAFE_RELEASE_NULL(_userObject);
	_userObject = sender;
}

Object* Node::getUserObject()const
{
	return _userObject;
}
//-------------------------------------Action--------------------------------
void Node::runAction(Action*action)
{
	_actionManager->addAction(action,this,!_running);
}
void Node::stopAllActions()
{
	_actionManager->removeAllActionsFromTarget(this);
}
Action*Node::getActionByTag(int tag)
{
	if(tag != -1)
		return _actionManager->getActionByTag(tag,this);
	return nullptr;
}
void Node::stopActionByTag(int tag)
{
	_actionManager->removeActionByTag(tag,this);
}
void Node::stopAction(Action*action)
{
	_actionManager->removeAction(action);
}
//----------------------------计时器相关-------------------------------
void Node::scheduleUpdate()
{
	_scheduler->schedule([&](float dt){this->update(dt);},this,"update"+_uniqueID,0,SDL_REPEAT_FOREVER,0,!_running);
}
void Node::unscheduleUpdate()
{
	_scheduler->unschedule(this,"update"+_uniqueID);
}
void Node::schedule(const ccScheduleCallback&callback,const std::string& key,float interval,unsigned int repeat,float delay)
{
	_scheduler->schedule(callback,this,key,interval,repeat,delay,!_running);
}
void Node::schedule(SEL_SCHEDULE selector,float interval,unsigned int repeat,float delay)
{
	_scheduler->schedule(selector,this,interval,repeat,delay,!_running);
}
void Node::unschedule(const std::string&key)
{
	_scheduler->unschedule(this,key);
}
void Node::unschedule(SEL_SCHEDULE selector)
{
	_scheduler->unschedule(this,selector);
}
void Node::scheduleOnce(const ccScheduleCallback&callback,const std::string&key,float delay)
{
	_scheduler->schedule(callback,this,key,0,1,delay,!_running);
}
void Node::scheduleOnce(SEL_SCHEDULE selector,float delay)
{
	_scheduler->schedule(selector,this,0,1,delay,!_running);
}
void Node::unscheduleAllCallbacks()
{
	_scheduler->unschedulerForTarget(this);
}
//----------------------------------监听器--------------------------------
void Node::removeAllListeners()
{
	_eventDispatcher->removeEventListenerForTarget(this);
}
NS_SDL_END
