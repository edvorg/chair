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

#include "ScreenShaker.hpp"

#include <math.h>

#include "Graphics.hpp"

namespace test {

  void ScreenShaker::Shake() {
	shake = true;
  }

  void ScreenShaker::Update(double dt) {
	if (shake) {
	  shakeTimer += dt;
	  if (shakeTimer > shakeTime) {
		shake = false;
		SetTranslate(0, 0);
		shakeTimer = 0.0f;
	  }
	  else {
		auto amplitude = (1.0 - shakeTimer / shakeTime) * 5.0;
		auto angle = shakeTimer / shakeTime * M_PI * 2.0f * 10.0f;
		SetTranslate(amplitude * sin(angle), 0);
	  }
	}
  }

  void ScreenShaker::ApplyMatrix() {
	if (shake) {
	  auto amplitude = (1.0 - shakeTimer / shakeTime) * 1.0;
	  auto angle = shakeTimer / shakeTime * M_PI * 2.0f * 10.0f;
	  SetTranslate(amplitude * sin(angle), 0);
	}
	else {
	  SetTranslate(0, 0);
	}
  }

}
