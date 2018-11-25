#ifndef __SDL_PhysicalDrawNode_H__
#define __SDL_PhysicalDrawNode_H__
#include "../Node.h"
#include "Box2D/Box2D.h"

NS_SDL_BEGIN
class DebugDraw;
class PhysicalDrawNode:public Node
{
private:
	b2World*_pWorld;
	DebugDraw*_pDebugDraw;
public:
	PhysicalDrawNode();
	~PhysicalDrawNode();
	static PhysicalDrawNode*create(b2World*pWorld,float ptmRatio);
	bool init(b2World*pWorld,float ptmRatio);
public:
	virtual void render();
};
NS_SDL_END

#endif