#ifndef __SDL_LoadingBar_H__
#define __SDL_LoadingBar_H__

#include "SENode.h"
#include "SETexture.h"
#include "SERect.h"
NS_SDL_BEGIN
enum class LoadingBarDir
{
	kLoadingBar_Left,//从左至右
	kLoadingBar_Right,//从右至左
	kLoadingBar_Up,
	kLoadingBar_Down
};

class Sprite;

class LoadingBar : public Node
{
private:
	Sprite* _sprite;
	//[0,1]
	float _percent;
	LoadingBarDir _dir;
	Rect _rect;
public:
	LoadingBar();
	virtual ~LoadingBar();

	static LoadingBar* create(Sprite* sprite, float percent = 1.f, LoadingBarDir dir = LoadingBarDir::kLoadingBar_Left);
	bool init(Sprite* sprite, float percent, LoadingBarDir dir);
		
	void setPercent(float percent)
	{
		_percent = percent;
	}
	float getPercent() const
	{
		return _percent;
	}
	//重写draw函数
	virtual void draw();
};
NS_SDL_END
#endif
