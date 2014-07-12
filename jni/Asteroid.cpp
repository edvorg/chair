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

#include "Asteroid.hpp"

#include "Misc.hpp"
#include "Graphics.hpp"

namespace test {

  void Asteroid::Init() {
	tail.Init();
  }

  void Asteroid::Update(double dt) {
	tail.SetSpawning(GetVelX() * GetVelX() + GetVelY() * GetVelY() > 400 && GetSize() < 5.0);
	tail.Update(dt);
	tail.SetOwnerX(GetPosX());
	tail.SetOwnerY(GetPosY());
	tail.SetOwnerVelX(GetVelX());
	tail.SetOwnerVelY(GetVelY());
	tail.SetOwnerSize(GetSize());
	DecVelY(10 * dt);
  }

  void Asteroid::Draw() {
	DrawAsteroid(*this);
	tail.Draw();
  }

  void Asteroid::Release() {
	tail.Release();
  }

  Dimensions Asteroid::GetDimensions() const {
	return Dimensions(GetPosX(), GetPosY(), GetSize());
  }

  Asteroid & Asteroid::operator = (const Asteroid & another) {
	Particle::operator=(another);
	return *this;
  }

}
