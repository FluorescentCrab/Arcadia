#include "entity.hpp"
#include "raylib.h"
#include "raymath.h"
#include "mazeMaker.hpp"
#include "shootingMech.hpp"

// normal enemy that just follows the pplayer 
// main problem i think preimtevely i think will happen is =>
// it will look at the player thrugh the wall and will get stuck 
// so for that we can either do the following 
// 1. make it move from point A to point B and if it sees the player then it chases the player
// now how will it see the player ; if i can draw a line directly from player to enemy without any wall b/w them , then 
// the enemy can seee the player
// 2. make a proper ai that sees the walls and navigates from exactly the middle of the free space and selects random points 
// in the map and goes there

// for shooting enemy just add a cooldown and shooting mech 

// i also want a rapidly chasing enemy 


// generic enemy does not attack just if you bum into it you and enemy both get damage
// knows the walls and just randomly moves 
class enemy1 : public entity {
protected:
	// movement related
	Vector2 Position;
	Vector2 Velocity = { 0.5f,0.0f };
	Vector2 Direction = { 0.0f , 0.0f };
	float HitRadius = 15.0f;
	float maxVelocityPerDim;
	/*float veloctyInc = 2.0f;
	float veloctyDrag = 0.1f;*/

public:
	enemy1(Vector2 Position, float velocity, int healthPtMax, int bodyDamage) : Position(Position), Velocity({static_cast<float>(GetRandomValue(-velocity,velocity)) ,static_cast<float>(GetRandomValue(-velocity,velocity)) }), maxVelocityPerDim(velocity), entity(healthPtMax, bodyDamage) {

	}
	using entity::update;
	virtual void update() override;
	virtual void draw() override;
	std::pair<Vector2, float> collisionData() override;
	void collisionHandler(Vector2 collisionDirection, float collisionElasticity, float depth) override;
};
// so for now the generic enemy is done

// moving onto the enemy type 2 chaser

class enemy2 : public enemy1 {

public:
	enemy2(Vector2 Position, float velocity, int healthPtMax, int bodyDamage) : enemy1(Position, velocity, healthPtMax, bodyDamage) {

	}
	using enemy1::update;
	void update(const Vector2& target,MazeMaker& maze);
	void draw() override;
};


// moves towards you and shoots

class enemy3 : public enemy1 {

public:
	enemy3(Vector2 Position, float velocity, int healthPtMax, int bodyDamage) : enemy1(Position, velocity, healthPtMax, bodyDamage) {

	}
	using enemy1::update;

	void update(const Vector2& target, MazeMaker& maze,shooter* gun);
	void draw() override;
};

