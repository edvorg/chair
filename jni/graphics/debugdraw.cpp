#include "debugdraw.hpp"

#include <EGL/egl.h>
#include <GLES/gl.h>
#include <cmath>
#include <log.h>
#include "Graphics.hpp"

namespace graphics {

DebugDraw::DebugDraw() {
	SetFlags(
		e_shapeBit
		// | e_jointBit
		// | e_aabbBit
		// | e_pairBit
		// | e_centerOfMassBit
		);
}

void DebugDraw::DrawPolygon(const b2Vec2* v, int32 vertexCount, const b2Color& color) {
}

void DebugDraw::DrawSolidPolygon(const b2Vec2* v, int32 vertexCount, const b2Color& color) {

	if (std::abs(0.900000f - color.r) > 0.1)
		test::drawPoly(v, vertexCount, color, 0.5);
}

void DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) {
}

void DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) {
}

void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {
}

void DebugDraw::DrawTransform(const b2Transform& xf) {
}

}
