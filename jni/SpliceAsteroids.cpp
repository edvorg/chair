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

#include "SpliceAsteroids.hpp"

#include <algorithm>
#include "Misc.hpp"

namespace test {

  void SpliceAsteroids::Init() {
	Asteroids::Init();

	auto topPostSpawn = TopPostSpawn();

	PushPeriod([this] {
		if (active) {
		  return spawnPeriodActive;
		}
		else {
		  return spawnPeriodPassive;
		}
	  });

	// use some generated data from Super and modify for our task

	PushPostSpawn([this, topPostSpawn] (Asteroid & created) {
		// call old callback

		topPostSpawn(created);

		// change some settings

		created.SetPos(Rand(- GetFieldWidth() * 0.5f, 0.0f, 0.1f),
					   Rand(GetFieldHeight() * 0.5f, GetFieldHeight() * 1.5f, 0.1f));
		created.SetVel(GetFieldWidth() * 0.25,
					   - GetFieldHeight() * 0.125);

		if (leftToRight) {
		  created.IncPosX(GetFieldWidth() * 2.0f);
		  created.DecVelX(- created.GetVelX());
		}
	  });
  }

  void SpliceAsteroids::Update(double dt) {
	Asteroids::Update(dt);

	if (active) {
	  spawnPeriodActiveTimer += dt;
	  if (spawnPeriodActiveTimer > spawnPeriodActivePeriod) {
		spawnPeriodActiveTimer = 0.0f;
		active = false;
	  }
	}
	else {
	  spawnPeriodPassiveTimer += dt;
	  if (spawnPeriodPassiveTimer > spawnPeriodPassivePeriod) {
		spawnPeriodPassiveTimer = 0.0f;
		active = true;
		leftToRight = rand() % 2;
	  }
	}
  }

  void SpliceAsteroids::SetLevel(unsigned int newLevel) {
	if (level != newLevel) {
	  level = newLevel;
	  auto l = level + 1;
	  spawnPeriodActive = spawnPeriodActiveInitial / l / l;
	  spawnPeriodPassive = spawnPeriodPassiveInitial / l / l;
	  spawnPeriodActiveTimer = 0.0f;
	  spawnPeriodPassiveTimer = 0.0f;
	}
  }

}
