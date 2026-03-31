#pragma once 
#include "raylib.h"
#include <utility>

class entity {
protected:
	// current healthpoints
	int healthPt;
	// max health or total health entity can have
	int healthPtMax;
	// body touch damage
	int bodyDamage;
	// entity status
	bool isAlive;
public:
	// constructor
	entity(int healthPtMax, int bodyDamage) : healthPt(healthPtMax) , healthPtMax(healthPtMax), bodyDamage(bodyDamage), isAlive(true) {
	}

	// for the children
	virtual void update() = 0; 
	virtual std::pair<Vector2, float> collisionData() = 0;
	virtual void draw() = 0;
	virtual void collisionHandler(Vector2 collisionDirection, float collisionElasticity,float depth) = 0;
	virtual void setPosition(const Vector2& newPosition) {}


	// general damage control / resolver
	//===========================================================================================================
	void takeDmg(int DamageTaken) {
		healthPt -= DamageTaken;
		if (healthPt <= 0) isAlive = false;
	}

	int giveDmg() {
		return bodyDamage;
	}
	//===========================================================================================================

	// the status of the entity
	bool aliveStatus() {
		return isAlive;
	}

};