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

#include <cstdlib>
#include <cmath>
#include <algorithm>

#include "graphics/Graphics.hpp"
#include "graphics/debugdraw.hpp"

#include <log.h>
#include <vector>

namespace test {

App::App() :
	world(b2Vec2(0, -9.8)),
	playerBodiesPoints(playerBodiesCount),
	playerBodies(playerBodiesCount),
	playerBodiesShapes(playerBodiesCount),
	playerEyesBodiesPoints(playerEyesCount),
	playerEyesBodies(playerEyesCount),
	playerEyesBodiesShapes(playerEyesCount),
	borderShapes(4),
	borderBodies(4) {

	world.SetDebugDraw(new graphics::DebugDraw);

	for (auto i = 0; i < 4; ++i) {
		const auto shape = new b2PolygonShape;
		shape->SetAsBox(150, 5);

		b2BodyDef bdef;

		bdef.awake = true;
		bdef.allowSleep = true;
		bdef.active = true;
		bdef.type = b2_kinematicBody;

		const auto body = world.CreateBody(&bdef);

		b2FixtureDef fdef;
		fdef.shape = shape;

		const auto fixt = body->CreateFixture(&fdef);

		borderShapes[i].reset(shape);
		borderBodies[i] = body;
	}

	borderBodies[0]->SetTransform({ 50, 0 }, 0);
	borderBodies[1]->SetTransform({ 350, 0 }, 0);
	borderBodies[2]->SetTransform({ 50, 55 }, 0);
	borderBodies[3]->SetTransform({ 350, 55 }, 0);

	for (auto i = 0; i < playerBodiesCount; ++i) {
		const auto shape = new b2PolygonShape;
		shape->SetAsBox(playerBodySize, playerBodySize);

		b2BodyDef bdef;

		bdef.position.x = 0 + rand() % 50;
		bdef.position.y = 15 + rand() % 30;
		bdef.type = b2_dynamicBody;

		const auto body = world.CreateBody(&bdef);

		b2FixtureDef fdef;
		fdef.shape = shape;
		fdef.density = 1.0;

		const auto fixt = body->CreateFixture(&fdef);

		playerBodiesShapes[i].reset(shape);
		playerBodies[i] = body;
		playerBodiesPoints[i] = bdef.position;
	}

	for (auto i = 0; i < playerEyesCount; ++i) {
		const auto shape = new b2PolygonShape;
		shape->SetAsBox(playerEyeSize, playerEyeSize);

		b2BodyDef bdef;

		bdef.position.x = 0 + rand() % 50;
		bdef.position.y = 15 + rand() % 30;
		bdef.type = b2_dynamicBody;

		const auto body = world.CreateBody(&bdef);

		b2FixtureDef fdef;
		fdef.shape = shape;
		fdef.density = 1.0;

		const auto fixt = body->CreateFixture(&fdef);

		playerEyesBodiesShapes[i].reset(shape);
		playerEyesBodies[i] = body;
		playerEyesBodiesPoints[i] = bdef.position;
	}
}

App::~App() {
	for (auto& b : playerBodies)
		world.DestroyBody(b);
	for (auto& b : playerEyesBodies)
		world.DestroyBody(b);
	for (auto& b : borderBodies)
		world.DestroyBody(b);
}

void App::Init() {
}

void App::Update(double dt) {
	progress.Update(dt);
	shaker.Update(dt);

	if (!progress.IsPaused()) {
		world.Step(dt, 1, 2);

		world.SetGravity({ 0, playerGravityState });

		for (auto i = 0; i < playerBodies.size(); i++)
			playerBodiesPoints[i] = playerBodies[i]->GetPosition();

		for (auto i = 0; i < playerEyesBodies.size(); i++)
			playerEyesBodiesPoints[i] = playerEyesBodies[i]->GetPosition();

		for (auto& b1 : playerBodies) {
			b1->SetAngularVelocity(dt * playerAngleState);

			for (auto& b2 : playerBodies) {
				b2Vec2 dir = b1->GetPosition() - b2->GetPosition();
				b2->ApplyForceToCenter(playerForceState * dir, true);
			}
		}
	}
}

void App::Draw() {
	SetProjection(fieldWidth, fieldHeight);

	if (!progress.IsPaused()) {
		shaker.ApplyMatrix();

		world.DrawDebugData();

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
	auto y = (1.0f - newY / screenHeight) * fieldHeight;
}

void App::TouchEnd(int player, float newX, float newY) {
	if (progress.IsPaused()) {
		progress.Touch(newX, newY);
		return;
	}

	playerStatePoint++;
	if (playerStatePoint >= playerStatePoints.size()) {
		playerStatePoint = 0;
		shaker.Shake();
	}

	playerState = playerStatePoints[playerStatePoint];
	playerStateInv = 1.0f - playerState;

	if (playerState <= 0.5) {
		const auto remap = playerState * 2.0f;
		const auto remapInv = (1.0f - remap);

		playerAngleState = remapInv * playerAngleStates[0] +
			remap * playerAngleStates[1];
		playerForceState = remapInv * playerForceStates[0] +
			remap * playerForceStates[1];
		playerGravityState = remapInv * playerGravityStates[0] +
			remap * playerGravityStates[1];
	}
	else {
		const auto remap = (playerState - 0.5f) * 2.0f;
		const auto remapInv = (1.0f - remap);

		playerAngleState = remapInv * playerAngleStates[1] +
			remap * playerAngleStates[2];
		playerForceState = remapInv * playerForceStates[1] +
			remap * playerForceStates[2];
		playerGravityState = remapInv * playerGravityStates[1] +
			remap * playerGravityStates[2];
	}
}

void App::ScreenSize(float newWidth, float newHeight) {
	screenWidth = (newWidth > 0.1 ? newWidth : 1.0f);
	screenHeight = (newHeight > 0.1 ? newHeight : 1.0f);
	fieldHeight = fieldWidth * screenHeight / screenWidth;

	progress.FieldSize(fieldWidth, fieldHeight);
}

}
