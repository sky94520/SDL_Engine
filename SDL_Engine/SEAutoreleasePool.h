#ifndef __AutoreleasePool_H__
#define __AutoreleasePool_H__
#include<vector>
#include <algorithm>

#include "SEObject.h"
NS_SDL_BEGIN
class AutoreleasePool : public Object
{
private:
	std::vector<Object*> _managedObjects;
public:
	AutoreleasePool();
	virtual ~AutoreleasePool();
	//insert dirfferent
	void addObject(Object* pObject);
	void removeObject(Object* pObject);
	//清理容器
	void clear();
};
NS_SDL_END
#endif
