#ifndef __SDL_Scale9Sprite_H__
#define __SDL_Scale9Sprite_H__
#include <string>
#include "SENode.h"
#include "SERect.h"
#include "SESize.h"
#include "SEProtocols.h"
#include "SESprite.h"

NS_SDL_BEGIN
class Sprite;

class Scale9Sprite : public Node,TextureProtocol
{
private:
	Sprite* _scale9Sprite;
	Sprite* _innerSpriteList[9];
	Rect _capInsets;
	Size _preferredSize;
	bool _modX;
	bool _modY;
public:
	Scale9Sprite();
	~Scale9Sprite();

	static Scale9Sprite* create(Sprite* sprite);
	static Scale9Sprite* create(Sprite* sprite, const Rect& capInsets);

	static Scale9Sprite* create(const std::string& file);
	static Scale9Sprite* create(const std::string& file,const Rect& capInsets);

	bool initWithFile(const std::string& file,const Rect& capInsets);
	bool init(Sprite*sprite,const Rect& capInsets);

	void setPreferredSize(const Size&preferredSize);
	virtual Texture*getTexture() const;
	virtual void setTexture(Texture*pTexture);
};
NS_SDL_END
#endif
