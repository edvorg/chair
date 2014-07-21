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

#include <EGL/egl.h>
#include <GLES/gl.h>

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

		bdef.awake = false;
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
		playerBodiesFallen[body] = false;
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
		shape->SetAsBox(2, 13);

		b2BodyDef bdef;

		bdef.awake = false;
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

		bdef.awake = false;
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

	{
		const auto shape = new b2PolygonShape;
		shape->SetAsBox(2, 7);

		b2BodyDef bdef;

		bdef.awake = false;
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

		bottomHighObstacleShape.reset(shape);
		bottomHighObstacle = body;
	}

	{
		const auto shape = new b2PolygonShape;
		shape->SetAsBox(12.5, 2);

		b2BodyDef bdef;

		bdef.awake = false;
		bdef.allowSleep = true;
		bdef.active = true;
		bdef.type = b2_kinematicBody;
		bdef.position.x = -100.0f;

		const auto body = world.CreateBody(&bdef);

		b2FixtureDef fdef;
		fdef.shape = shape;
		fdef.friction = borderFriction;
		fdef.restitution = borderRestitution;
		fdef.filter.categoryBits = holeCategory;

		const auto fixt = body->CreateFixture(&fdef);

		holeTopShape.reset(shape);
		holeTopBody = body;
	}

	{
		const auto shape = new b2PolygonShape;
		shape->SetAsBox(12.5, 2);

		b2BodyDef bdef;

		bdef.awake = false;
		bdef.allowSleep = true;
		bdef.active = true;
		bdef.type = b2_kinematicBody;
		bdef.position.x = -100.0f;

		const auto body = world.CreateBody(&bdef);

		b2FixtureDef fdef;
		fdef.shape = shape;
		fdef.friction = borderFriction;
		fdef.restitution = borderRestitution;
		fdef.filter.categoryBits = holeCategory;

		const auto fixt = body->CreateFixture(&fdef);

		holeBottomShape.reset(shape);
		holeBottomBody = body;
	}

	ComputePoints();
	RespawnBorders(true);
}

App::~App() {
	for (auto& b : playerBodies)
		world.DestroyBody(b);
	for (auto& b : playerEyesBodies)
		world.DestroyBody(b);
	for (auto& b : borderBodies)
		world.DestroyBody(b);

	world.DestroyBody(holeTopBody);
	world.DestroyBody(holeBottomBody);
	world.DestroyBody(topObstacle);
	world.DestroyBody(bottomObstacle);
	world.DestroyBody(bottomHighObstacle);
}

void App::Init() {
}

void App::ComputeCampPos(double dt) {
	camPos = camPos + (playerBodiesPointsMiddle.x - camPos) * dt * playerGameVelocity;
}

void App::ComputePoints() {
	playerBodiesPointsMiddle = { 0.0f, 0.0f };
	playerVelocityMiddle = { 0.0f, 0.0f };
	auto count = 0;

	for (auto i = 0; i < playerBodies.size(); i++) {
		playerBodiesPoints[i] = playerBodies[i]->GetPosition();

		if (!playerBodiesFallen[playerBodies[i]])
		{
			playerBodiesPointsMiddle += playerBodiesPoints[i];
			playerVelocityMiddle += playerBodies[i]->GetLinearVelocity();
			count++;
		}
	}

	playerBodiesPointsMiddle = (1.0f / count) * playerBodiesPointsMiddle;
	playerVelocityMiddle = (1.0f / count) * playerVelocityMiddle;

// eye points

	for (auto i = 0; i < playerEyesBodies.size(); i++)
		playerEyesBodiesPoints[i] = playerEyesBodies[i]->GetPosition();
}

void App::BodyCrossBody(double dt) {
	for (auto& b1 : playerBodies) {
		if (!playerBodiesFallen[b1]) {
			b1->SetAngularVelocity(dt * playerAngleState);

			for (auto& b2 : playerBodies) {
				if (!playerBodiesFallen[b2]) {
					const auto dir = b1->GetPosition() - b2->GetPosition();
					b2->ApplyForceToCenter(playerForceState * dir, true);
				}
			}
		}
	}
}

