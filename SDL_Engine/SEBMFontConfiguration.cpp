#include "SEBMFontConfiguration.h"
#include "SEFileUtils.h"
#include "SEDirector.h"
#include "SETextureCache.h"

NS_SDL_BEGIN
std::map<std::string, BMFontConfiguration*>* BMFontConfiguration::g_pConfigurations = nullptr;

BMFontDef::BMFontDef()
	:x(0)
	,y(0)
	,width(0)
	,height(0)
	,xoffset(0)
	,yoffset(0)
	,xadvance(0)
	,page(0)
{
}

BMFontDef::BMFontDef(int x, int y, int width, int height, int xoffset, int yoffset, int xadvance, int page)
	:x(x)
	,y(y)
	,width(width)
	,height(height)
	,xoffset(xoffset)
	,yoffset(yoffset)
	,xadvance(xadvance)
	,page(page)
{
}


BMFontConfiguration::BMFontConfiguration()
	:_pBitmapFontMap(nullptr)
	,_commonHeight(0)
{
}

BMFontConfiguration::~BMFontConfiguration()
{
	SDL_SAFE_DELETE(_pBitmapFontMap);
}

BMFontConfiguration* BMFontConfiguration::createWithFile(const std::string& fontFile)
{
	BMFontConfiguration* pRet = nullptr;
	//容器为空，则新建容器
	if (g_pConfigurations == nullptr)
	{
		g_pConfigurations = new std::map<std::string, BMFontConfiguration*>();
	}
	//在容器中查找是否存在对应的文件
	auto it = g_pConfigurations->find(fontFile);

	if (it != g_pConfigurations->end())
	{
		pRet = it->second;
	}
	else
	{
		pRet = BMFontConfiguration::configurationWithFNTFile(fontFile);
		//插入并引用
		g_pConfigurations->insert(std::make_pair(fontFile, pRet));
		SDL_SAFE_RETAIN(pRet);
	}
	return pRet;
}

void BMFontConfiguration::removeFNTCache()
{
	if (g_pConfigurations != nullptr)
	{
		for (auto it = g_pConfigurations->begin(); it != g_pConfigurations->end();)
		{
			auto config = it->second;
			SDL_SAFE_RELEASE(config);

			it = g_pConfigurations->erase(it);
		}
		SDL_SAFE_DELETE(g_pConfigurations);
	}
}

BMFontConfiguration* BMFontConfiguration::configurationWithFNTFile(const std::string& fontFile)
{
	auto pRet = new BMFontConfiguration();

	if (pRet && pRet->initWithFNTFile(fontFile))
		pRet->autorelease();
	else
		SDL_SAFE_DELETE(pRet);

	return pRet;
}

bool BMFontConfiguration::initWithFNTFile(const std::string& fontFile)
{
	//解析font文件
	if (fontFile.empty())
		return false;

	if (_pBitmapFontMap == nullptr)
	{
		_pBitmapFontMap = new std::map<unsigned int, BMFontDef>();
	}
	this->parseConfigFile(fontFile);

	return true;
}

void BMFontConfiguration::parseConfigFile(const std::string& fontFile)
{
	//尝试读取数据
	std::unique_ptr<char> uniqueData = std::move(FileUtils::getInstance()->getUniqueDataFromFile(fontFile));
	//获取数据
	std::istringstream in = std::istringstream(uniqueData.get());
	
	std::string line;
	//每次都只处理一行
	while (getline(in, line))
	{
		//进行判断
		if (line.substr(0, strlen("info face")) == "info face")
			this->parseInfoArguments(line);
		else if (line.substr(0, strlen("common lineHeight")) == "common lineHeight")
			this->parseCommonArguments(line);
		else if (line.substr(0, strlen("page id")) == "page id")
			this->parseImageFileName(line, fontFile);
		else if (line.substr(0, strlen("char id")) == "char id")
			this->parseChar(line);
	}
	uniqueData.reset();
}

void BMFontConfiguration::parseInfoArguments(const std::string& text)
{

}

