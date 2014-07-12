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

#ifndef COMETTAIL_HPP
#define COMETTAIL_HPP

#include "ParticleSystem.hpp"
#include "DecorateStar.hpp"

namespace test {

  // represents small particle system throwed
  // e.g. from player's ship engine or from asteroid
  class CometTail : public ParticleSystem<DecorateStar> {

  public:
	using Super = ParticleSystem<DecorateStar>;

	virtual void Init() override;

	inline void SetOwnerX(float x) { ownerX = x; }
	inline void SetOwnerY(float y) { ownerY = y; }
	inline void SetOwnerVelX(float velX) { ownerVelX = velX; }
	inline void SetOwnerVelY(float velY) { ownerVelY = velY; }
	inline void SetOwnerSize(float size) { ownerSize = size; }

  protected:
  private:
	const int poolSize = 25;

	// pos x of owning object
	float ownerX = 0.0f;
	// pos y of owning object
	float ownerY = 0.0f;
	// vel x of owning object
	float ownerVelX = 0.0f;
	// vel y of owning object
	float ownerVelY = 0.0f;
	// size of owning object
	float ownerSize = 0.0f;
  };

}

#endif// COMETTAIL_HPP
