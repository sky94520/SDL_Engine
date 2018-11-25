#ifndef __SDL_UserDefault_H__
#define __SDL_UserDefault_H__
#include<memory>
#include<fstream>

#include "SEObject.h"
#include "SEPlatformMarcos.h"

NS_SDL_BEGIN
class UserDefault:public Object
{
private:
	std::string _filePath;
	static UserDefault*_pInstance;
private:
	UserDefault();
	~UserDefault();
	bool init();
public:
	static UserDefault*getInstance();
	static void purge();
	//get
	void setValueForKey(const std::string&key,const std::string&value);
	void setBoolForKey(const std::string&key,bool value);
	void setIntegerForKey(const std::string&key,int value);
	void setFloatForKey(const std::string&key,float value);
	void setDoubleForKey(const std::string&key,double value);
	void setStringForKey(const std::string&key,const std::string&value);
	//get
	std::string getValueForKey(const std::string&key); 
	bool getBoolForKey(const std::string&key,bool defaultValue);
	int getIntegerForKey(const std::string&key,int defaultValue);
	float getFloatForKey(const std::string&key,float defaultValue);
	double getDoubleForKey(const std::string&key,double defaultValue);
	std::string getStringForKey(const std::string&key,const std::string&defaultValue);
	//该xml是否存在
	bool isXMLExist();
};
NS_SDL_END
#endif
