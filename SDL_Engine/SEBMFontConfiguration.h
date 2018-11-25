#ifndef __SDL_BMFontConfiguration_H__
#define __SDL_BMFontConfiguration_H__

#include <map>
#include <string>
#include <cstring>
#include <sstream>

#include "SEObject.h"
#include "SEPlatformMarcos.h"

NS_SDL_BEGIN

struct BMFontDef
{
public:
	int x;
	int y;
	int width;
	int height;
	int xoffset;
	int yoffset;
	int xadvance;
	int page;
public:
	BMFontDef();
	BMFontDef(int x, int y, int width, int height, int xoffset, int yoffset, int xadvance, int page);
};

class BMFontConfiguration : public Object
{
public:
	//字的编码值和对应信息的映射
	std::map<unsigned int, BMFontDef>* _pBitmapFontMap;
	//使用的文件映射
	std::map<unsigned int, std::string> _atlasNames;
	//FNT文件中的字图公共项Height
	unsigned int _commonHeight;
private:
	static std::map<std::string, BMFontConfiguration*>* g_pConfigurations;
public:
	BMFontConfiguration();
	~BMFontConfiguration();

	static BMFontConfiguration* createWithFile(const std::string& fontFile);
	static void removeFNTCache();

	static BMFontConfiguration* configurationWithFNTFile(const std::string& fontFile);
	bool initWithFNTFile(const std::string& fontFile);
private:
	//解析FNT文件
	void parseConfigFile(const std::string& fontFile);
	//解析信息行
	void parseInfoArguments(const std::string& text);
	//解析公共信息
	void parseCommonArguments(const std::string& text);
	//解析图片名称
	void parseImageFileName(const std::string& text, const std::string& fontFile);
	//解析字符
	void parseChar(const std::string& text);
};
NS_SDL_END
#endif
