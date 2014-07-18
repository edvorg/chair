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

#ifndef APP_HPP
#define APP_HPP

#include "LevelProgress.hpp"
#include "ScreenShaker.hpp"
#include <Box2D/Box2D.h>

namespace test {

// main application class, handles all game objects
class App {

public:
    App();

	void Init();
	void Update(double dt);
	void Draw();
	void Release();

	// handle some parameters from device

	// must be called on touch begin
	// player - player id (less then maxPlayersCount)
	void Touch(int player, float newX, float newY);
	// must be called on touch end
	// player - player id (less then maxPlayersCount)
	void TouchEnd(int player, float newX, float newY);
	// set real device screen resolution
	void ScreenSize(float newWidth, float newHeight);

protected:
private:
	// screen res independent field width
	const float fieldWidth = 100.0;
	// maximum allowed number of players
	float fieldHeight = 100.0f;

	// real device screen width
	float screenWidth = 0.0f;
	// real device screen height
	float screenHeight = 0.0f;

	LevelProgress progress;
    ScreenShaker shaker;

    b2World world;

	std::vector<b2Vec2> playerPoints;
	std::vector<b2Body*> playerBodies;
};

}

#endif
