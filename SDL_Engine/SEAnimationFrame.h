#ifndef __SDL_AnimationFrame_H__
#define __SDL_AnimationFrame_H__
#include "SEObject.h"
#include "SEValue.h"

NS_SDL_BEGIN

class SpriteFrame;
class Node;

class AnimationFrame:public Object
{
public:
	struct DisplayedEventInfo
	{
		Node* target;
		const ValueMap* userInfo;
	};
private:
	SpriteFrame*_spriteFrame;
	float _delayUnits;
	ValueMap _userInfo;
public:
	AnimationFrame();
	~AnimationFrame();
	static AnimationFrame*create(SpriteFrame*spriteFrame,float delayUnits);
	bool init(SpriteFrame*spriteFrame,float delayUnits);

	void setSpriteFrame(SpriteFrame*spriteFrame);
	SpriteFrame*getSpriteFrame()const;

	void setDelayUnits(float delayUnits);
	float getDelayUnits()const;
	//get user information
	const ValueMap& getUserInfo()const { return _userInfo;}
	ValueMap& getUserInfo() { return _userInfo;}
	//set user information
	void setUserInfo(const ValueMap&userInfo)
	{
		_userInfo = userInfo;
	}
};
NS_SDL_END
#endif
