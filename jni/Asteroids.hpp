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

#ifndef ASTEROIDS_HPP
#define ASTEROIDS_HPP

#include <memory>
#include <vector>
#include <functional>

#include "Asteroid.hpp"
#include "RandomizeParams.hpp"
#include "Dimensions.hpp"
#include "ParticleSystem.hpp"

namespace test {

  class Asteroids : public ParticleSystem<Asteroid> {

  public:
	using Super = ParticleSystem<Asteroid>;

	virtual void Init() override;
	virtual void Update(double dt) override;

	// set current progress level and adjust rand params
	void SetLevel(unsigned int level);

  protected:
  private:
	const float periodMinInitial = 1.0f;
	const float periodMaxInitial = 2.0f;

	// minimum size of asteroid to crush in small parts
	float minCrushSize = 5.0f;

	//asteroids can crush in smaller parts
	int crushPartsCount = 3;
	int crushPartsAwayVel = 10.0f;

	// random asteroids gen periods
	float periodMin = periodMinInitial;
	float periodMax = periodMaxInitial;
  };

}

#endif// ASTEROIDS_HPP
