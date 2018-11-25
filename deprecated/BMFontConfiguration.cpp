#include "BMFontConfiguration.h"
#include "FileUtils.h"
#include "Director.h"
#include "TextureCache.h"

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
	auto text = uniqueData.get();
	
	rapidxml::xml_document<> doc;
	//解析资源
	doc.parse<0>(text);
	//获取根节点
	auto root_node = doc.first_node();
	//进行遍历
	for (auto node = root_node->first_node(); node != nullptr; node = node->next_sibling())
	{
		auto name = node->name();

		if (strcmp(name, "info") == 0)
			this->parseInfoArguments(node);
		else if (strcmp(name, "common") == 0)
			this->parseCommonArguments(node);
		else if (strcmp(name, "pages") == 0)
			this->parseImageFileName(node, fontFile);
		else if (strcmp(name, "chars") == 0)
			this->parseChars(node);
	}
	uniqueData.reset();
}

void BMFontConfiguration::parseInfoArguments(rapidxml::xml_node<char>* root)
{

}

void BMFontConfiguration::parseCommonArguments(rapidxml::xml_node<char>* root)
{
	//获取到height
	char* str = root->first_attribute("lineHeight")->value();

	_commonHeight = SDL_atoi(str);
}

void BMFontConfiguration::parseImageFileName(rapidxml::xml_node<char>* root, const std::string& fontFile)
{
	//获取相对路径
	auto pos = fontFile.find_last_of("/");
	std::string relativePath;

	 if (pos != std::string::npos)
	 {
		 relativePath = fontFile.substr(0, pos + 1);
	 }

	for (auto node = root->first_node(); node != nullptr; node = node->next_sibling())
	{
		int id = SDL_atoi(node->first_attribute("id")->value());
		//TODO
		auto file = node->first_attribute("file")->value();
		//先尝试加载绝对路径
		auto fullpath = FileUtils::getInstance()->fullPathForFilename(file);
		
		if (fullpath.empty() && !relativePath.empty())
		{
			fullpath = FileUtils::getInstance()->fullPathForFilename(relativePath + file);
		}
		//进行保存
		_atlasNames.insert(std::make_pair(id, relativePath + file));
		//添加文件
		Director::getInstance()->getTextureCache()->addImage(fullpath, relativePath + file);
	}
}

void BMFontConfiguration::parseChars(rapidxml::xml_node<char>* root)
{
	BMFontDef def;

	for (auto node = root->first_node(); node != nullptr; node = node->next_sibling())
	{
		//获取各种属性
		int id = SDL_atoi(node->first_attribute("id")->value());

		def.x = SDL_atoi(node->first_attribute("x")->value());
		def.y = SDL_atoi(node->first_attribute("y")->value());
		def.width = SDL_atoi(node->first_attribute("width")->value());
		def.height = SDL_atoi(node->first_attribute("height")->value());
		def.xoffset = SDL_atoi(node->first_attribute("xoffset")->value());
		def.yoffset = SDL_atoi(node->first_attribute("yoffset")->value());
		def.xadvance = SDL_atoi(node->first_attribute("xadvance")->value());
		def.page = SDL_atoi(node->first_attribute("page")->value());
		//存储
		_pBitmapFontMap->insert(std::make_pair(id, def));
	}
}

NS_SDL_END
