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

#include "App.hpp"

#include <stdlib.h>
#include <math.h>
#include <algorithm>

#include "Graphics.hpp"

namespace test {

  void App::Init() {
	asteroids.Init();
	spliceAsteroids.Init();
	progress.Init();
	stars.Init();

	for (int i = 0; i < maxPlayersCount; i++) {
	  players[i].Init();
	}
  }

  void App::Update(double dt) {
	progress.Update(dt);
	shaker.Update(dt);

	if (!progress.IsPaused()) {
	  asteroids.Update(dt);
	  spliceAsteroids.Update(dt);
	  stars.Update(dt);
	  asteroids.SetLevel(progress.GetLevel());
	  spliceAsteroids.SetLevel(progress.GetLevel());

	  int livesTotal = 0;
	  int activeTotal = 0;
	  int spawnedTotal = 0;

	  CollideAsteroids();

	  // process collisions, collect some data
	  for (int i = 0; i < maxPlayersCount; i++) {
		players[i].Update(dt);
		CollidePlayer(players[i]);

		if (players[i].IsSpawned()) {
		  spawnedTotal++;
		}

		if (players[i].IsActive()) {
		  activeTotal++;
		  livesTotal += players[i].GetLives();
		}
	  }

	  // GAME OVER
	  if (livesTotal < 1 && activeTotal) {
		for (int i = 0; i < maxPlayersCount; i++) {
		  players[i].Reset();
		}
		progress.RestartGame();
		asteroids.Clean();
		spliceAsteroids.Clean();
		stars.Clean();
		scoreTotal = 0.0f;
	  }

	  // no players spawned, restart level and wait for spawn
	  if (activeTotal && !spawnedTotal) {
		progress.RestartLevel();
	  }
	}
  }

  void App::Draw() {
	SetProjection(fieldWidth, fieldHeight);

	if (!progress.IsPaused()) {
	  shaker.ApplyMatrix();
	  asteroids.Draw();
	  spliceAsteroids.Draw();
	  stars.Draw();
	  for (int i = 0; i < maxPlayersCount; i++) {
		players[i].Draw();
	  }
	  DrawNumber(false,
				 fieldWidth - 5.0f,
				 fieldHeight - 10.0f,
				 1,
				 1.5,
				 progress.GetLevel());
	  DrawNumber(true,
				 5.0f,
				 fieldHeight - 10.0f,
				 1,
				 1.5,
				 scoreTotal);
	}

	SetTranslate(0, 0);
	progress.Draw();
  }

  void App::Release() {
	asteroids.Release();
	spliceAsteroids.Release();
	progress.Release();
	stars.Release();
	for (int i = 0; i < maxPlayersCount; i++) {
	  players[i].Release();
	}
  }

  void App::Touch(int player, float newX, float newY) {
	auto x = newX / screenWidth * fieldWidth;
	auto y = (1.0 - newY / screenHeight) * fieldHeight;

	progress.Touch(newX, newY);

	if (!progress.IsPaused() &&
		players[player].GetLives() > 0 &&
		player < maxPlayersCount &&
		player >= 0) {
	  players[player].Touch(x, y);
	}
  }

  void App::TouchEnd(int player, float newX, float newY) {
  }

  void App::ScreenSize(float newWidth, float newHeight) {
	screenWidth = (newWidth > 0.1 ? newWidth : 1.0f);
	screenHeight = (newHeight > 0.1 ? newHeight : 1.0f);
	fieldHeight = fieldWidth * screenHeight / screenWidth;

	asteroids.FieldSize(fieldWidth, fieldHeight);
	asteroids.MarginLeft(10);
	asteroids.MarginRight(10);
	asteroids.MarginTop(fieldHeight * 0.5);
	asteroids.MarginBottom(10);

	spliceAsteroids.FieldSize(fieldWidth, fieldHeight);
	spliceAsteroids.MarginLeft(fieldWidth);
	spliceAsteroids.MarginRight(fieldWidth);
	spliceAsteroids.MarginTop(fieldHeight * 0);
	spliceAsteroids.MarginBottom(10);

	stars.FieldSize(fieldWidth, fieldHeight);
	stars.MarginLeft(10);
	stars.MarginRight(10);
	stars.MarginTop(10);
	stars.MarginBottom(10);

	progress.FieldSize(fieldWidth, fieldHeight);
	for (int i = 0; i < maxPlayersCount; i++) {
	  players[i].FieldSize(fieldWidth, fieldHeight);

	  // compute hud elements params
	  auto x = 5 + (fieldWidth - 10.0f) / (maxPlayersCount - 1) * i;
	  players[i].LiveCorner(x,
							5,
							x < fieldWidth * 0.5 ? true : false);
	  players[i].RespawnTimeCorner(x,
								   10,
								   x < fieldWidth * 0.5 ? true : false);
	}
  }

  void App::CollideAsteroids() {
	  auto callback = [&] (Asteroid & a, Asteroid & b) {
		if (&a != &b) {
		  // TODO improve algo using absolute rigid body law
		  auto deltax = a.GetPosX() - b.GetPosX();
		  auto deltay = a.GetPosY() - b.GetPosY();
		  auto distance = sqrt(deltax * deltax + deltay * deltay);
		  deltax /= distance;
		  deltay /= distance;
		  a.IncVel(deltax * b.GetSize(), deltay * b.GetSize());
		  b.DecVel(deltax * a.GetSize(), deltay * a.GetSize());

		  if (a.GetSize() > 7.5f) {
			a.SetDead(true);
		  }
		  if (b.GetSize() > 7.5f) {
			b.SetDead(true);
		  }
		}
	  };

	  asteroids.Collide<Asteroid>(asteroids, callback);
	  asteroids.Collide<Asteroid>(spliceAsteroids, callback);
	  spliceAsteroids.Collide<Asteroid>(spliceAsteroids, callback);
  }

  void App::CollidePlayer(Player & player) {
	auto callback1 = [&] (Asteroid & a, Bullet & b) {
	  a.SetDead(true);
	  b.SetDead(true);
	  scoreTotal += std::abs(a.GetVelX() * a.GetVelY());
	};
	auto callback2 = [&] (Asteroid & a, Player & p) {
	  p.Kill();
	  shaker.Shake();
	  a.SetDead(true);
	};

	player.CollideBullets<Asteroid>(asteroids, callback1);
	player.CollideBullets<Asteroid>(spliceAsteroids, callback1);
	player.Collide<Asteroid>(asteroids, callback2);
	player.Collide<Asteroid>(spliceAsteroids, callback2);
  }

}
