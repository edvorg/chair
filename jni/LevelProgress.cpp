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

#include "LevelProgress.hpp"

#include "Graphics.hpp"

namespace test {

  void LevelProgress::Init() {
  }

  void LevelProgress::Update(double dt) {
	// wait some time before we can start game
	if (paused) {
	  startTimer += dt;
	  return;
	}

	// if we played enough go to next level
	levelTimer += dt;
	if (levelTimer > levelTime) {
	  levelTimer = 0.0f;
	  levelTime *= levelTimeExtendKoeff;
	  level++;
	}
  }

  void LevelProgress::Draw() {
	DrawProgress(*this);
  }

  void LevelProgress::Release() {
  }

  void LevelProgress::FieldSize(float width, float height) {
	params.fieldWidth = width;
	params.fieldHeight = height;
  }

  void LevelProgress::RestartLevel() {
	levelTimer = 0.0f;
  }

  void LevelProgress::RestartGame() {
	// reset to first level
	levelTimer = 0.0f;
	level = 0;
	levelTime = levelTimeInitial + 1.0;
	paused = true;
  }

  void LevelProgress::Touch(float x, float y) {
	// try to start game
	if (startTimer > startTimerPeriod) {
	  paused = false;
	  startTimer = 0.0f;
	}
  }

}
