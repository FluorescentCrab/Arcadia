#include "testDummy.hpp"

void testDummy::update()
{// constrain the velocity
	if (abs(Velocity.x) > maxVelocityPerDim) Velocity.x = std::signbit(Velocity.x) ? -maxVelocityPerDim : maxVelocityPerDim;
	if (abs(Velocity.y) > maxVelocityPerDim) Velocity.y = std::signbit(Velocity.y) ? -maxVelocityPerDim : maxVelocityPerDim;

	//update the position
	Position = Vector2Add(Position, Velocity);

}

void testDummy::draw()
{
    DrawRectangleV(Vector2Add(Position, { -10.0f,-10.0f }), { 20.0f,20.0f }, ORANGE);
    DrawCircleLinesV(Position, HitRadius, GREEN);

	DrawRectangleV(Vector2Add(Position, { -10.0f,25.0f }), { 20.0f,5.0f }, GRAY);
	DrawRectangleV(Vector2Add(Position, { -10.0f,25.0f }), { ((float)this->healthPt / (float)this->healthPtMax) * 20.0f,5.0f }, GREEN);

}

std::pair<Vector2, float> testDummy::collisionData()
{
    return {Position,HitRadius};
}

void testDummy::collisionHandler(Vector2 collisionDirection, float collisionElasticity,float depth)
{
	this->Position = Vector2Add(this->Position, Vector2Scale(Vector2Normalize(collisionDirection), depth));
	// considering the direction of collison init to be towards the center
	// normalizing the direction vector
	Vector2 collisionDirectionNm = Vector2Scale(Vector2Normalize(collisionDirection), -1);
	// considering the direction of collision Normalized to be away from the center

	float dot = Vector2DotProduct(Velocity, collisionDirectionNm);

	if (dot > 0) {
		Vector2 impulse = Vector2Scale(collisionDirectionNm, (1.0f + collisionElasticity) * dot);
		this->Velocity = Vector2Subtract(this->Velocity, impulse);
	}
}
