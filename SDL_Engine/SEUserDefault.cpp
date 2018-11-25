#include "SEUserDefault.h"
#include "../rapidxml/rapidxml.hpp"
#include "../rapidxml/rapidxml_utils.hpp"
#include "../rapidxml/rapidxml_print.hpp"

#include "SEValue.h"
#include "SEFileUtils.h"
NS_SDL_BEGIN

UserDefault*UserDefault::_pInstance = nullptr;
UserDefault::UserDefault()
{
}
UserDefault::~UserDefault()
{
}
UserDefault*UserDefault::getInstance()
{
	if(_pInstance == nullptr)
	{
		_pInstance = new UserDefault();
		_pInstance->init();
	}
	return _pInstance;
}
void UserDefault::purge()
{
	SDL_SAFE_DELETE(_pInstance);
}
bool UserDefault::init()
{
	_filePath = "save.xml";
	
	return true;
}
void UserDefault::setValueForKey(const std::string&key,const std::string&value)
{
	rapidxml::xml_document<> doc;
	//获取完整路径
	std::string filePath = FileUtils::getInstance()->getWritablePath() + _filePath;
	std::unique_ptr<char> text_ptr = std::move(FileUtils::getInstance()->getUniqueDataFromFile(filePath));

	rapidxml::xml_node<>*root = nullptr;
	rapidxml::xml_node<>*node = nullptr;
	//添加文件头
	rapidxml::xml_node<>*head = doc.allocate_node(rapidxml::node_pi,
	doc.allocate_string("xml version='1.0' encoding='UTF-8'"));
	doc.append_node(head);
	//文件存在，先创建
	if(text_ptr != nullptr)
	{
		doc.parse<0>(text_ptr.get());
		//获取root节点
		root = doc.first_node();
		//是否存在对应key
		node = root->first_node(key.c_str());
		//存在 删除后覆盖
		if(node)
		{
			root->remove_node(node);
			node = nullptr;
		}
		node = doc.allocate_node(rapidxml::node_element,key.c_str(),value.c_str());
		//覆盖该值
		root->append_node(node);
	}
	else//不存在该文件 新建
	{
		//根节点
		root = doc.allocate_node(rapidxml::node_element,"root");
		doc.append_node(root);
		//分配该值
		node = doc.allocate_node(rapidxml::node_element,key.c_str(),value.c_str());
		//覆盖该值
		root->append_node(node);
	}
	std::string text;
	if(text_ptr)
		text += "<?xml version='1.0' encoding='UTF-8'?>\n";

	rapidxml::print(std::back_inserter(text),doc,0);
	//覆盖文件
	SDL_RWops*fout = SDL_RWFromFile(filePath.c_str(),"w");
	if(fout == nullptr)
		LOG("UserDefault::setValueForKey:%s\n",SDL_GetError());
	else//写入到文件中
	{
		SDL_RWwrite(fout,text.c_str(),text.size(),1);
		SDL_RWclose(fout);
	}
	text_ptr.reset();
}
void UserDefault::setBoolForKey(const std::string&key,bool value)
{
	if(key.empty())
		return;
	std::string text = value == true?"true":"false";
	setValueForKey(key,text);
}
void UserDefault::setIntegerForKey(const std::string&key,int value)
{
	if(key.empty())
		return;
	setValueForKey(key,Value(value).asString());
}
void UserDefault::setFloatForKey(const std::string&key,float value)
{
	if(key.empty())
		return;
	setValueForKey(key,Value(value).asString());
}
void UserDefault::setDoubleForKey(const std::string&key,double value)
{
	if(key.empty())
		return;
	setValueForKey(key,Value(value).asString());
}
void UserDefault::setStringForKey(const std::string&key,const std::string&value)
{
	if(key.empty())
		return;
	setValueForKey(key,value);
}
std::string UserDefault::getValueForKey(const std::string&key)
{
	rapidxml::xml_document<> doc;
	//获取完整路径
	auto filePath = FileUtils::getInstance()->getWritablePath() + _filePath;
	std::unique_ptr<char> text_ptr = std::move(FileUtils::getInstance()->getUniqueDataFromFile(filePath));
	std::string text;

	try
	{
		if(text_ptr != nullptr)
		{
			doc.parse<0>(text_ptr.get());
			//root
			auto root = doc.first_node();
			//找到对应的node
			auto node = root->first_node(key.c_str());
			//获取节点 文本
			if(node)
				text = node->value();
		}
	}
	catch(std::runtime_error e)
	{
	}
	//清除缓存
	text_ptr.reset();

	return text;
}
bool UserDefault::getBoolForKey(const std::string&key,bool defaultValue)
{
	auto text = getValueForKey(key);
	if(text.empty())
		return defaultValue;
	if(text == "true")
		return true;
	else if(text == "false")
		return false;
	return false;
}
int UserDefault::getIntegerForKey(const std::string&key,int defaultValue)
{
	auto text = getValueForKey(key);
	if(text.empty())
		return defaultValue;
	Value value = Value(text);
	return value.asInt();
}
float UserDefault::getFloatForKey(const std::string&key,float defaultValue)
{
	auto text = getValueForKey(key);
	if(text.empty())
		return defaultValue;
	Value value = Value(text);
	return value.asFloat();
}
double UserDefault::getDoubleForKey(const std::string&key,double defaultValue)
{
	auto text = getValueForKey(key);
	if(text.empty())
		return defaultValue;
	Value value = Value(text);
	return value.asDouble();
}
std::string UserDefault::getStringForKey(const std::string&key,const std::string&defaultValue)
{
	auto text = getValueForKey(key);
	if(text.empty())
		return defaultValue;
	return text;
}
bool UserDefault::isXMLExist()
{
	//获取完整路径
	std::string filePath = FileUtils::getInstance()->getWritablePath() + _filePath;
	if(FileUtils::getInstance()->isFileExist(filePath))
		return true;
	return false;
}
NS_SDL_END
