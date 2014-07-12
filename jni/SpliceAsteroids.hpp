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

#ifndef SPLICEASTEROIDS_HPP
#define SPLICEASTEROIDS_HPP

#include "Asteroids.hpp"

namespace test {

  // represents secondary asteroids system, which time to time
  // thorows asteroids with big speed from right or left side chosen randomly
  class SpliceAsteroids : public Asteroids {

  public:
	virtual void Init() override;
	virtual void Update(double dt) override;

	// set game level, and ajust randomization parameters
	void SetLevel(unsigned int level);

  protected:
  private:
	const float velocity = 10.0f;
	const float spawnPeriodActiveInitial = 0.1f;
	const float spawnPeriodPassiveInitial = 10.0f;
	const float spawnPeriodActivePeriod = 1.0f;
	const float spawnPeriodPassivePeriod = 25.0f;
	float spawnPeriodActive = spawnPeriodActiveInitial;
	float spawnPeriodPassive = spawnPeriodPassiveInitial;
	bool active = false;
	bool leftToRight = false;
	float spawnPeriodActiveTimer = 0.0f;
	float spawnPeriodPassiveTimer = 0.0f;
	int level = 0;
  };

}

#endif// SPLICEASTEROIDS_HPP
