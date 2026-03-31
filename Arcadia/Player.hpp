#pragma once

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include "entity.hpp"
#include <cmath>
#include <iostream>
#include <deque>
#include "shootingMech.hpp"
#include "global.hpp"

// now we have to make over certain aspects like 
// in constructor we will take the level and according it we will set

// Position | Color | velocityDrag | velocityInc 

// now in level 1 => player will have : quick movement ; dash ; shoot : fully yellow

// level 2 => player will have : somewhat slower movement ; dash ; orange

// level 3 => player will have : only movement ; more redder

// level 4 => slow as hell ; red as walls

class Player : public entity {
	private:
			struct TrailStep {
		Vector2 pos;
		Vector2 dir;
	};
	std::deque<TrailStep> trail;

	const int maxTrailLength = 15; 
	int trailUpdateCounter = 0;
		// movement related
		Vector2 position;
		Vector2 velocity;
		Vector2 direction;

		// enviroment drag
		float velocityInc;
		float velocityDrag;

		// used to make the hit box
		float hitRadius;

		// color of player 
		Color playerColor;

		// blinding angle offset
		float angleOffset;

		// camera
		Camera2D* gameCamera;	

		GameStatus curlevel;

		
	public :
		// constructor
		Player(Vector2 position, int healthPtMax, int bodyDamage, Camera2D& camera , GameStatus curlevel);

		// updates only movement for now
		using entity::update;
		void update() override;
		void update(shooter& gun);

		void setPosition(const Vector2& newPosition) override;

		// draws the player and the health bar
		void draw() override;

		// return the position and the hit radius
		std::pair<Vector2, float> collisionData() override;

		// handle if collision happens and how the player reacts to the collison
		void collisionHandler(Vector2 collisionDirection, float collisionElasticity,float depth) override;
};