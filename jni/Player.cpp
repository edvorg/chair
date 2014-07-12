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

#include "Player.hpp"

#include <math.h>

#include "Misc.hpp"
#include "Graphics.hpp"

namespace test {

  void Player::Init() {
	tail.Init();
	bullets.Init();
  }

  void Player::Update(double dt) {

	tail.Update(dt);
	tail.SetSpawning(IsSpawned());

	bullets.Update(dt);
	bullets.SetPos(GetX(), GetY());
	bullets.SetAngle(GetAngle());
	bullets.SetSpawning(IsSpawned());

	if (!IsSpawned()) {
	  respawnTimer += dt;
	  return;
	}

	/////////////////////////////////////////////////////////////////////
	// player manipulation code /////////////////////////////////////////

	float deltaX = targetX - x;
	float deltaY = targetY - y;
	float distance = sqrt(deltaX * deltaX + deltaY * deltaY);

	if (distance > 0.1f) {
	  if (distance > 15.0f) { // acceleration
		velX += acceleration;
		velY += acceleration;
	  }
	  else { // deceleration
		velX /= deceleration;
		velY /= deceleration;
	  }

	  x += deltaX * velX;
	  y += deltaY * velY;

	  // look in direction of finger

	  deltaX /= distance;
	  angle = acos(deltaX) / M_PI * 180.0f;
	  if (deltaY < 0.0) {
		angle *= - 1.0f;
	  }
	  angle -= 90.0f;
	}

	// player manipulation code /////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////

	x += velX;
	y += velY;

	// trow particles form player's ship engine

	auto dirx = cos((angle + 90.0f) * M_PI / 180.0f) * 10.0f;
	auto diry = sin((angle + 90.0f) * M_PI / 180.0f) * 10.0f;

	tail.SetOwnerX(x - dirx);
	tail.SetOwnerY(y - diry);
	tail.SetOwnerSize(size * 0.5);
	tail.SetOwnerVelX(dirx);
	tail.SetOwnerVelY(diry);
  }

  void Player::Draw() {
	DrawPlayer(*this);
	tail.Draw();
	bullets.Draw();

	// draw hud

	if (IsActive()) {
	  if (!IsSpawned() && respawnTimer < respawnPeriod && GetLives() > 0) {
		DrawNumber(respawnTimerFromLeft,
				   respawnTimerX,
				   respawnTimerY,
				   1,
				   1.5,
				   std::max<float>(0.1f, 1.0f + respawnPeriod - respawnTimer));
	  }
	  DrawNumber(LiveFromLeft,
				 LiveX,
				 LiveY,
				 1,
				 1.5,
				 lives);
	}
  }

  void Player::Release() {
	tail.Release();
	bullets.Release();
  }

  void Player::Kill() {
	if (spawned) {
	  spawned = false;
	  lives = std::max<int>(lives - 1, 0);
	  respawnTimer = 0.0f;
	}
  }

  void Player::Touch(float newX, float newY) {

	if (!spawned && respawnTimer > respawnPeriod) {
	  // spawn player

	  spawned = true;
	  x = newX;
	  y = newY;
	  angle = 0.0f;
	  velX = 0.0f;
	  velY = 0.0f;
	  targetX = newX;
	  targetY = newY;
	  size = 7.0f;
	  respawnTimer = 0.0f;
	  active = true;
	}
	else {
	  targetX = newX;
	  targetY = newY;
	}
  }

  void Player::Reset() {
	lives = livesInitial;
	active = false;
	respawnTimer = respawnPeriod + 1.0f;
	bullets.Clean();
  }

  Dimensions Player::GetDimensions() const {
	return Dimensions(x, y, size * 0.5);
  }

  void Player::FieldSize(float w, float h) {
	bullets.FieldSize(w, h);
	tail.FieldSize(w, h);
	fieldWidth = w;
	fieldHeight = h;
  }

  void Player::RespawnTimeCorner(float x, float y, bool fromLeft) {
	respawnTimerX = x;
	respawnTimerY = y;
	respawnTimerFromLeft = fromLeft;
  }

  void Player::LiveCorner(float x, float y, bool fromLeft) {
	LiveX = x;
	LiveY = y;
	LiveFromLeft = fromLeft;
  }

}
