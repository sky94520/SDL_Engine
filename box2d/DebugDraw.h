#ifndef __DebugDraw_H__
#define __DebugDraw_H__

#include "../SDL_Engine/SEPlatformMarcos.h"
#include "../SDL_Engine/SERenderCommand.h"

#include "Box2D/Box2D.h"
#include "SDL2_gfxPrimitives.h"

NS_SDL_BEGIN

class DebugDraw:public b2Draw
{
public:
	DebugDraw(uint32 drawFlags,float ptmRatio);
	~DebugDraw();
	/// 绘制一个封闭的多边形，不填充
	virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
	/// 绘制一个封闭的多边形，填充
	virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
	/// 绘制一个圆，不填充
	virtual void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
	/// 绘制一个圆，填充
	virtual void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
	/// 绘制一个线段
	virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
	//绘制b2Transform对象
	virtual void DrawTransform(const b2Transform& xf);
	//绘制点
	virtual void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);
private:
	Uint8 m_alpha;
	float m_fPtmRatio;
	CustomCommand _command;
};
NS_SDL_END
#endif