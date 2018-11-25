#ifndef __SDL_TMXObjectGroup_H__
#define __SDL_TMXObjectGroup_H__
#include<map>
#include<vector>
#include<string>
#include<algorithm>

#include "SEObject.h"
#include "SEValue.h"

#include "../rapidxml/rapidxml.hpp"

NS_SDL_BEGIN

class TMXObjectGroup:public Object
{
private:
	std::string _groupName;
	ValueVector _objects;
public:
	TMXObjectGroup();
	~TMXObjectGroup();
	CREATE_FUNC(TMXObjectGroup);
	bool init();
	//获取指定名称的对象
	ValueMap getObject(const std::string&objectName)const;
	//获取所有对象
	ValueVector& getObjects();

	void addObject(const ValueMap&dict);

	std::string getGroupName()const;
	void setGroupName(const std::string&groupName);
};
NS_SDL_END
#endif