void App::BodyCrossEye() {
	for (auto& e : playerEyesBodies) {
		for (auto& b : playerBodies) {
			if (!playerBodiesFallen[b]) {
				const auto dir = b->GetPosition() - e->GetPosition();
				e->ApplyForceToCenter(playerEyeBodyForceState * dir, true);
			}
		}
	}
}

void App::EyesAntigrav() {
	for (auto& e : playerEyesBodies) {
		e->ApplyForceToCenter({ 0.0, playerEyeAntigravState }, true);
	}
}

void App::RespawnBorders(bool force) {
	const auto holeChoice = rand() % 3;
	const auto doHole1 = holeChoice == 1;
	const auto doHole2 = holeChoice == 2;
	const auto hole1 = doHole1 ? 15 : 0;
	const auto hole2 = doHole2 ? 15 : 0;

	if (force) {
		borderBodies[0]->SetTransform({ camPos, 5 }, 0);
		borderBodies[1]->SetTransform({ camPos, 50 }, 0);
		borderBodies[2]->SetTransform({ borderBodies[0]->GetPosition().x + 200 + hole1, 5 }, 0);
		borderBodies[3]->SetTransform({ borderBodies[1]->GetPosition().x + 200 + hole2, 50 }, 0);

		if (doHole1) {
			holeBottom = { borderBodies[0]->GetPosition().x + 100 + hole1 * 0.5f, 5.0f };
			holeBottomBody->SetTransform({ borderBodies[0]->GetPosition().x + 100 + hole1 * 0.5f, 5.0f }, 0);
		}

		if (doHole2) {
			holeTop = { borderBodies[1]->GetPosition().x + 100 + hole2 * 0.5f, 50.0f };
			holeTopBody->SetTransform({ borderBodies[1]->GetPosition().x + 100 + hole2 * 0.5f, 50.0f }, 0);
		}

		borderBodies[2]->SetAwake(false);
		borderBodies[3]->SetAwake(false);
	}
	else {
		if (borderBodies[0]->GetPosition().x - camPos < - 125.0) {
			borderBodies[0]->SetTransform({ borderBodies[2]->GetPosition().x + 200 + hole1, 5 }, 0);
			borderBodies[0]->SetAwake(false);

			if (doHole1) {
				holeBottom = { borderBodies[2]->GetPosition().x + 100 + hole1 * 0.5f, 5.0f };
				holeBottomBody->SetTransform({ borderBodies[2]->GetPosition().x + 100 + hole1 * 0.5f, 5.0f }, 0);
			}
		} else if (borderBodies[2]->GetPosition().x - camPos < - 125.0) {
			borderBodies[2]->SetTransform({ borderBodies[0]->GetPosition().x + 200 + hole1, 5 }, 0);
			borderBodies[2]->SetAwake(false);

			if (doHole1) {
				holeBottom = { borderBodies[0]->GetPosition().x + 100 + hole1 * 0.5f, 5.0f };
				holeBottomBody->SetTransform({ borderBodies[0]->GetPosition().x + 100 + hole1 * 0.5f, 5.0f }, 0);
			}
		}

		if (borderBodies[1]->GetPosition().x - camPos < - 125.0) {
			borderBodies[1]->SetTransform({ borderBodies[3]->GetPosition().x + 200 + hole2, 50 }, 0);
			borderBodies[1]->SetAwake(false);

			if (doHole2) {
				holeTop = { borderBodies[3]->GetPosition().x + 100 + hole2 * 0.5f, 50.0f };
				holeTopBody->SetTransform({ borderBodies[3]->GetPosition().x + 100 + hole2 * 0.5f, 50.0f }, 0);
			}
		} else if (borderBodies[3]->GetPosition().x - camPos < - 125.0) {
			borderBodies[3]->SetTransform({ borderBodies[1]->GetPosition().x + 200 + hole2, 50.0f }, 0);
			borderBodies[3]->SetAwake(false);

			if (doHole2) {
				holeTop = { borderBodies[1]->GetPosition().x + 100 + hole2 * 0.5f, 50.0f };
				holeTopBody->SetTransform({ borderBodies[1]->GetPosition().x + 100 + hole2 * 0.5f, 50.0f }, 0);
			}
		}
	}
}

