#include "SEObject.h"
#include "SEPoolManager.h"

NS_SDL_BEGIN
Object::Object()
	:_uniqueID(0)
{
	//定义一个静态变量作为实例对象引用器，每个对象唯一
	static unsigned int uObjectCount = 0;
	uObjectCount++;
	//当类进行实例化时，设置为1
	_referenceCount = 1;
	_managed = false;
	//log("构造函数\n");
	_uniqueID = uObjectCount;
}

bool Object::init()
{
	return true;
}

Object::~Object()
{
	//被自动释放管理器管理，调用内存管理器
	if(_managed)
	{
		PoolManager::getInstance()->removeObject(this);
	}
	//log("析构函数\n");
}

void Object::retain()
{
	SDLASSERT(_referenceCount > 0,"引用数不能小于0");
	++_referenceCount;
}

void Object::release()
{
	SDLASSERT(_referenceCount > 0,"引用数不能小于0");
	--_referenceCount;
	//如果引用计数为0 释放
	if(_referenceCount == 0)
		delete this;
}

Object* Object::autorelease()
{
	//不重复插入
	if(_managed == false)
	{
		//加入自动释放池中
		PoolManager::getInstance()->addObject(this);
		_managed = true;
	}
	return this;
}

NS_SDL_END
