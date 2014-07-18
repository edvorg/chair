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

App::App() : world(b2Vec2(0, -9.8)) {
}

void App::Init() {
}

void App::Update(double dt) {
	progress.Update(dt);
	shaker.Update(dt);

	if (!progress.IsPaused()) {
		// update gameplay here
	}
}

void App::Draw() {
	SetProjection(fieldWidth, fieldHeight);

	if (!progress.IsPaused()) {
		shaker.ApplyMatrix();
		DrawNumber(false,
				   fieldWidth - 5.0f,
				   fieldHeight - 10.0f,
				   1,
				   1.5,
				   progress.GetLevel());
	}

	SetTranslate(0, 0);
	progress.Draw();
}

void App::Release() {
}

void App::Touch(int player, float newX, float newY) {
	auto x = newX / screenWidth * fieldWidth;
	auto y = (1.0 - newY / screenHeight) * fieldHeight;

	progress.Touch(newX, newY);

	if (!progress.IsPaused()) {
	}
}

void App::TouchEnd(int player, float newX, float newY) {
}

void App::ScreenSize(float newWidth, float newHeight) {
	screenWidth = (newWidth > 0.1 ? newWidth : 1.0f);
	screenHeight = (newHeight > 0.1 ? newHeight : 1.0f);
	fieldHeight = fieldWidth * screenHeight / screenWidth;

	progress.FieldSize(fieldWidth, fieldHeight);
}

}
