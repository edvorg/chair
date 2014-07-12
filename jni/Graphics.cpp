// ASTEROIDS_LICENSE_BEGIN
/*

  asteroids GPL Source Code
  Copyright (C) 2013 Edward Knyshov

  This file is part of the asteroids GPL Source Code (asteroids Source Code).

  asteroids Source Code is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  asteroids Source Code is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with asteroids Source Code. If not, see <http://www.gnu.org/licenses/>.

*/
// ASTEROIDS_LICENSE_END

#include "Graphics.hpp"

#include <EGL/egl.h>
#include <GLES/gl.h>
#include <math.h>
#include <algorithm>

#include "Bullet.hpp"
#include "DecorateStar.hpp"
#include "Asteroid.hpp"
#include "Player.hpp"
#include "LevelProgress.hpp"

#include <stack>

namespace test {

  void SetTranslate(float x, float y) {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x, y, 0);
  }

  void SetProjection(float fieldWidth, float fieldHeight) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrthof(0, fieldWidth, 0, fieldHeight, 1, -1);
  }

  void DrawNumber(bool fromLeft, float x, float y, float sizex, float sizey, unsigned int number) {
	if (fromLeft) {
	  std::stack<unsigned int> digits;

	  do {
		digits.push(number % 10);
		number /= 10;
	  } while (number);

	  while (digits.size()) {
		DrawDigit(x, y, sizex, sizey, digits.top());
		x += sizex * 2.5f;
		digits.pop();
	  }
	}
	else {
	  do {
		DrawDigit(x, y, sizex, sizey, number % 10);
		x -= sizex * 2.5f;
		number /= 10;
	  } while (number);
	}
  }

  void DrawDigit(float x, float y, float sizex, float sizey, unsigned int digit) {

	int corners = 0;
	void * vertices = nullptr;
	void * indices = nullptr;

	if (digit == 0) {
	  static const int corn = 5;
	  static GLfloat verts[corn * 2] = {
		- 1.0f, - 1.0f,
		- 1.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, - 1.0f,
		- 1.0f, - 1.0f,
	  };
	  static GLubyte inds[corn] = {
		0, 1, 2, 3, 4
	  };
	  corners = corn;
	  vertices = verts;
	  indices = inds;
	}
	else if (digit == 1) {
	  static const int corn = 2;
	  static GLfloat verts[corn * 2] = {
		1.0f, 1.0f,
		1.0f, - 1.0f,
	  };
	  static GLubyte inds[corn] = {
		0, 1,
	  };
	  corners = corn;
	  vertices = verts;
	  indices = inds;
	}
	else if (digit == 2) {
	  static const int corn = 6;
	  static GLfloat verts[corn * 2] = {
		- 1.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		- 1.0f, 0.0f,
		- 1.0f, - 1.0f,
		1.0f, - 1.0f,
	  };
	  static GLubyte inds[corn] = {
		0, 1, 2, 3, 4, 5,
	  };
	  corners = corn;
	  vertices = verts;
	  indices = inds;
	}
	else if (digit == 3) {
	  static const int corn = 7;
	  static GLfloat verts[corn * 2] = {
		- 1.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		-1.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, -1.0f,
		- 1.0f, - 1.0f,
	  };
	  static GLubyte inds[corn] = {
		0, 1, 2, 3, 4, 5, 6,
	  };
	  corners = corn;
	  vertices = verts;
	  indices = inds;
	}
	else if (digit == 4) {
	  static const int corn = 5;
	  static GLfloat verts[corn * 2] = {
		- 1.0f, 1.0f,
		- 1.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, -1.0f,
	  };
	  static GLubyte inds[corn] = {
		0, 1, 2, 3, 4,
	  };
	  corners = corn;
	  vertices = verts;
	  indices = inds;
	}
	else if (digit == 5) {
	  static const int corn = 6;
	  static GLfloat verts[corn * 2] = {
		1.0f, 1.0f,
		- 1.0f, 1.0f,
		- 1.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, - 1.0f,
		- 1.0f, - 1.0f,
	  };
	  static GLubyte inds[corn] = {
		0, 1, 2, 3, 4, 5,
	  };
	  corners = corn;
	  vertices = verts;
	  indices = inds;
	}
	else if (digit == 6) {
	  static const int corn = 7;
	  static GLfloat verts[corn * 2] = {
		1.0f, 1.0f,
		- 1.0f, 1.0f,
		- 1.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, - 1.0f,
		- 1.0f, - 1.0f,
		- 1.0f, 0.0f,
	  };
	  static GLubyte inds[corn] = {
		0, 1, 2, 3, 4, 5, 6,
	  };
	  corners = corn;
	  vertices = verts;
	  indices = inds;
	}
	else if (digit == 7) {
	  static const int corn = 3;
	  static GLfloat verts[corn * 2] = {
		- 1.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, - 1.0f,
	  };
	  static GLubyte inds[corn] = {
		0, 1, 2,
	  };
	  corners = corn;
	  vertices = verts;
	  indices = inds;
	}
	else if (digit == 8) {
	  static const int corn = 8;
	  static GLfloat verts[corn * 2] = {
		- 1.0f, 0.0f,
		- 1.0f, - 1.0f,
		1.0f,- 1.0f,
		1.0f, 0.0f,
		- 1.0f, 0.0f,
		- 1.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
	  };
	  static GLubyte inds[corn] = {
		0, 1, 2, 3, 4, 5, 6, 7,
	  };
	  corners = corn;
	  vertices = verts;
	  indices = inds;
	}
	else if (digit == 9) {
	  static const int corn = 7;
	  static GLfloat verts[corn * 2] = {
		- 1.0f, - 1.0f,
		1.0f,- 1.0f,
		1.0f, 0.0f,
		- 1.0f, 0.0f,
		- 1.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
	  };
	  static GLubyte inds[corn] = {
		0, 1, 2, 3, 4, 5, 6,
	  };
	  corners = corn;
	  vertices = verts;
	  indices = inds;
	}

	// render
	if (corners && vertices && indices) {
	  glMatrixMode(GL_MODELVIEW);
	  glPushMatrix();
	  glTranslatef(x, y, 0);
	  glScalef(sizex, sizey, 1);
	  glEnableClientState(GL_VERTEX_ARRAY);
	  glVertexPointer(2, GL_FLOAT, 0, vertices);
	  glDrawElements(GL_LINE_STRIP, corners, GL_UNSIGNED_BYTE, indices);
	  glDisableClientState(GL_VERTEX_ARRAY);
	  glPopMatrix();
	}
  }

  void DrawPlayer(const Player & player) {
	// TODO rewrite using vertex buffer and array objects
	static const int corners = 3;
	static GLfloat vertices[corners * 2] = {
	  0.75, -1.0,
	  0.0, 1.0,
	  -0.75, -1.0,
	};
	static GLubyte indices[corners] = {
	  0, 1, 2
	};

	if (player.IsSpawned()) {
	  // render
	  glMatrixMode(GL_MODELVIEW);
	  glPushMatrix();
	  glTranslatef(player.GetX(), player.GetY(), 0);
	  glRotatef(player.GetAngle(), 0, 0, 1);
	  glScalef(player.GetSize(), player.GetSize(), 1);
	  glEnableClientState(GL_VERTEX_ARRAY);
	  glVertexPointer(2, GL_FLOAT, 0, vertices);
	  glDrawElements(GL_LINE_LOOP, corners, GL_UNSIGNED_BYTE, indices);
	  glDisableClientState(GL_VERTEX_ARRAY);
	  glPopMatrix();
	}
  }
  void DrawAsteroid(const Asteroid & asteroid) {
	// TODO rewrite using vertex buffer and array objects
	static const int corners = 8;
	static GLfloat vertices[corners * 2];
	static GLubyte indices[corners];
	static bool verticesInited = false;

	if (!verticesInited) {
	  float angle = 0;
	  for (unsigned int i = 0; i < corners; i++) {
		vertices[i * 2] = cos(angle);
		vertices[i * 2 + 1] = sin(angle);
		indices[i] = i;
		angle += 2.0 * M_PI / corners;
	  }
	  verticesInited = true;
	}

	GLfloat currentColor[4];
	glGetFloatv(GL_CURRENT_COLOR, currentColor);

	glColor4f(0x94 / 255.0f, 0x8c / 255.0f, 0x75 / 255.0f, 1.0f);

	// render
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(asteroid.GetPosX(), asteroid.GetPosY(), 0);
	glRotatef(asteroid.GetAngle(), 0, 0, 1);
	glScalef(asteroid.GetSize(), asteroid.GetSize(), 1);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glDrawElements(GL_LINE_LOOP, corners, GL_UNSIGNED_BYTE, indices);
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();

	glColor4f(currentColor[0],
			  currentColor[1],
			  currentColor[2],
			  currentColor[3]);
  }
  void DrawDecorateStar(const DecorateStar & star) {
	// TODO rewrite using vertex buffer and array objects
	static const int corners = 1;
	static GLfloat vertices[corners * 2] = {
	  0.0f, 0.0f,
	};
	static GLubyte indices[corners] = {
	  0,
	};

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLfloat currentColor[4];
	GLfloat oldSize;
	glGetFloatv(GL_CURRENT_COLOR, currentColor);
	glGetFloatv(GL_POINT_SIZE, &oldSize);

	glPointSize(star.GetSize());
    glColor4f(0xbe / 255.0f, 0xb4 / 255.0f, 0xfa / 255.0f, 1.0f);

	// render
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(star.GetPosX(), star.GetPosY(), 0);
	glScalef(star.GetSize(), star.GetSize(), 1);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glDrawElements(GL_POINTS, corners, GL_UNSIGNED_BYTE, indices);
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();

	glPointSize(oldSize);
	glColor4f(currentColor[0],
			  currentColor[1],
			  currentColor[2],
			  currentColor[3]);

	glDisable(GL_BLEND);
  }
  void DrawBullet(const Bullet & bullet) {
	// TODO rewrite using vertex buffer and array objects
	static const int corners = 8;
	static GLfloat vertices[corners * 2];
	static GLubyte indices[corners];
	static bool verticesInited = false;

	if (!verticesInited) {
	  float angle = 0;
	  for (unsigned int i = 0; i < corners; i++) {
		vertices[i * 2] = cos(angle);
		vertices[i * 2 + 1] = sin(angle);
		indices[i] = i;
		angle += 2.0 * M_PI / corners;
	  }
	  verticesInited = true;
	}

	// render
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(bullet.GetPosX(), bullet.GetPosY(), 0);
	glRotatef(bullet.GetAngle(), 0, 0, 1);
	glScalef(bullet.GetSize(), bullet.GetSize(), 1);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glDrawElements(GL_LINE_LOOP, corners, GL_UNSIGNED_BYTE, indices);
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
  }

  void DrawProgress(const LevelProgress & progress) {
	if (progress.IsPaused()) {
	  // TODO rewrite using vertex buffer and array objects
	  static const int corners = 3;
	  static GLfloat vertices[corners * 2] = {
		0.0f, -1.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
	  };
	  static GLubyte indices[corners] = {
		0, 1, 2
	  };

	  GLfloat currentColor[4];
	  GLfloat width;
	  glGetFloatv(GL_CURRENT_COLOR,currentColor);
	  glGetFloatv(GL_LINE_WIDTH, &width);
	  glColor4f(0x34 / 255.0f, 0xbe / 255.0f, 0xda / 255.0f, 1.0f);
	  glLineWidth(progress.playLineWidth);

	  // render
	  glMatrixMode(GL_MODELVIEW);
	  glPushMatrix();
	  glTranslatef(progress.GetParams().fieldWidth * 0.5f,
				   progress.GetParams().fieldHeight * 0.5f,
				   0);
	  glScalef(progress.playButtonSize * progress.GetParams().fieldWidth,
			   progress.playButtonSize * progress.GetParams().fieldWidth, 1);
	  glEnableClientState(GL_VERTEX_ARRAY);
	  glVertexPointer(2, GL_FLOAT, 0, vertices);
	  glDrawElements(GL_LINE_LOOP, corners, GL_UNSIGNED_BYTE, indices);
	  glDisableClientState(GL_VERTEX_ARRAY);
	  glPopMatrix();

	  glColor4f(currentColor[0],
				currentColor[1],
				currentColor[2],
				currentColor[3]);
	  glLineWidth(width);
	}
	else {
	  // TODO rewrite using vertex buffer and array objects
	  static const int corners = 2;
	  static GLfloat vertices[corners * 2] = {
		0.0f, 0.0f,
		0.0f, 1.0f,
	  };
	  static GLubyte indices[corners] = {
		0, 1,
	  };

	  GLfloat currentColor[4];
	  GLfloat width;
	  glGetFloatv(GL_CURRENT_COLOR,currentColor);
	  glGetFloatv(GL_LINE_WIDTH, &width);
	  glColor4f(0x34 / 255.0f, 0xbe / 255.0f, 0xda / 255.0f, 1.0f);
	  glLineWidth(progress.lineWidth);

	  // render
	  glMatrixMode(GL_MODELVIEW);
	  glPushMatrix();
	  glTranslatef(progress.GetParams().fieldWidth - progress.lineWidth / progress.GetParams().fieldWidth,
				   0,
				   0);
	  glScalef(1, progress.GetLevelTimer() / progress.GetLevelTime() * progress.GetParams().fieldHeight, 1);
	  glEnableClientState(GL_VERTEX_ARRAY);
	  glVertexPointer(2, GL_FLOAT, 0, vertices);
	  glDrawElements(GL_LINE_LOOP, corners, GL_UNSIGNED_BYTE, indices);
	  glDisableClientState(GL_VERTEX_ARRAY);
	  glPopMatrix();

	  glColor4f(currentColor[0],
				currentColor[1],
				currentColor[2],
				currentColor[3]);
	  glLineWidth(width);
	}
  }

}
