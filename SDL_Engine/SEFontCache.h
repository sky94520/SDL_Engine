#ifndef __SDL_FontCache_H__
#define __SDL_FontCache_H__
#include <map>
#include <string>
#include "SEObject.h"

NS_SDL_BEGIN;
class Font;

class FontCache : public Object
{
private:
	static FontCache* _pInstance;
private:
	std::map<std::string, Font*> _fonts;
private:
	FontCache();
	~FontCache();

	bool init();
public:
	static FontCache* getInstance();
	static void purge();
public:
	//添加字体，并保存
	Font* addFont(const std::string& filename, int fontSize);
	//获得对应字符
	Font* getFont(const std::string& filename, int fontSize);
};
NS_SDL_END
#endif
