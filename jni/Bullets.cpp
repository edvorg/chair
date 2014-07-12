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

#include "Bullets.hpp"

#include <math.h>

namespace test {

  void Bullets::Init() {
	ParticleSystem<Bullet>::Init();

	PushPeriod([this] {
		return period;
	  });

	PushPostSpawn([this] (Bullet & bullet) {
		// throw bullets in front of owner orientation
		// TODO load params from xml/json
		bullet.SetPos(x, y);
		bullet.SetVel(cos((angle + 90.0f) / 180.0f * M_PI) * 100.0f,
					  sin((angle + 90.0f) / 180.0f * M_PI) * 100.0f);
		bullet.SetSize(1.0f);
	  });
  }

  void Bullets::SetPos(float newX, float newY) {
	x = newX;
	y = newY;
  }

  void Bullets::SetAngle(float newAngle) {
	angle = newAngle;
  }

}