void BMFontConfiguration::parseCommonArguments(const std::string& text)
{
	//获取到height
	auto index1 = text.find("lineHeight=");
	auto index2 = text.find(' ', index1);
	auto len = strlen("lineHeight=");
	auto str = text.substr(index1 + len, index2 - index1 - len);

	_commonHeight = SDL_atoi(str.c_str());
}

void BMFontConfiguration::parseImageFileName(const std::string& text, const std::string& fontFile)
{
	//获取id
	auto index1 = text.find("id=");
	auto index2 = text.find(' ', index1);
	auto len = strlen("id=");
	auto str = text.substr(index1 + len, index2 - index1 - len);
	int id = SDL_atoi(str.c_str());
	//获取文件名称 和以往的不同，这里要去掉双引号
	index1 = text.find("file=\"", index2);
	len = strlen("file=\"");
	index2 = text.find('\"', index1 + len);
	str = text.substr(index1 + len, index2 - index1 - len);
	//获取相对路径
	auto pos = fontFile.find_last_of("/");
	std::string relativePath;

	 if (pos != std::string::npos)
	 {
		 relativePath = fontFile.substr(0, pos + 1);
	 }
	//先尝试加载绝对路径
	auto fullpath = FileUtils::getInstance()->fullPathForFilename(str);
	if (fullpath.empty() && !relativePath.empty())
	{
		fullpath = FileUtils::getInstance()->fullPathForFilename(relativePath + str);
	}
	//进行保存
	_atlasNames.insert(std::make_pair(id, relativePath + str));
	//添加文件
	Director::getInstance()->getTextureCache()->addImage(fullpath, relativePath + str);
}

void BMFontConfiguration::parseChar(const std::string& text)
{
	BMFontDef def;

	//获取id
	auto index1 = text.find("id=");
	auto index2 = text.find(' ', index1);
	auto len = strlen("id=");
	auto str = text.substr(index1 + len, index2 - index1 - len);
	int id = SDL_atoi(str.c_str());
	//获取x
	index1 = text.find("x=", index2);
	index2 = text.find(' ', index1);
	len = strlen("x=");
	str = text.substr(index1 + len, index2 - index1 - len);
	def.x = SDL_atoi(str.c_str());
	//获取y
	index1 = text.find("y=", index2);
	index2 = text.find(' ', index1);
	len = strlen("y=");
	str = text.substr(index1 + len, index2 - index1 - len);
	def.y = SDL_atoi(str.c_str());
	//获取width
	index1 = text.find("width=", index2);
	index2 = text.find(' ', index1);
	len = strlen("width=");
	str = text.substr(index1 + len, index2 - index1 - len);
	def.width = SDL_atoi(str.c_str());
	//获取height
	index1 = text.find("height=", index2);
	index2 = text.find(' ', index1);
	len = strlen("height=");
	str = text.substr(index1 + len, index2 - index1 - len);
	def.height = SDL_atoi(str.c_str());
	//获取xoffset
	index1 = text.find("xoffset=", index2);
	index2 = text.find(' ', index1);
	len = strlen("xoffset=");
	str = text.substr(index1 + len, index2 - index1 - len);
	def.xoffset = SDL_atoi(str.c_str());
	//获取yoffset
	index1 = text.find("yoffset=", index2);
	index2 = text.find(' ', index1);
	len = strlen("yoffset=");
	str = text.substr(index1 + len, index2 - index1 - len);
	def.yoffset = SDL_atoi(str.c_str());
	//获取xadvance
	index1 = text.find("xadvance=", index2);
	index2 = text.find(' ', index1);
	len = strlen("xadvance=");
	str = text.substr(index1 + len, index2 - index1 - len);
	def.xadvance = SDL_atoi(str.c_str());
	//获取page
	index1 = text.find("page=", index2);
	index2 = text.find(' ', index1);
	len = strlen("page=");
	str = text.substr(index1 + len, index2 - index1 - len);
	def.page = SDL_atoi(str.c_str());
	//存储
	_pBitmapFontMap->insert(std::make_pair(id, def));
}

NS_SDL_END
