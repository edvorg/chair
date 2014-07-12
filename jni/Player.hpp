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

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <functional>

#include "Dimensions.hpp"
#include "ParticleSystem.hpp"
#include "CometTail.hpp"
#include "Bullets.hpp"

namespace test {

  // represents game player
  class Player {
  public:
	template<class PARTICLE>
	using CollideBulletsCallback = std::function<void (PARTICLE &, Bullet &)>;
	template<class PARTICLE>
	using CollideCallback = std::function<void (PARTICLE &, Player &)>;

	void Init();
	void Update(double dt);
	void Draw();
	void Release();

	// try to kill player
	void Kill();

	// process touch
	void Touch(float newX, float newY);

	// collisions with particle systems
	template<class PARTICLE>
	void Collide(ParticleSystem<PARTICLE> & system, CollideCallback<PARTICLE> callback);
	// collisions with bullets
	template<class PARTICLE>
	void CollideBullets(ParticleSystem<PARTICLE> & system, CollideBulletsCallback<PARTICLE> callback);

	// reset player to initial state
	void Reset();

	Dimensions GetDimensions() const;
	// is player on field?
	inline bool IsSpawned() const { return spawned; }
	inline float GetX() const { return x; }
	inline float GetY() const { return y; }
	inline float GetAngle() const { return angle; }
	inline float GetSize() const { return size; }
	inline int GetLives() const { return lives; }
	// is player enrolled in game
	inline int IsActive() const { return active; }
	void FieldSize(float w, float h);
	void RespawnTimeCorner(float x, float y, bool fromLeft);
	void LiveCorner(float x, float y, bool fromLeft);

  protected:
  private:
	// player respawn delay
	const float respawnPeriod = 3.0f;
	const int livesInitial = 3;
	float fieldWidth = 100.0f;
	float fieldHeight = 100.0f;
	// is player on field?
	bool spawned = false;
	float x = 0.0f;
	float y = 0.0f;
	float angle = 0.0f;
	float velX = 0.0f;
	float velY = 0.0f;
	// target x to move to
	float targetX = 0.0f;
	// target y to move to
	float targetY = 0.0f;
	float size = 7.0f;
	float acceleration = 0.005;
	float deceleration = 3;
	float respawnTimer = respawnPeriod + 1.0f;
	int lives = livesInitial;
	// is player enrolled in game
	bool active = false;
	// particles thrown by ship engine
	CometTail tail;
	// bullets thrown by ship cannon
	Bullets bullets;

	// hud

	float respawnTimerX = 0.0f;
	float respawnTimerY = 0.0f;
	float respawnTimerFromLeft = true;
	float LiveX = 0.0f;
	float LiveY = 0.0f;
	float LiveFromLeft = true;
  };

  template<class PARTICLE>
  void Player::Collide(ParticleSystem<PARTICLE> & system, CollideCallback<PARTICLE> callback) {
	if (IsSpawned()) {
	  system.Collide(*this, callback);
	}
  }

  template<class PARTICLE>
  void Player::CollideBullets(ParticleSystem<PARTICLE> & system, CollideBulletsCallback<PARTICLE> callback) {
	system.template Collide<Bullet>(bullets, callback);
  }

}

#endif// PLAYER_HPP