void App::RespawnObstacles(bool force) {
	const auto choice = rand() % 3;

	if (bottomObstacle->GetPosition().x - playerBodiesPointsMiddle.x < -75.0f
		&& bottomHighObstacle->GetPosition().x - playerBodiesPointsMiddle.x < -75.0f
		&& topObstacle->GetPosition().x - playerBodiesPointsMiddle.x < -75.0f) {
		if (choice == 0) {
			auto x = playerBodiesPointsMiddle.x + 75.0f;
			if (std::abs(holeBottom.x - x) < 13) x = holeBottom.x +
													 (rand() % 2 ? 13 : - 13);
			bottomObstacle->SetTransform(
				{ x, 10.0f }, bottomObstacle->GetAngle());
		}
		else if (choice == 1) {
			auto x = playerBodiesPointsMiddle.x + 75.0f;
			if (std::abs(holeBottom.x - x) < 13) x = holeBottom.x +
													 (rand() % 2 ? 13 : - 13);
			bottomHighObstacle->SetTransform(
				{ x, 14.0f }, bottomObstacle->GetAngle());
		}
		else if (choice == 2) {
			auto x = playerBodiesPointsMiddle.x + 75.0f;
			if (std::abs(holeTop.x - x) < 13) x = holeTop.x +
												  (rand() % 2 ? 13 : - 13);
			topObstacle->SetTransform(
				{ x, 39.0f }, topObstacle->GetAngle());
		}
	}
}

void App::FalloutHoles() {
	for (auto& b : playerBodies) {
		if (playerStatePoint == 1 &&
			std::abs(b->GetPosition().x - holeBottom.x) < 2.5f  &&
			std::abs(b->GetPosition().y - holeBottom.y) < 10.0f) {
			b->SetLinearVelocity({ 0.0f, - 30.0f });

		} else if (playerStatePoint == 2 &&
				   std::abs(b->GetPosition().x - holeTop.x) < 2.5f &&
				   std::abs(b->GetPosition().y - holeTop.y) < 25.0f) {
			b->SetLinearVelocity({ 0.0f, 40.0f });
		} else if (!playerBodiesFallen[b]) {
			// const auto vel = b->GetLinearVelocity();
			// b->SetLinearVelocity({ playerGameVelocity, vel.y });
			b->ApplyForceToCenter({ playerGameVelocity, 0.0f }, true);
		}
	}
}

void App::RespawnParticles() {
	for (auto& b1 : playerBodies) {
		if (b1->GetPosition().x < camPos - 25.0f) {
			b1->SetTransform(
				{ playerBodiesPointsMiddle.x, playerBodiesPointsMiddle.y },
				b1->GetAngle());
			b1->SetLinearVelocity(playerVelocityMiddle);
		}
	}
}

void App::RespawnEyes() {
	for (auto& e : playerEyesBodies) {
		if (e->GetPosition().y < 2.0
			|| e->GetPosition().y > 45.0f) {
			e->SetTransform({ camPos - 25.0f, 15.0f }, 0);
		}
	}
}

void App::UpdateBalancer(double dt) {
	playerState += (playerStatePoints[playerStatePoint] - playerState) * dt * 10;

	if (playerState <= 0.5) {
		const auto remap = playerState * 2.0f;
		for (auto& p : balancer) p.second = (1.0f - remap) * p.first[0] + remap * p.first[1];
	}
	else {
		const auto remap = (playerState - 0.5f) * 2.0f;
		for (auto& p : balancer) p.second = (1.0f - remap) * p.first[1] + remap * p.first[2];
	}
}

