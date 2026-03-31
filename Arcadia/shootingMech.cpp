#include "shootingMech.hpp"

// the bullet class implementation
//============================================================================================================================================================================================
bullet::bullet(Vector2 velocity, Vector2 position, float hitRadius, float damage) : velocity(velocity),position(position),hitRadius(hitRadius),damage(damage) {
	timeOfCreation = GetTime();
}

double bullet::giveTOC()
{
	return timeOfCreation;
}

float bullet::giveDmg() {
	return damage;
}

std::pair<Vector2, float> bullet::collisionData() {
	return { position,hitRadius };
}

void bullet::update() {
	position = Vector2Add(position, velocity);
}

//============================================================================================================================================================================================

// shooter class implementation
//============================================================================================================================================================================================
shooter::shooter()
{
	timeOfExpiry = 5.0f;
}

void shooter::spawnBullet(Vector2 Direction, Vector2 Position, float hitRadius, float damage,float speed)
{
	// considering direction to be normalized
	bulletsData.push_back({ Vector2Scale(Direction,speed),Position,hitRadius,damage });
}

void shooter::update(std::vector<entity*>& entities, MazeMaker& maze)
{
	// so this gives us which bullets are to be deleted ; true -> delete and false -> dont delete
	std::vector<bool> flag(bulletsData.size(), false);

	// check for any collsion with any entity
	for (entity *e : entities) {
		for (auto it = bulletsData.begin(); it != bulletsData.end();it++) {
			if (CheckCollisionCircles(e->collisionData().first, e->collisionData().second, it->collisionData().first, it->collisionData().second) && e->aliveStatus()) {
				flag[it - bulletsData.begin()] = true;
				e->takeDmg(it->giveDmg());
			}
		}
	}

	// check if any bullet has reached the time expiray or has collided with wall
	for (auto it = bulletsData.begin(); it != bulletsData.end(); it++) {
		if (GetTime() - it->giveTOC() > timeOfExpiry) flag[it - bulletsData.begin()] = true;
		if(maze.isColliding(it->collisionData().first , it->collisionData().second))  flag[it - bulletsData.begin()] = true;
	}


	// if the bullets are flagged then delete them from the list
	//starting from the last so that we can delete the indexes without any problem
	for (int i = flag.size() - 1; i >= 0; i--) {
		if (flag[i]) {
			bulletsData.erase(i + bulletsData.begin());
		}
	}

	// updating the remaining bullets
	for (bullet& b : bulletsData) {
		b.update();
	}

}

// draw the bullets 
void shooter::draw() {
	// bullets are just basic circles for now
	for (bullet& b : bulletsData) {
		DrawCircleV(b.collisionData().first, b.collisionData().second, BLUE);
	}

}
