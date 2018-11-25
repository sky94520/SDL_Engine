#ifndef __SDL_LabelBMFont_H__
#define __SDL_LabelBMFont_H__
#include <string>
#include "SENode.h"
#include "SEColor.h"
#include "SEProtocols.h"

NS_SDL_BEGIN

class BMFontConfiguration;

class LabelBMFont : public Node, public LabelProtocol
{
private:
	std::string _text;
	BMFontConfiguration* _pConfiguration;
	int _width;
	Color3B _color;
public:
	LabelBMFont();
	~LabelBMFont();

	static LabelBMFont* create(const std::string& text, const std::string& fontFile);
	static LabelBMFont* create(const std::string& text, const std::string& fontFile, int width, const Color3B& color= Color3B(255, 255, 255));

	bool init(const std::string& text, const std::string& fontFile);
	bool init(const std::string& text, const std::string& fontFile, int width, const Color3B& color);
	//颜色
	Color3B getColor() const;
	void setColor(const Color3B& color);

	virtual void setString(const std::string& text);
	virtual std::string getString() const;
private:
	//根据字符串和字体文件创建精灵
	void createFontChars();
};
NS_SDL_END
#endif
