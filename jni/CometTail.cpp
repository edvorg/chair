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

#include "CometTail.hpp"

#include <math.h>

#include "Misc.hpp"

namespace test {

  void CometTail::Init() {
	Super::Init();
	Super::Resize(poolSize);

	PushPeriod([] {
		return 0.05;
	  });

	PushPostSpawn([this] (DecorateStar & star) {
		// throw particles from back of owner orientation
		star.SetSize(5);
        float angle = Rand<float>(0.0, M_PI * 2.0, 0.01);
        float rad = Rand<float>(0.0, ownerSize, 0.01);
		star.SetPos(ownerX + cos(angle) * rad,
					ownerY + sin(angle) * rad);
		star.SetVel(- ownerVelX,
					- ownerVelY);
	  });

	PushLife([] {
		return 0.25f;
	  });
  }

}
