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

#ifndef LEVELPROGRESS_HPP
#define LEVELPROGRESS_HPP

#include "RandomizeParams.hpp"

namespace test {

  class LevelProgress {
  public:
	void Init();
	void Update(double dt);
	void Draw();
	void Release();

	void FieldSize(float width, float height);
	void RestartLevel();
	void RestartGame();
	void Touch(float x, float y);

	inline bool IsPaused() const { return paused; }
	inline RandomizeParams GetParams() const { return params; }
	inline float GetLevelTime() const { return levelTime; }
	inline float GetLevelTimer() const { return levelTimer; }
	inline unsigned int GetLevel() const { return level; }

	const float levelTimeExtendKoeff = 1.5f;
	const float levelTimeInitial = 20.0f;
	// progress bar line width
	const float lineWidth = 16.0f;
	// play picture line width
	const float playLineWidth = 8.0f;
	const float playButtonSize = 0.1f;
	// delay after which play button becomes enabled
	const float startTimerPeriod = 1.0f;

  protected:
  private:
	// current level length
	float levelTime = levelTimeInitial;
	// current level playing time
	float levelTimer = 0.0f;
	// some general params
	RandomizeParams params;
	// current game level
	unsigned int level = 0;
	// is game on pause?
	bool paused = true;
	float startTimer = startTimerPeriod + 1.0;
  };

}

#endif// LEVELPROGRESS_HPP
