#include "SEFontCache.h"
#include "SEFont.h"

NS_SDL_BEGIN

FontCache* FontCache::_pInstance = nullptr;

FontCache::FontCache()
{
}

FontCache::~FontCache()
{
	for (auto it = _fonts.begin(); it != _fonts.end();)
	{
		auto font = it->second;

		SDL_SAFE_RELEASE(font);

		it = _fonts.erase(it);
	}
}

bool FontCache::init()
{
	return true;
}

FontCache* FontCache::getInstance()
{
	if (_pInstance == nullptr)
	{
		_pInstance = new FontCache();
		_pInstance->init();
	}
	return _pInstance;
}

void FontCache::purge()
{
	SDL_SAFE_RELEASE_NULL(_pInstance);
}

Font* FontCache::addFont(const std::string& filename, int fontSize)
{
	//检测是否已经加载
	auto it = _fonts.find(filename);

	if (it != _fonts.end())
		return it->second;
	//加载字体
	Font* font = Font::create(filename.c_str(), fontSize);
	SDL_SAFE_RETAIN(font);

	_fonts.insert(std::make_pair(filename,font));

	return font;
}

Font* FontCache::getFont(const std::string& filename, int fontSize)
{
	//TODO 目前不能动态更改字体
	return this->addFont(filename, fontSize);
}

NS_SDL_END
