#include "DebugDraw.h"
#include "../SDL_Engine/SERenderer.h"
#include "../SDL_Engine/SEPoint.h"
#include "../SDL_Engine/SEDirector.h"

NS_SDL_BEGIN
DebugDraw::DebugDraw(uint32 drawFlags,float ptmRatio)
{
	m_drawFlags = drawFlags;
	m_fPtmRatio = ptmRatio;

	m_alpha = 180;
}

DebugDraw::~DebugDraw()
{
}

void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	Sint16* vx = new Sint16[vertexCount];
	Sint16* vy = new Sint16[vertexCount];
	//另存为
	memset(vx, 0, sizeof(Sint16) * vertexCount);
	memset(vy, 0, sizeof(Sint16) * vertexCount);
	for(int i = 0;i < vertexCount;i++)
	{
		vx[i] = Sint16(vertices[i].x * m_fPtmRatio);
		vy[i] = Sint16(vertices[i].y * m_fPtmRatio);
	}

	Renderer* renderer = Director::getInstance()->getRenderer();
	//获取颜色
	Uint8 r = Uint8(color.r * 255);
	Uint8 g = Uint8(color.g * 255);
	Uint8 b = Uint8(color.b * 255);

	auto oldColor = renderer->getDrawColor();
	SDL_BlendMode oldBlendMode = SDL_BLENDMODE_NONE;
	renderer->getDrawBlendMode(&oldBlendMode);
	auto render = renderer->getSDL_Renderer();

	filledPolygonRGBA(render,vx,vy,vertexCount,r,g,b,m_alpha);
	//还原现场
	renderer->setDrawBlendMode(oldBlendMode);
	renderer->setDrawColor(oldColor);
}

void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	for(int i=0;i<vertexCount;i++)
		this->DrawSegment(vertices[i],vertices[(i+1)%vertexCount],color);
}

void DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	//auto PTM_RATIO = 32;//PhysicalEngine::getInstance()->PTM_RATIO;
	Renderer*renderer = Director::getInstance()->getRenderer();
	
	Uint8 r = Uint8(color.r * 255);
	Uint8 g = Uint8(color.g * 255);
	Uint8 b = Uint8(color.b * 255);
	Sint16 x = Sint16(center.x *m_fPtmRatio);
	Sint16 y = Sint16(center.y * m_fPtmRatio);
	Sint16 radius1 = Sint16(radius * m_fPtmRatio);
	//保留现场
	auto oldColor = renderer->getDrawColor();
	SDL_BlendMode oldBlendMode = SDL_BLENDMODE_NONE;
	renderer->getDrawBlendMode(&oldBlendMode);
	auto render = renderer->getSDL_Renderer();

	circleRGBA(render,x,y,radius1,r,g,b,m_alpha);
	//还原现场
	renderer->setDrawBlendMode(oldBlendMode);
	renderer->setDrawColor(oldColor);
}

void DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	//auto PTM_RATIO = 32;//PhysicalEngine::getInstance()->PTM_RATIO;
	Renderer*renderer = Director::getInstance()->getRenderer();
	
	Uint8 r = Uint8(color.r * 255);
	Uint8 g = Uint8(color.g * 255);
	Uint8 b = Uint8(color.b * 255);
	Sint16 x = Sint16(center.x *m_fPtmRatio);
	Sint16 y = Sint16(center.y * m_fPtmRatio);
	Sint16 radius1 = Sint16(radius * m_fPtmRatio);
	//保留现场
	auto oldColor = renderer->getDrawColor();
	SDL_BlendMode oldBlendMode = SDL_BLENDMODE_NONE;
	renderer->getDrawBlendMode(&oldBlendMode);
	auto render = renderer->getSDL_Renderer();
	//绘图
	filledCircleRGBA(render,x,y,radius1,r,g,b,m_alpha);
	//还原现场
	renderer->setDrawBlendMode(oldBlendMode);
	renderer->setDrawColor(oldColor);
}

void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	Renderer*renderer = Director::getInstance()->getRenderer();
	//int PTM_RATIO = 32;//PhysicalEngine::getInstance()->PTM_RATIO;

	Uint8 r = Uint8(color.r * 255);
	Uint8 g = Uint8(color.g * 255);
	Uint8 b = Uint8(color.b * 255);

	Sint16 x1 = Sint16(p1.x * m_fPtmRatio);
	Sint16 y1 = Sint16(p1.y * m_fPtmRatio);
	Sint16 x2 = Sint16(p2.x * m_fPtmRatio);
	Sint16 y2 = Sint16(p2.y * m_fPtmRatio);

	auto oldColor = renderer->getDrawColor();
	SDL_BlendMode oldBlendMode = SDL_BLENDMODE_NONE;
	renderer->getDrawBlendMode(&oldBlendMode);
	auto render = renderer->getSDL_Renderer();

	lineRGBA(render,x1,y1,x2,y2,r,g,b,m_alpha);
	//还原现场
	renderer->setDrawBlendMode(oldBlendMode);
	renderer->setDrawColor(oldColor);
}

void DebugDraw::DrawTransform(const b2Transform& xf)
{
}

void DebugDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
}

NS_SDL_END