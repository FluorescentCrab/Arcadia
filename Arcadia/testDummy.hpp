#pragma once

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include "entity.hpp"
#include <cmath>

class testDummy : public entity {
private :
	// movement related
	Vector2 Position;
	Vector2 Velocity = { 0.5f,0.0f };
	Vector2 Direction = { 0.0f , 0.0f };
	float HitRadius = 15.0f;
	float maxVelocityPerDim = 10.0f;
	float veloctyInc = 2.0f;
	float veloctyDrag = 0.1f;


public:
	testDummy(Vector2 Position, int healthPtMax, int bodyDamage) : Position(Position), entity(healthPtMax, bodyDamage) {

	}
	void update() override;
	void draw() override;
	std::pair<Vector2, float> collisionData() override;
	void collisionHandler(Vector2 collisionDirection, float collisionElasticity,float depth) override;
};