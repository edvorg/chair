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
#include <log.h>

#include "LevelProgress.hpp"

namespace test {

void SetTranslate(float x, float y, bool override) {
	glMatrixMode(GL_MODELVIEW);
	if (override) glLoadIdentity();
	glTranslatef(x, y, 0);
  }

  void SetProjection(float fieldWidth, float fieldHeight) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrthof(0, fieldWidth, 0, fieldHeight, 1, -1);
  }

  float distance(b2Vec2 A, b2Vec2 B) {
	  return sqrt((A.x-B.x)*(A.x-B.x)+(A.y-B.y)*(A.y-B.y));
  }

  std::vector<b2Vec2> sphericize(std::vector<b2Vec2> vertices, float radius, int numPoints) {
	  std::vector<b2Vec2> result;
	  for (auto vertex: vertices) {
		  for (int i=0;i<numPoints; i++) {
			  b2Vec2 point(vertex.x + radius*cos(2*M_PI*i/numPoints),
					       vertex.y + radius*sin(2*M_PI*i/numPoints));
			  result.push_back(point);
		  }
	  }
	  return result;
  }

  std::vector<std::vector<b2Vec2>> findClusters(std::vector<b2Vec2> vertices, float threshold) {
	  std::vector<std::vector<b2Vec2>> clusters;
	  b2Vec2* verticesData = vertices.data();
	  int verticesCount = vertices.size();
	  std::vector<b2Vec2> tempSet;

	  //init colours
	  int* color = new int[vertices.size()];
	  for (int i=0; i<vertices.size(); i++) color[i]=0;

	  //go thru all the vertices
	  int maxColor = 0;
	  for (int i=0; i<verticesCount; i++) {
		  //if the vertex has not been coloured before, colour it into a new colour
		  if (color[i] == 0) {
			  maxColor++;
			  color[i] = maxColor;
		  }

		  for (int j=i+1; j<verticesCount; j++) {
			 //when the two points are near
			 if (distance(verticesData[i],verticesData[j])<threshold) {
				 //colour the second one to the colour of the cluster
				 if (color[j] == 0) {
					 color[j] = color[i];
				 } else {
					 //recolor the whole new coloring
					 int oldColor = color[i];
					 for (int k = 0; k<j; k++)
					 {
						 if (color[k] == oldColor) {
							 color[k] = color[j];
						 }
					 }
					 color[i] = color[j];
				 }
			 }
		  }
	  }

	  //separate vertices into clusters by colour
	  //very much not optimized, to be improved later
	  for (int col = 1; col<=maxColor; col++) {
		  std::vector<b2Vec2> tempSet;
		  for (int i=0; i< verticesCount; i++) {
			  if (color[i] == col)
				  tempSet.push_back(verticesData[i]);
		  }
		  if (tempSet.size()>0) {
			  clusters.push_back(tempSet);
		  }
	  }

	  return clusters;
  }

  float getPointsRadius(std::vector<b2Vec2> vertices) {
	  float radius = 0;
	  if (!vertices.empty()) {
		  float minX, maxX, minY, maxY;
		  minX = maxX = (*vertices.begin()).x;
		  minY = maxY = (*vertices.begin()).y;
		  for (auto vertex: vertices) {
			  if (vertex.x<minX) minX = vertex.x;
			  if (vertex.x>maxX) maxX = vertex.x;
			  if (vertex.y<minY) minY = vertex.y;
			  if (vertex.y>maxY) maxY = vertex.y;
		  }
		  return sqrt((maxX-minX)*(maxX-minX) + (maxY-minY)*(maxY-minY));
	  }
	  return radius;
  }

  std::vector<b2Vec2> smoothSegment(b2Vec2 A, b2Vec2 B, float smoothRatio) {
	  std::vector<b2Vec2> result;
	  b2Vec2 C(A.x + (B.x-A.x)*smoothRatio, A.y + (B.y-A.y)*smoothRatio);
	  result.push_back(C);
	  C.Set(B.x + (A.x-B.x)*smoothRatio, B.y + (A.y-B.y)*smoothRatio);
	  result.push_back(C);
	  return result;
  }
  std::vector<b2Vec2> smoothSurface(std::vector<b2Vec2> vertices, int iterations, float smoothRatio) {
	  std::vector<b2Vec2> answer;
	  if (vertices.size()>1) {
		  auto it1 = vertices.begin();
		  auto it2 = it1+1;
		  while (true) {//it2 != vertices.end()) {
			  std::vector<b2Vec2> smoothed = smoothSegment(*it1, *it2, smoothRatio);
			  answer.insert(answer.end(), smoothed.begin(), smoothed.end());
			  if (it2 == vertices.begin()) break;
			  it1 = it2;
			  it2++;
			  if (it2 == vertices.end()) it2 = vertices.begin();
		  }
	  }
	  if (iterations>1) return smoothSurface(answer, iterations-1, smoothRatio);
	  return answer;
  }

  bool isToTheLeftFromLine(b2Vec2 A, b2Vec2 B, b2Vec2 C) {
	  bool answer = ((B.x - A.x)*(C.y - A.y) - (B.y - A.y)*(C.x - A.x)) > 0;
	  return answer;
  }

  float distanceToTheLine(b2Vec2 A, b2Vec2 B, b2Vec2 C) {
	  float denom = sqrt((B.x-A.x)*(B.x-A.x)+(B.y-A.y)*(B.y-A.y));
	  if (!denom) return 0;
	  float D1 = B.x-A.x;
	  float D2 = B.y-A.y;
	  float top = fabs(D2*C.x-D1*C.y-A.x*B.y+B.x*A.y);
	  if (!top) return 0;
	  float bottom = sqrt(D1*D1+D2*D2);
	  return 10*top/bottom;
  }
  std::vector<b2Vec2> subHull(b2Vec2 A, b2Vec2 B, std::vector<b2Vec2> vertices, std::vector<b2Vec2>* leftovers) {
  	  std::vector<b2Vec2> answer;

  	  float maxDistance = 100;
  	  float* distances = new float[vertices.size()];
  	  if (vertices.empty()) return answer;
  	  float max = 0;
  	  auto maxIterator = vertices.begin();
  	  int i=0;
  	  for (auto vertex = vertices.begin(); vertex!=vertices.end(); vertex++) {
  		  float dist = distanceToTheLine(A,B,*vertex);
  		  distances[i] = dist/maxDistance;
  		  if (dist>max) {
  			  max = dist;
  			  maxIterator = vertex;
  		  }
  		  i++;
  	  }

  	  std::vector<b2Vec2> set1;
  	  std::vector<b2Vec2> set2;

  	  for (auto vertex = vertices.begin(); vertex!=vertices.end(); vertex++) {
  		  if (vertex != maxIterator) {
  			  if (isToTheLeftFromLine(A, *maxIterator, *vertex)) {
  				  set1.push_back(*vertex);
  			  } else if (isToTheLeftFromLine(*maxIterator, B, *vertex)) {
  				  set2.push_back(*vertex);
  			  } else if (leftovers!=nullptr) {
  				  leftovers->push_back(*vertex);
  			  }
  		  }
  	  }

  	  set1 = subHull(A, *maxIterator, set1, leftovers);
  	  set2 = subHull(*maxIterator, B, set2, leftovers);

  	  answer.insert(answer.end(), set2.begin(), set2.end());
  	  answer.push_back(*maxIterator);
  	  answer.insert(answer.end(), set1.begin(), set1.end());
  	  //drawPointsColored(vertices.data(), vertices.size(), distances, 1);
  	  delete[] distances;
  	  return answer;
  }

  std::vector<b2Vec2> quickHull(std::vector<b2Vec2> vertices, std::vector<b2Vec2>* leftovers) {
	  //find left and right
	  std::vector<b2Vec2> answer;

	  if (vertices.size()>0) {
		  float min = (*vertices.begin()).x;
		  float max = (*vertices.begin()).x;
		  auto minIterator = vertices.begin();
		  auto maxIterator = vertices.begin();
		  for (auto vertex = vertices.begin(); vertex!=vertices.end(); vertex++) {
			  if ((*vertex).x<min) {
				  min = (*vertex).x;
				  minIterator = vertex;
			  } else if ((*vertex).x>max) {
				  max = (*vertex).x;
				  maxIterator = vertex;
			  }
		  }
		  answer.push_back(*minIterator);

		  //LOGW("MIN: %d, %d", (int)(*minIterator).x, (int)(*minIterator).y);
		  //LOGW("MAX: %d, %d", (int)(*maxIterator).x, (int)(*maxIterator).y);
		  std::vector<b2Vec2> set1;
		  std::vector<b2Vec2> set2;
		  for (auto vertex = vertices.begin(); vertex!=vertices.end(); vertex++) {
			  if ((vertex != minIterator) && (vertex != maxIterator)) {
				  if (isToTheLeftFromLine(*minIterator, *maxIterator, *vertex))
					  set1.push_back(*vertex);
				  else
					  set2.push_back(*vertex);
			  }
		  }

		  set1 = subHull(*minIterator, *maxIterator, set1, leftovers);  //order?
		  set2 = subHull(*maxIterator, *minIterator, set2, leftovers);

		  /*b2Color color1(0,0.5,0);
		  drawPoly(set1.data(), set1.size(), color1);
		  //answer.insert();
		  color1.Set(0,0,0.7);
		  drawPoly(set2.data(), set2.size(), color1);*/

		  answer.insert(answer.end(), set2.begin(), set2.end());

		  if (minIterator != maxIterator)
		      answer.push_back(*maxIterator);

		  answer.insert(answer.end(), set1.begin(), set1.end());

	  }

	  return answer;
  }

  void drawEllipse(b2Vec2 center, float radius, b2Color color, int numPoints) {
	  b2Vec2* points = new b2Vec2[numPoints];
	  for (int i=0 ;i<numPoints; i++) {
		  points[i].Set(center.x + radius * cos(2*M_PI*i/numPoints), center.y + radius * sin(2*M_PI*i/numPoints));
	  }

	  glColor4f(color.r, color.g, color.b, 1);

	  glEnableClientState(GL_VERTEX_ARRAY);
	  glVertexPointer(2, GL_FLOAT, 0, points);
	  glDrawArrays(GL_TRIANGLE_FAN, 0, numPoints);

	  glDisableClientState(GL_VERTEX_ARRAY);

	  delete[] points;
  }

  void drawPointsColored(const b2Vec2* points, int32 vertexCount, float* color, float radius, int numPoints) {
	  for (int i=0; i<vertexCount; i++) {
		  b2Color color2B(0, color[i], 0);
		  drawEllipse(points[i], radius, color2B, numPoints);
	  }
  }

  void drawPoints(const b2Vec2* points, int32 vertexCount, b2Color color, float radius, int numPoints) {
	  for (int i=0; i<vertexCount; i++) {
		  drawEllipse(points[i], radius, color, numPoints);
	  }
  }

  void drawPoly(const b2Vec2* points, int32 vertexCount, b2Color color, float alpha) {

      /*GLfloat vertices[points.size()*2];
	  int i=0;
	  for (auto point: points) {
		  vertices[i*2] = point.x;
		  vertices[i*2+1] = point.y;
		  i++;
	  }*/

      glEnable(GL_BLEND);
      // glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL__ALPHA);
      glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      // glBlendFunc (GL_ONE, GL_ONE);
	  glColor4f(color.r, color.g, color.b, alpha);

	  /*b2Vec2* points2 = new b2Vec2[vertexCount];
	  for (int i=0; i<vertexCount; i++)
		  points2[vertexCount-i-1] = points[i];*/
	  glEnableClientState(GL_VERTEX_ARRAY);
	  glVertexPointer(2, GL_FLOAT, 0, points);
	  glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);

	  glDisableClientState(GL_VERTEX_ARRAY);
	  //glDrawElements(GL_LINE_LOOP, corners, GL_UNSIGNED_BYTE, indices);*/




        /*static const int corners = 3;
		static GLfloat vertices[corners * 2] = {
			50.0f, 10.0f,
			10.0f, 50.0f,
			50.0f, 45.0f,
		};
		static GLubyte indices[corners] = {
			0, 1, 2
		};

		GLfloat currentColor[4];
		GLfloat width;
		glGetFloatv(GL_CURRENT_COLOR,currentColor);
		glGetFloatv(GL_LINE_WIDTH, &width);
		glColor4f(0x34 / 255.0f, 0xbe / 255.0f, 0xda / 255.0f, 1.0f);
		//glLineWidth(progress.playLineWidth);

		// render
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, vertices);
		glDrawElements(GL_LINE_LOOP, corners, GL_UNSIGNED_BYTE, indices);
		glDisableClientState(GL_VERTEX_ARRAY);

		glColor4f(currentColor[0],
				  currentColor[1],
				  currentColor[2],
				  currentColor[3]);
		glLineWidth(width);*/

      glDisable(GL_BLEND);
  }

