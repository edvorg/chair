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

#ifndef SCREENSHAKER_HPP
#define SCREENSHAKER_HPP

namespace test {

  // screen shake effect used in by e.g. explosions
  class ScreenShaker {
  public:
	// starts screen shake effect
	void Shake();

	void Update(double dt);
	// apply transformation matrix of shake effect
	void ApplyMatrix();

  protected:
  private:
	const float shakeTime = 1.0f;
	float shakeTimer = 0.0f;
	bool shake = false;
  };

}

#endif// SCREENSHAKER_HPP
