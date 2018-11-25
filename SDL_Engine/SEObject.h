#ifndef __Object_H__
#define __Object_H__

#include "SEPlatformMarcos.h"
NS_SDL_BEGIN
class AutoreleasePool;
/*拷贝接口*/
class Clonable
{
public:
	virtual Clonable* clone() const = 0;
	//添加析构函数，不添加可能会发生内存泄漏
	virtual ~Clonable(){}
};
//所有类的基类
class Object
{
protected:
	//引用计数器
	unsigned int _referenceCount;
	//是否由内存管理器管理
	bool _managed;
	unsigned int _uniqueID;
public:
	CREATE_FUNC(Object);
	bool init();
	Object();
	virtual ~Object();
	//保留
	void retain();
	//释放
	void release();
	//自动释放
	Object*autorelease();
	//获得引用数量
	unsigned int getReferenceCount() const { return _referenceCount;}
	//获取当前的id
	unsigned int getUniqueID() const { return _uniqueID; }
	//是否交给释放池
	bool isManaged() const { return _managed; }
	//友元
	friend class AutoreleasePool;
};
//成员函数指针
typedef void (Object::*SEL_CallFunc)(void);
typedef void (Object::*SEL_SCHEDULE)(float);
typedef void (Object::*SEL_CallFuncO)(Object*);

#define callfunc_selector(_SELECTOR) static_cast<SDL::SEL_CallFunc>(&_SELECTOR)
#define schedule_selector(_SELECTOR) static_cast<SDL::SEL_SCHEDULE>(&_SELECTOR)
#define scheduleO_selector(_SELECTOR) static_cast<SDL::SEL_CallFuncO>(&_SELECTOR)
NS_SDL_END
#endif
