#ifndef __SDL_Protocols_H__
#define __SDL_Protocols_H__
#include<string>
#include "SEPlatformMarcos.h"
#include "SETexture.h"
NS_SDL_BEGIN
/*各种各样的协议，接口*/
/**/
class TextureProtocol
{
public:
	virtual ~TextureProtocol(){}
	virtual Texture*getTexture() const = 0;
	virtual void setTexture(Texture*pTexture) = 0;
};
/**/
class LabelProtocol
{
public:
	virtual ~LabelProtocol(){}
	virtual void setString(const std::string&label)=0;
	virtual std::string getString()const=0;
};
NS_SDL_END
#endif
