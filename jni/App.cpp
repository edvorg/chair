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
		shape->SetAsBox(100, 3);

		b2BodyDef bdef;

		bdef.awake = true;
		bdef.allowSleep = true;
		bdef.active = true;
		bdef.type = b2_kinematicBody;

		const auto body = world.CreateBody(&bdef);

		b2FixtureDef fdef;
		fdef.shape = shape;
		fdef.friction = borderFriction;
		fdef.restitution = borderRestitution;
		fdef.filter.categoryBits = borderCategory;

		const auto fixt = body->CreateFixture(&fdef);

		borderShapes[i].reset(shape);
		borderBodies[i] = body;
	}

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
		fdef.density = playerBodyDencity;
		fdef.friction = playerBodyFriction;
		fdef.restitution = playerBodyRestitution;
		fdef.filter.categoryBits = playerBodyCategory;

		const auto fixt = body->CreateFixture(&fdef);

		playerBodiesShapes[i].reset(shape);
		playerBodies[i] = body;
		playerBodiesPoints[i] = bdef.position;
	}

	for (auto i = 0; i < playerEyesCount; ++i) {
		const auto shape = new b2CircleShape;
		shape->m_radius = playerEyeSize;

		b2BodyDef bdef;

		bdef.position.x = 0 + rand() % 50;
		bdef.position.y = 15 + rand() % 30;
		bdef.type = b2_dynamicBody;

		const auto body = world.CreateBody(&bdef);

		b2FixtureDef fdef;
		fdef.shape = shape;
		fdef.density = playerEyeDencity;
		fdef.friction = playerEyeFriction;
		fdef.restitution = playerEyeRestitution;
		fdef.filter.categoryBits = playerEyeCategory;

		const auto fixt = body->CreateFixture(&fdef);

		playerEyesBodiesShapes[i].reset(shape);
		playerEyesBodies[i] = body;
		playerEyesBodiesPoints[i] = bdef.position;
	}

	{
		const auto shape = new b2PolygonShape;
		shape->SetAsBox(2, 10);

		b2BodyDef bdef;

		bdef.awake = true;
		bdef.allowSleep = true;
		bdef.active = true;
		bdef.type = b2_kinematicBody;

		const auto body = world.CreateBody(&bdef);

		b2FixtureDef fdef;
		fdef.shape = shape;
		fdef.friction = borderFriction;
		fdef.restitution = borderRestitution;
		fdef.filter.categoryBits = borderCategory;

		const auto fixt = body->CreateFixture(&fdef);

		topObstacleShape.reset(shape);
		topObstacle = body;
	}

	{
		const auto shape = new b2PolygonShape;
		shape->SetAsBox(2, 5);

		b2BodyDef bdef;

		bdef.awake = true;
		bdef.allowSleep = true;
		bdef.active = true;
		bdef.type = b2_kinematicBody;

		const auto body = world.CreateBody(&bdef);

		b2FixtureDef fdef;
		fdef.shape = shape;
		fdef.friction = borderFriction;
		fdef.restitution = borderRestitution;
		fdef.filter.categoryBits = borderCategory;

		const auto fixt = body->CreateFixture(&fdef);

		bottomObstacleShape.reset(shape);
		bottomObstacle = body;
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

		// convex points

		playerBodiesPointsMiddle = { 0.0f, 0.0f };
		playerVelocityMiddle = { 0.0f, 0.0f };

		for (auto i = 0; i < playerBodies.size(); i++) {
			playerBodiesPoints[i] = playerBodies[i]->GetPosition();
			playerBodiesPointsMiddle += playerBodiesPoints[i];
			playerVelocityMiddle += playerBodies[i]->GetLinearVelocity();
		}

		playerBodiesPointsMiddle = (1.0f / playerBodies.size()) * playerBodiesPointsMiddle;
		playerVelocityMiddle = (1.0f / playerBodies.size()) * playerVelocityMiddle;

		// cam pos

		camPos = camPos + (playerBodiesPointsMiddle.x - camPos) * dt * 4.0;

		// eye points

		for (auto i = 0; i < playerEyesBodies.size(); i++)
			playerEyesBodiesPoints[i] = playerEyesBodies[i]->GetPosition();

		// body <-> body

		for (auto& b1 : playerBodies) {
			b1->SetAngularVelocity(dt * playerAngleState);

			for (auto& b2 : playerBodies) {
				const auto dir = b1->GetPosition() - b2->GetPosition();
				b2->ApplyForceToCenter(playerForceState * dir, true);
			}
		}

		// eyes antigrav

		for (auto& e : playerEyesBodies) {
			e->ApplyForceToCenter({ 0.0, playerEyeAntigravState }, true);
		}

		// body <-> eye

		for (auto& e : playerEyesBodies) {
			for (auto& b : playerBodies) {
				const auto dir = b->GetPosition() - e->GetPosition();
				e->ApplyForceToCenter(playerEyeBodyForceState * dir, true);
			}
		}

		// borders respawn

		for (auto& b : borderBodies) {
			if (b->GetPosition().x < - 200) {
				b->SetTransform({ 200, b->GetPosition().y }, b->GetAngle());
			}
		}

		// back respawn

		const auto hole1 = rand() % 2 ? 25 : 0;
		const auto hole2 = rand() % 2 ? 25 : 0;

		if (borderBodies[0]->GetPosition().x - playerBodiesPointsMiddle.x < 50.0) {
			borderBodies[0]->SetTransform(
				{ (((int)playerBodiesPointsMiddle.x) / 200) * 200.0f + 25.0f,     5 }, 0);
			borderBodies[1]->SetTransform(
				{ (((int)playerBodiesPointsMiddle.x) / 200) * 200.0f + 25.0f,     50 }, 0);
		}

		if (borderBodies[2]->GetPosition().x - playerBodiesPointsMiddle.x < 50.0) {
			borderBodies[2]->SetTransform(
				{ (((int)playerBodiesPointsMiddle.x) / 200) * 200.0f + 225.0f + hole1, 5 }, 0);
			borderBodies[3]->SetTransform(
				{ (((int)playerBodiesPointsMiddle.x) / 200) * 200.0f + 225.0f + hole2, 50 }, 0);
		}

		const auto choice = rand() % 2;

		if (bottomObstacle->GetPosition().x - playerBodiesPointsMiddle.x < -75.0f &&
			topObstacle->GetPosition().x - playerBodiesPointsMiddle.x < -75.0f) {
			if (choice == 0) {
				bottomObstacle->SetTransform(
					{ playerBodiesPointsMiddle.x + 75.0f, 10.0f }, bottomObstacle->GetAngle());
			}
			else if (choice == 1) {
				topObstacle->SetTransform(
					{ playerBodiesPointsMiddle.x + 75.0f, 45.0f }, topObstacle->GetAngle());
			}
		}

		for (auto& b1 : playerBodies) {
			if ((playerBodiesPointsMiddle - b1->GetPosition()).Length() > 20.0) {
				b1->SetTransform(
					{ playerBodiesPointsMiddle.x, playerBodiesPointsMiddle.y },
					b1->GetAngle());
				b1->SetLinearVelocity(playerVelocityMiddle);
			}
		}

		for (auto& b1 : playerBodies) {
			const auto vel = b1->GetLinearVelocity();
			b1->SetLinearVelocity({ 40.0f, vel.y });
		}
	}
}

