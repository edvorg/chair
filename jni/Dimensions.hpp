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

#ifndef DIMENSIONS_HPP
#define DIMENSIONS_HPP

namespace test {

  // TODO extend with more complex dimensions definition (maby by polygons)
  // dimensions definition using object radius
  class Dimensions {
  public:
	Dimensions() = default;
	Dimensions(float newX, float newY, float newRad);

	inline bool Intersect(const Dimensions & another) const;

  protected:
  private:
	float x = 0;
	float y = 0;
	float rad = 0;
  };

  inline bool Dimensions::Intersect(const Dimensions & another) const {
	float deltax = x - another.x;
	float deltay = y - another.y;
	float rads = rad + another.rad;
	return (deltax * deltax + deltay * deltay < rads * rads);
  }
}

#endif// DIMENSIONS_HPP
