#include "enemies.hpp"

void enemy1::update()
{
	// updates the position according to the velocity
	Position = Vector2Add(Position, Velocity);	
}

void enemy1::draw()
{
	// draw the body the enemy1
	DrawRectangleV(Vector2Add(Position, { -10.0f,-10.0f }), { 20.0f,20.0f }, ORANGE);
	// draws the health bar
	DrawRectangleV(Vector2Add(Position, { -10.0f,25.0f }), { 20.0f,5.0f }, GRAY);
	DrawRectangleV(Vector2Add(Position, { -10.0f,25.0f }), { ((float)this->healthPt / (float)this->healthPtMax) * 20.0f,5.0f }, GREEN);
	//DrawCircleLinesV(Position, HitRadius, GREEN);


}

std::pair<Vector2, float> enemy1::collisionData()
{
	return {Position,HitRadius};
}

void enemy1::collisionHandler(Vector2 collisionDirection, float collisionElasticity, float depth)
{
	// how to implement?
	// 1st step we just find the compoenet of velocity along the normal and reverse it using 
	// imp assuming that the one that we are colliding is sufficently large why?
	// so that we can use vf = - e * vi

	this->Position = Vector2Add(this->Position, Vector2Scale(Vector2Normalize(collisionDirection), depth));
	// considering the direction of collison init to be towards the center
	// normalizing the direction vector
	Vector2 collisionDirectionNm = Vector2Scale(Vector2Normalize(collisionDirection), -1);
	// considering the direction of collision Normalized to be away from the center

	float dot = Vector2DotProduct(Velocity, collisionDirectionNm);

	if (dot > 0) {
		// now giving impluse to update the velocity   [ delta v = vf - vi = -(1+e)*vi ]
		Vector2 impulse = Vector2Scale(collisionDirectionNm, (1.0f + collisionElasticity) * dot);
		this->Velocity = Vector2Subtract(this->Velocity, impulse);
	}
}

void enemy2::update(const Vector2& target, MazeMaker& maze)
{
	Vector2 lineOfSight = Vector2Subtract(target, Position);

	// if there are no obstruction
	if (!maze.losCheckObs(Position, lineOfSight)) {
		Velocity = Vector2Scale(Vector2Normalize(lineOfSight), maxVelocityPerDim);
	}

	Position = Vector2Add(Position, Velocity);
}



void enemy2::draw()
{
	// draw the body the enemy1
	DrawRectangleV(Vector2Add(Position, { -10.0f,-10.0f }), { 20.0f,20.0f }, PINK);
	// draws the health bar
	DrawRectangleV(Vector2Add(Position, { -10.0f,25.0f }), { 20.0f,5.0f }, GRAY);
	DrawRectangleV(Vector2Add(Position, { -10.0f,25.0f }), { ((float)this->healthPt / (float)this->healthPtMax) * 20.0f,5.0f }, GREEN);


}

void enemy3::update(const Vector2& target, MazeMaker& maze, shooter* gun)
{
	Vector2 lineOfSight = Vector2Subtract(target, Position);

	// if there are no obstruction
	if (!maze.losCheckObs(Position, lineOfSight)) {
		if(static_cast<int>(GetTime()*100)%(100) == 0)gun->spawnBullet(Vector2Normalize(lineOfSight), Vector2Add(Vector2Scale(Vector2Normalize(lineOfSight),35.0f),Position), 10.0f, 10.0f, 6.0f);
		Velocity = Vector2Scale(Vector2Normalize(lineOfSight), maxVelocityPerDim);
	}


	Position = Vector2Add(Position, Velocity);
}

void enemy3::draw()
{
	// draw the body the enemy1
	DrawRectangleV(Vector2Add(Position, { -10.0f,-10.0f }), { 20.0f,20.0f }, YELLOW);
	// draws the health bar
	DrawRectangleV(Vector2Add(Position, { -10.0f,25.0f }), { 20.0f,5.0f }, GRAY);
	DrawRectangleV(Vector2Add(Position, { -10.0f,25.0f }), { ((float)this->healthPt / (float)this->healthPtMax) * 20.0f,5.0f }, GREEN);

}
