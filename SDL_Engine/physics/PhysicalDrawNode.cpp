#include "PhysicalDrawNode.h"
#include "DebugDraw.h"

NS_SDL_BEGIN

PhysicalDrawNode::PhysicalDrawNode()
:_pWorld(nullptr)
{
}

PhysicalDrawNode::~PhysicalDrawNode()
{
	SDL_SAFE_DELETE(_pDebugDraw);
}

PhysicalDrawNode*PhysicalDrawNode::create(b2World*pWorld,float ptmRatio)
{
	auto node = new PhysicalDrawNode();

	if (node && node->init(pWorld,ptmRatio))
		node->autorelease();
	else 
		SDL_SAFE_DELETE(node);

	return node;
}

bool PhysicalDrawNode::init(b2World*pWorld,float ptmRatio)
{
	_pWorld = pWorld;

	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	/*flags += b2Draw::e_jointBit;
	flags += b2Draw::e_aabbBit;
	flags += b2Draw::e_pairBit;
	flags += b2Draw::e_centerOfMassBit;*/

	_pDebugDraw = new DebugDraw(flags,ptmRatio);

	_pWorld->SetDebugDraw(_pDebugDraw);

	return true;
}

void PhysicalDrawNode::render()
{
	if (_pWorld)
	{
		_pWorld->DrawDebugData();
	}
}
NS_SDL_END