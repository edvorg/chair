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

namespace test {

  class Player;
  class Asteroid;
  class DecorateStar;
  class Bullet;
  class LevelProgress;

  // apply translation matrix
  void SetTranslate(float x, float y);
  // set ortho projection
  void SetProjection(float fieldWidth, float fieldHeight);
  void DrawNumber(bool fromLeft, float x, float y, float sizex, float sizey, unsigned int number);
  void DrawDigit(float x, float y, float sizex, float sizey, unsigned int digit);
  void DrawPlayer(const Player & player);
  void DrawAsteroid(const Asteroid & asteroid);
  void DrawDecorateStar(const DecorateStar & star);
  void DrawBullet(const Bullet & bullet);
  void DrawProgress(const LevelProgress & progress);

}

#endif// GRAPHICS
