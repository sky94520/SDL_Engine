#ifndef __SDL_Layer_H__
#define __SDL_Layer_H__
#include "SDL.h"
#include "SENode.h"
NS_SDL_BEGIN

class Touch;
/*暂时和Scene相同，为了以后扩展*/
class Layer : public Node
{
protected:
public:
	Layer();
	virtual ~Layer();
	
	CREATE_FUNC(Layer);
	bool init();
	//留给子类扩展
	virtual bool onTouchBegan(Touch* touch, SDL_Event* event);
	virtual void onTouchMoved(Touch* touch, SDL_Event* event);
	virtual void onTouchEnded(Touch* touch, SDL_Event* event);
	virtual void onTouchCancelled(Touch* touch, SDL_Event* event);

	virtual void onTouchesBegan(std::vector<Touch*> touches, SDL_Event* event);
	virtual void onTouchesMoved(std::vector<Touch*> touches, SDL_Event* event);
	virtual void onTouchesEnded(std::vector<Touch*> touches, SDL_Event* event);
	virtual void onTouchesCancelled(std::vector<Touch*> touches, SDL_Event* event);

	virtual void onKeyPressed(SDL_Keycode keyCode, SDL_Event* event);
	virtual void onKeyReleased(SDL_Keycode keyCode, SDL_Event* event);
};
NS_SDL_END
#endif
