#include "SEAutoreleasePool.h"
NS_SDL_BEGIN

AutoreleasePool::AutoreleasePool(void)
{
}

AutoreleasePool::~AutoreleasePool(void)
{
}

void AutoreleasePool::addObject(Object*pObject)
{
	_managedObjects.push_back(pObject);
}

void AutoreleasePool::removeObject(Object* pObject)
{
	//find the iterator
	auto it = std::find(_managedObjects.begin(), _managedObjects.end(), pObject);
	//erase
	if (it != _managedObjects.end())
	{
		it = _managedObjects.erase(it);

		pObject->_managed = false;
	}
}

void AutoreleasePool::clear()
{
	for(auto it = _managedObjects.begin(); it != _managedObjects.end();)
	{
		auto object = *it;
		object->_managed = false;
		object->release();

		it = _managedObjects.erase(it);
	}
}

NS_SDL_END
