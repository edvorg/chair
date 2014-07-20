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

#ifndef GRAPHICS
#define GRAPHICS

#include <Box2D/Box2D.h>
#include <vector>
#include <stack>

namespace test {
  class LevelProgress;

  // apply translation matrix
void SetTranslate(float x, float y, bool override = true);
  // set ortho projection
  void SetProjection(float fieldWidth, float fieldHeight);
  void DrawNumber(bool fromLeft, float x, float y, float sizex, float sizey, unsigned int number);
  void DrawDigit(float x, float y, float sizex, float sizey, unsigned int digit);

  std::vector<std::vector<b2Vec2>> findClusters(std::vector<b2Vec2> vertices, float threshold);
  std::vector<b2Vec2> sphericize(std::vector<b2Vec2> vertices, float radius, int numPoints=4);
  float getPointsRadius(std::vector<b2Vec2> vertices);
  void drawEllipse(b2Vec2 center, float radius, b2Color color, int numPoints = 6);
  void drawPoints(const b2Vec2* points, int32 vertexCount, b2Color color, float radius, int numPoints = 6);
  void drawPointsColored(const b2Vec2* points, int32 vertexCount, float* color, float radius, int numPoints = 6);
void drawPoly(const b2Vec2* points, int32 vertexCount, b2Color color, float alpha = 1.0f);
  std::vector<b2Vec2> smoothSurface(std::vector<b2Vec2> vertices, int iterations, float smoothRatio);
  std::vector<b2Vec2> quickHull(std::vector<b2Vec2> vertices, std::vector<b2Vec2>* leftovers = nullptr);
  //void DrawPlayer(const Player & player);
  //void DrawObstacle(const Asteroid & asteroid);
  //void DrawDecorateStar(const DecorateStar & star);
  //void DrawBullet(const Bullet & bullet);
  void drawCreature(std::vector<b2Vec2> vertices, b2Color color, float alpha, float state);
  void drawEyes(std::vector<b2Vec2> vertices, std::vector<b2Vec2> creature);


  void DrawProgress(const LevelProgress & progress);

}

#endif// GRAPHICS