void App::Update(double dt) {
	progress.Update(dt);
	shaker.Update(dt);

	static float time = 0.0f;

	if (!progress.IsPaused()) {
		time += dt;

		world.Step(dt, 1, 2);

		world.SetGravity({ 0, playerGravityState });

		// move back

		const auto camPosDelta = camPos - prevCamPos;
		prevCamPos = camPos;

		backPos -= camPosDelta * 0.75;

		if (backPos < - 100) {
			backPos = 0.0f;
		}

		// convex points

		ComputePoints();

		// cam pos

		ComputeCampPos(dt);

		// body <-> body

		BodyCrossBody(dt);

		// eyes antigrav

		EyesAntigrav();

		// body <-> eye

		BodyCrossEye();

		// borders respawn

		RespawnBorders(false);

		// respawn obstacles

		RespawnObstacles(false);

		// fall out in holes

		FalloutHoles();

		// particles respawn

		RespawnParticles();

		//respawn eyes

		RespawnEyes();

		// handle balancer

		UpdateBalancer(dt);

		// mark stuck bodies

		if (time > 2.0) {
			auto fallenCount = 0;

			for (auto& b : playerBodies) {
				if (std::abs(b->GetLinearVelocity().x) < 6.0f
					&& std::abs(b->GetLinearVelocity().y) < 10.0f
					) {
					playerBodiesStucks[b] += dt;

					if (playerBodiesStucks[b] > 0.75f) {
						playerBodiesFallen[b] = true;
						fallenCount++;
					}
				}
				else {
					playerBodiesStucks[b] = 0;

					if (b->GetPosition().y < 0.0
						|| b->GetPosition().y > 52.0f) {
						playerBodiesFallen[b] = true;
						fallenCount++;
					}
					else {
						playerBodiesFallen[b] = false;
					}
				}
			}

			if (fallenCount > playerBodiesCount / 3 ) {
				progress.RestartGame();

				for (auto& b : playerBodies) {
					const float x = 0 + rand() % 50;
					const float y = 15 + rand() % 30;
					b->SetTransform({ x, y }, 0);
					playerBodiesFallen[b] = false;
					playerBodiesStucks[b] = 0.0f;
				}

				for (auto& e : playerEyesBodies) {
					const float x = 0 + rand() % 50;
					const float y = 15 + rand() % 30;
					e->SetTransform({ x, y }, 0);
				}

				camPos = 0;
				prevCamPos = camPos;
				backPos = 0.0f;
				time = 0.0f;

				ComputePoints();
				RespawnBorders(true);
				RespawnObstacles(true);
				GoSolid();

				playerGameVelocity = playerGameVelocityDefault;
			}
		}
	}
}

static void checkGlError(const char* op) {
	for (GLint error = glGetError(); error; error = glGetError()) {
		LOGI("after %s() glError (0x%x)\n", op, error);
	}
}

void drawTexture(const GLuint texture,
				 const float x,
				 const float y,
				 const float width,
				 const float height,
				 const float scaleTcX,
				 const float scaleTcY) {

	b2Vec2 points[] = {
		{x,y},
		{x + width, y},
		{x,y + height},
		{x + width,y + height}
	};

	b2Vec2 tex[] = {
		{0,0},
		{width * scaleTcX,0},
		{0,1},
		{width * scaleTcX,1}
	};

	glColor4f(0.5, 0.5, 0.5, 1);

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, points);
	glTexCoordPointer(2, GL_FLOAT, 0, tex);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
}

