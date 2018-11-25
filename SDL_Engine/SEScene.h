#ifndef __SDL_Scene_H__
#define __SDL_Scene_H__

#include "SENode.h"
NS_SDL_BEGIN
class Scene : public Node
{
public:
	Scene();
	~Scene();
	CREATE_FUNC(Scene);
	bool init();
};
NS_SDL_END
#endif
