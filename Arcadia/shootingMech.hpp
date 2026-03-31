#pragma once
#include <vector>
#include "raylib.h"
#include "entity.hpp"
#include "mazeMaker.hpp"
#include "raymath.h"
#include <utility>

// bullet class
class bullet {
private:
	// movement
	Vector2 velocity;
	Vector2 position;
	// hit radius
	float hitRadius;
	// damage
	float damage;
	//time of creation
	double timeOfCreation;

public :

	bullet(Vector2 velocity, Vector2 position, float hitRadius, float damage);

	// return the collsion data of the bullet
	std::pair<Vector2, float> collisionData();

	// return the TOC
	double giveTOC();

	// damage of the bullet
	float giveDmg();

	// updation of the bullet
	void update();
};

// shooter class
// so this is the class that actually handles the bullets
class shooter {

private:
	// container having data of the all the bullets
	std::vector<bullet> bulletsData;
	double timeOfExpiry;
public:
	shooter();
	// spawns and puts the bullet into the container
	void spawnBullet(Vector2 Direction, Vector2 Position, float hitRadius, float damage,float speed);
	// updates and check for any collision
	void update(std::vector<entity*>& entities , MazeMaker& maze);
	//drawing all the bullets
	void draw();
};