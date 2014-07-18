#ifndef GRAPHICS_DEBUGDRAW_HPP
#define GRAPHICS_DEBUGDRAW_HPP

#include <Box2D/Box2D.h>

namespace graphics {

class DebugDraw : public b2Draw {
public:
private:
	virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
	virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
	virtual void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) override;
	virtual void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) override;
	virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;
	virtual void DrawTransform(const b2Transform& xf) override;
};

}

#endif // GRAPHICS_DEBUGDRAW_HPP
