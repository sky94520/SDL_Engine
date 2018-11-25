#include "SETMXObjectGroup.h"
NS_SDL_BEGIN

TMXObjectGroup::TMXObjectGroup()
	:_groupName("")
{
}
TMXObjectGroup::~TMXObjectGroup()
{
	_objects.clear();
}

bool TMXObjectGroup::init()
{
	return true;
}

ValueMap TMXObjectGroup::getObject(const std::string&objectName)const
{
	if (!_objects.empty())
	{
		for (const auto &v : _objects)
		{
			const ValueMap& dict = v.asValueMap();

			if (dict.find("name") != dict.end())
			{
				if (dict.at("name").asString() == objectName)
					return dict;
			}
		}
	}
	return ValueMap();
}

ValueVector& TMXObjectGroup::getObjects()
{
	return _objects;
}

void TMXObjectGroup::addObject(const ValueMap&dict)
{
	_objects.push_back(Value(dict));
}

std::string TMXObjectGroup::getGroupName()const
{
	return _groupName;
}

void TMXObjectGroup::setGroupName(const std::string&groupName)
{
	_groupName = groupName;
}

NS_SDL_END
