#include "debugdraw.hpp"

#include <EGL/egl.h>
#include <GLES/gl.h>
#include <cmath>
#include <log.h>

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
	GLfloat currentColor[4];
	glGetFloatv(GL_CURRENT_COLOR, currentColor);
	glColor4f(0x34 / 255.0f, 0xbe / 255.0f, 0xda / 255.0f, 1.0f);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, v);
	glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
	glDisableClientState(GL_VERTEX_ARRAY);

	glColor4f(currentColor[0],
			  currentColor[1],
			  currentColor[2],
			  currentColor[3]);
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