void App::Draw() {
	SetProjection(fieldWidth, fieldHeight);

	SetTranslate(0, 0);
	progress.Draw();

	if (!progress.IsPaused()) {
		SetTranslate(25 - camPos, 0);
		shaker.ApplyMatrix();

	drawCreature(playerBodiesPoints, b2Color(playerColorRState, playerColorGState, playerColorBState), 1);

	world.DrawDebugData();

	SetTranslate(0, 0);
	shaker.ApplyMatrix();

	DrawNumber(false,
			   fieldWidth - 5.0f,
			   fieldHeight - 10.0f,
			   1,
			   1.5,
			   progress.GetLevel());
	}
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

	// drop eyes

	if  (playerStatePoint == 2) {
		for (auto& e : playerEyesBodies) {
			b2Filter f = e->GetFixtureList()->GetFilterData();
			f.maskBits = borderCategory | playerEyeCategory;
			e->GetFixtureList()->SetFilterData(f);
		}
	}
	else {
		for (auto& e : playerEyesBodies) {
			b2Filter f = e->GetFixtureList()->GetFilterData();
			f.maskBits = borderCategory | playerBodyCategory | playerEyeCategory;
			e->GetFixtureList()->SetFilterData(f);
		}
	}

	if (playerStatePoint == 0) {
		for (auto& b1 : playerBodies) {
			b1->ApplyLinearImpulse({ 0.0f, -100.0f}, {0.0f, 0.0f}, true);
		}
	}
	else if (playerStatePoint == 2) {
		for (auto& b1 : playerBodies) {
			const auto vel = b1->GetLinearVelocityFromLocalPoint({ 0, 0});
			b1->SetLinearVelocity(vel + b2Vec2 { 0, 25.0 });
		}
	}

	// handle balancer

	playerState = playerStatePoints[playerStatePoint];

	if (playerState <= 0.5) {
		const auto remap = playerState * 2.0f;
		for (auto& p : balancer) p.second = (1.0f - remap) * p.first[0] + remap * p.first[1];
	}
	else {
		const auto remap = (playerState - 0.5f) * 2.0f;
		for (auto& p : balancer) p.second = (1.0f - remap) * p.first[1] + remap * p.first[2];
	}
}

void App::ScreenSize(float newWidth, float newHeight) {
	screenWidth = (newWidth > 0.1 ? newWidth : 1.0f);
	screenHeight = (newHeight > 0.1 ? newHeight : 1.0f);
	fieldHeight = fieldWidth * screenHeight / screenWidth;

	progress.FieldSize(fieldWidth, fieldHeight);
}

}
