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

#ifndef DECORATESTARS_HPP
#define DECORATESTARS_HPP

#include "ParticleSystem.hpp"
#include "DecorateStar.hpp"

namespace test {

  // represents stars cloud particle system
  class DecorateStars : public ParticleSystem<DecorateStar> {

  public:
	using Super = ParticleSystem<DecorateStar>;

	virtual void Init() override;

  protected:
  private:
	const int poolSize = 500;
  };

}

#endif// DECORATESTARS_HPP
