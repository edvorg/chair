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
    ~App();

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

    static constexpr const auto playerBodiesCount = 32;
    static constexpr const auto playerEyesCount = 2;
    static constexpr const auto playerStatesCount = 3;
    static constexpr const auto playerBodySize = 0.5f;
    static constexpr const auto playerEyeSize = 0.75f;

    const std::vector<float> playerGravityStates { -12.8, -9.8, 1.0 };
    const std::vector<float> playerAngleStates { 0.0, -300, -700 };
    const std::vector<float> playerForceStates { 1.25, 0.0, 0.05 };

    const std::vector<float> playerStatePoints { 0.0, 0.493519, 1.0 };

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

    /// player

    float playerState = 0.0f;
    float playerStateInv = 1.0f - playerState;

    int playerStatePoint = 0;

    float playerGravityState = playerGravityStates[playerStatePoint];
    float playerAngleState = playerAngleStates[playerStatePoint];
    float playerForceState = playerForceStates[playerStatePoint];

	std::vector<b2Vec2> playerBodiesPoints;
	std::vector<b2Body*> playerBodies;
    std::vector<std::shared_ptr<b2Shape>> playerBodiesShapes;

    std::vector<b2Vec2> playerEyesBodiesPoints;
    std::vector<b2Body*> playerEyesBodies;
    std::vector<std::shared_ptr<b2Shape>> playerEyesBodiesShapes;

    /// borders

    std::vector<std::shared_ptr<b2Shape>> borderShapes;
    std::vector<b2Body*> borderBodies;

    /// balancer

    const std::vector<std::pair<const float*, float&>> balancer {
        { playerGravityStates.data(), playerGravityState },
        { playerAngleStates.data(), playerAngleState },
        { playerForceStates.data(), playerForceState },
    };
};

}

#endif