void drawCreature(std::vector<b2Vec2> vertices, b2Color color, float alpha) {
	//b2Color color;
	std::vector < std::vector < b2Vec2 >> bodyClusters = findClusters(vertices, 5);
	int i = 0;
	int maxClusters = bodyClusters.size();
	for (auto cluster : bodyClusters) {
		//drawPoints(cluster.data(), cluster.size(),color, 1);
		if (cluster.size() < 4) {
			cluster = sphericize(cluster, 2, 6);
		}
		std::vector<b2Vec2> leftovers;
		std::vector<b2Vec2> outer;
		std::vector<b2Vec2> inner;
		outer = quickHull(cluster, &leftovers);
		inner = quickHull(leftovers);

		outer = smoothSurface(outer, 4, 0.2);
		inner = smoothSurface(inner, 4, 0.2);

		b2Color innerColor(color.r*0.7, color.g*0.8, color.b*0.8);
		drawPoly(outer.data(), outer.size(), innerColor);
		drawPoly(inner.data(), inner.size(), color);
		i++;
	}
}

void drawEyes(std::vector<b2Vec2> vertices, std::vector<b2Vec2> creature) {
	float creatureRadius = getPointsRadius(creature);
	float eyesRadius = 2;
	float koeff = 8;
	if (creatureRadius<eyesRadius * koeff) {
		eyesRadius=creatureRadius/koeff;
	}
	for (auto eye: vertices) {
		drawEllipse(eye, eyesRadius, b2Color(1,1,1), 15);
		drawEllipse(eye, eyesRadius*0.2, b2Color(0,0,0), 15);
	}
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
	  glTranslatef(progress.getFieldWidth() * 0.5f,
				   progress.getFieldHeight() * 0.5f,
				   0);
	  glScalef(progress.playButtonSize * progress.getFieldWidth(),
			   progress.playButtonSize * progress.getFieldWidth(), 1);
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
	  glTranslatef(progress.getFieldWidth() - progress.lineWidth / progress.getFieldWidth(),
				   0,
				   0);
	  glScalef(1, progress.GetLevelTimer() / progress.GetLevelTime() * progress.getFieldHeight(), 1);
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
