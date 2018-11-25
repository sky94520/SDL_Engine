#include "SEScene.h"
#include "SEDirector.h"
NS_SDL_BEGIN
Scene::Scene()
{
}

Scene::~Scene()
{
}

bool Scene::init()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	setContentSize(visibleSize);
	//设置锚点为0，0
	this->setAnchorPoint(Point(0.f,0.f));

	return true;
}

NS_SDL_END