void App::Draw() {
	SetProjection(fieldWidth, fieldHeight);

	SetTranslate(0, 0);
	progress.Draw();

	if (!progress.IsPaused()) {

		// background

		SetTranslate(0, 0);
		shaker.ApplyMatrix();

		drawTexture(texture, backPos, 0, 100, fieldHeight, 1.0f / 100, 1.0f);
		drawTexture(texture, backPos + 100, 0, 100, fieldHeight, 1.0f / 100, 1.0f);

		// debug data

		SetTranslate(25 - camPos, 0);
		shaker.ApplyMatrix();

		world.DrawDebugData();

		// platforms

		drawTexture(bottom,
					borderBodies[0]->GetPosition().x - 100,
					0,
					200,
					8,
					1.0f / fieldWidth * 3.0f,
					1.0f);
		drawTexture(top,
					borderBodies[1]->GetPosition().x - 100,
					fieldHeight - 13,
					200,
					13,
					1 / fieldWidth * 2.0f,
					1.0f);
		drawTexture(bottom,
					borderBodies[2]->GetPosition().x - 100,
					0,
					200,
					8,
					1.0f / fieldWidth * 3.0f,
					1.0f);
		drawTexture(top,
					borderBodies[3]->GetPosition().x - 100,
					fieldHeight - 13,
					200,
					13,
					1 / fieldWidth * 2.0f,
					1.0f);

		// creature

		drawCreature(playerBodiesPoints, b2Color(playerColorRState, playerColorGState, playerColorBState), 1, playerStatePoint);
		drawEyes(playerEyesBodiesPoints, playerBodiesPoints);

		// numbers

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

void App::GoSolid() {
	playerStatePoint = 0;

	shaker.Shake();

	for (auto& e : playerEyesBodies) {
		b2Filter f = e->GetFixtureList()->GetFilterData();
		f.maskBits = borderCategory | playerBodyCategory | playerEyeCategory;
		e->GetFixtureList()->SetFilterData(f);
	}

	for (auto& b1 : playerBodies) {
		const auto vel = b1->GetLinearVelocityFromLocalPoint({ 0, 0});
		b1->SetLinearVelocity(vel + b2Vec2 { 0, -100.0 });

		b2Filter f = b1->GetFixtureList()->GetFilterData();
		f.maskBits = borderCategory | holeCategory | playerBodyCategory | playerEyeCategory;
		b1->GetFixtureList()->SetFilterData(f);
	}
}

void App::GoLiquid() {
	playerStatePoint = 1;

	for (auto& e : playerEyesBodies) {
		b2Filter f = e->GetFixtureList()->GetFilterData();
		f.maskBits = borderCategory | playerBodyCategory | playerEyeCategory;
		e->GetFixtureList()->SetFilterData(f);
	}

	for (auto& b1 : playerBodies) {
		b2Filter f = b1->GetFixtureList()->GetFilterData();
		f.maskBits = borderCategory | playerBodyCategory | playerEyeCategory;
		b1->GetFixtureList()->SetFilterData(f);
	}
}

void App::GoGas() {
	playerStatePoint = 2;

	for (auto& e : playerEyesBodies) {
		b2Filter f = e->GetFixtureList()->GetFilterData();
		f.maskBits = borderCategory | playerEyeCategory;
		e->GetFixtureList()->SetFilterData(f);
	}

	for (auto& b1 : playerBodies) {
		const auto vel = b1->GetLinearVelocityFromLocalPoint({ 0, 0});
		b1->SetLinearVelocity(vel + b2Vec2 { 0, 40.0 });

		b2Filter f = b1->GetFixtureList()->GetFilterData();
		f.maskBits = borderCategory | playerBodyCategory;
		b1->GetFixtureList()->SetFilterData(f);
	}
}

void App::TouchEnd(int player, float newX, float newY) {
	if (progress.IsPaused()) {
		progress.Touch(newX, newY);
		return;
	}

	playerStatePoint++;
	if (playerStatePoint >= playerStatePoints.size()) {
		playerStatePoint = 0;
	}

	// drop eyes

	if  (playerStatePoint == 2) {
		GoGas();
	} else if (playerStatePoint == 0) {
		GoSolid();
	} else if (playerStatePoint == 1) {
		GoLiquid();
	}
}

void App::ScreenSize(float newWidth, float newHeight) {
	screenWidth = (newWidth > 0.1 ? newWidth : 1.0f);
	screenHeight = (newHeight > 0.1 ? newHeight : 1.0f);
	fieldHeight = fieldWidth * screenHeight / screenWidth;

	progress.FieldSize(fieldWidth, fieldHeight);
}

}
