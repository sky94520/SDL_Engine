#ifndef __SDL_FileUtils_H__
#define __SDL_FileUtils_H__

#include<string>
#include<map>
#include<vector>
#include<memory>
#include<stack>
#include<algorithm>
#include<stdio.h>

#include "SDL.h"
#include "SEObject.h"
#include "SEValue.h"

#include "../rapidxml/rapidxml.hpp"
#include "../rapidxml/rapidxml_print.hpp"

NS_SDL_BEGIN
class FileUtils : public Object
{
private:
	static FileUtils* _pInstance;
	//存放着key所对应的文件路径
	std::map<std::string,std::string> _fullPathCache;
	//搜寻路径集合
	std::vector<std::string> _searchPathArray;
	//组织名 在SDL_GetPrefPath()会起作用
	std::string _org;
	std::string _app;
private:
	FileUtils();
	~FileUtils();
public:
	static FileUtils* getInstance()
	{
		if(_pInstance == nullptr)
		{
			_pInstance = new FileUtils();
		}
		return _pInstance;
	}
	static void purge();

	std::string fullPathForFilename(const std::string& filename);
	//添加搜寻路径
	void addSearchPath(const std::string& path);
	//是否是绝对路径
	bool isAbsolutePath(const std::string& filename) const;
	//文件是否存在
	bool isFileExist(const std::string& filename) const;
	//获取文件内容
	std::string getDataFromFile(const std::string& filename);
	//使用键值对获取 plist
	ValueMap getValueMapFromFile(const std::string& plist);
	ValueVector getValueVectorFromFile(const std::string& plist);
	//写文件
	bool writeValueMapToFile(ValueMap& dict,const std::string& fullpath);
	bool writeValueVectorToFile(ValueVector& array,const std::string& fullpath);
	//获取 数据
	std::unique_ptr<char> getUniqueDataFromFile(const std::string& filename, size_t* sz = nullptr);
	//获取可写路径(可以认为这是唯一的写的路径)
	std::string getWritablePath() const;
	//org app
	void setOrgString(const std::string&org) { _org = org; }
	std::string getOrgString() const { return _org; }
	void setAppString(const std::string&app) { _app = app; }
	std::string getAppString() const { return _app; }
private:
	std::string getFullPath(const std::string& filename,const std::string& path) const;
	rapidxml::xml_node<>* generateElementForArray(ValueVector& array,rapidxml::xml_document<>& doc);
	rapidxml::xml_node<>* generateElementForDict(ValueMap& dict,rapidxml::xml_document<>& doc);
	rapidxml::xml_node<>* generateElementForObject(const Value& value,rapidxml::xml_document<>& doc);
};
NS_SDL_END
#endif
