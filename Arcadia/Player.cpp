#include "Player.hpp"


Player::Player(Vector2 position, int healthPtMax, int bodyDamage, Camera2D& camera, GameStatus curlevel) : entity(healthPtMax,bodyDamage)
{
	// general init 
	this->position = position;
	this->gameCamera = &camera;
	velocity = {0.0f,0.0f};
	direction = { 0.0f,0.0f };
	this->curlevel = curlevel;

	// hit radius is going to small for initial levels

	switch (curlevel) {
	case GameStatus::level_1:
		// level 1 player init
		hitRadius = 10.0f;
		velocityInc = 5.0f;
		velocityDrag = 0.5f;
		playerColor = { 255,255,0,255 };
		angleOffset = 80;
		break;
	case GameStatus::level_2:
		// level 2 player init
		hitRadius = 10.0f;
		velocityInc = 4.0f;
		velocityDrag = 0.5f;
		playerColor = { 255,255,0,255 };
		angleOffset = 60;
		break;
	case GameStatus::level_3:
		// level 3 player init
		hitRadius = 10.0f;
		velocityInc = 3.0f;
		velocityDrag = 0.5f;
		playerColor = { 255,255,0,255 };
		angleOffset = 30;
		break;
	case GameStatus::level_4:
		// level 4 player init
		hitRadius = 10.0f;
		velocityInc = 10.0f;
		velocityDrag = 1.0f;
		playerColor = { 255,255,0,255 };
		angleOffset = 15;
		break;
	default:
		// do nothin' 
		break;
	}
}

void Player::update()
{
}

void Player::update(shooter& gun)
{
	//std::cout << this->position.x << " " << this->position.y << std::endl;
	// general updation
	//====================================================================================================================

	// direction of the shooting mech.
	direction = Vector2Scale(Vector2Subtract(GetScreenToWorld2D(GetMousePosition(), *gameCamera), position), 1.0f / Vector2Length(Vector2Subtract(GetScreenToWorld2D(GetMousePosition(), *gameCamera), position)));

	//retard the velocity
	if (abs(velocity.x) > 0) velocity.x -= velocityDrag * velocity.x;
	if (abs(velocity.y) > 0) velocity.y -=  velocityDrag * velocity.y;

	// update the velocity
	if (IsKeyDown('W')) {
		velocity.y-= velocityInc;
	}
	if (IsKeyDown('S')) {
		velocity.y+= velocityInc;
	}
	if (IsKeyDown('A')) {
		velocity.x-= velocityInc;
	}
	if (IsKeyDown('D')) {
		velocity.x+= velocityInc;
	}
	
	// update the position
	position = Vector2Add(position, velocity);

	trailUpdateCounter++;
	if (trailUpdateCounter >= 2) { // Add a trail point every 2 frames
		trail.push_front({ position, direction });
		if (trail.size() > maxTrailLength) {
			trail.pop_back();
		}
		trailUpdateCounter = 0;
	}

	// update the camera position
	gameCamera->target = this->position;
	
	//====================================================================================================================

	// dash mech.
	// only do dash if we are level 1 and 2 
	if (IsKeyPressed('E') && ((curlevel == GameStatus::level_1) || (curlevel == GameStatus::level_2))) {
		velocity = Vector2Add(velocity, Vector2Scale(Vector2Normalize(direction), 100.0f));
	}

	// shooting mech.
	// only happens in level 1
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && (curlevel == GameStatus::level_1)) {
		gun.spawnBullet(direction, Vector2Add(position, Vector2Scale(direction, hitRadius + 10.0f)),5.0f,1.0f,20.0f);
	}
	

}

void Player::setPosition(const Vector2& newPosition)
{
	position = newPosition;
}

void Player::draw()
{
	for (size_t i = 0; i < trail.size(); i++) {
		float alphaFactor = 1.0f - ((float)i / (float)trail.size());
		Color trailColor = { 255, 255, 0, (unsigned char)(150 * alphaFactor) };
		float trailRadius = hitRadius * (0.4f + 0.6f * alphaFactor);
		Vector2 tPos = trail[i].pos;
		Vector2 tDir = Vector2Normalize(trail[i].dir);

		DrawTriangle(
			Vector2Add(Vector2Rotate(Vector2Scale(tDir, trailRadius + 5), 2.09439f), tPos),
			Vector2Add(Vector2Scale(tDir, trailRadius + 5), tPos),
			Vector2Add(Vector2Rotate(Vector2Scale(tDir, trailRadius + 5), -2.09439f), tPos),
			trailColor
		);
	}

	DrawLineV(position, GetScreenToWorld2D(GetMousePosition(), *gameCamera), RED);
	DrawCircleV(Vector2Add(position, Vector2Scale(direction, hitRadius + 5.0f)), 2.0f, PINK);

	DrawTriangle(
		Vector2Add(Vector2Rotate(Vector2Scale(Vector2Normalize(direction), hitRadius + 5), 2.09439f), position),
		Vector2Add(Vector2Scale(Vector2Normalize(direction), hitRadius + 5), position),
		Vector2Add(Vector2Rotate(Vector2Scale(Vector2Normalize(direction), hitRadius + 5), -2.09439f), position),
		playerColor
	);

	DrawRectangleV(Vector2Add(position, { -10.0f, 25.0f }), { 20.0f, 5.0f }, GRAY);
	DrawRectangleV(Vector2Add(position, { -10.0f, 25.0f }), { ((float)this->healthPt / (float)this->healthPtMax) * 20.0f, 5.0f }, GREEN);

	float angle = RAD2DEG * Vector2Angle(direction, { 1.0f, 0.0f });
	DrawCircleSector(position, GetScreenWidth(), -angle - angleOffset, -angle - (360 - angleOffset), 30, BLACK);
	DrawCircleSectorLines(position, GetScreenWidth(), -angle - angleOffset, -angle - (360 - angleOffset), 30, playerColor);

}
std::pair<Vector2, float> Player::collisionData()
{
	return {position,hitRadius };
}

void Player::collisionHandler(Vector2 collisionDirection, float collisionElasticity,float depth)
{

	// how to implement?
	// 1st step we just find the compoenet of velocity along the normal and reverse it using 
	// imp assuming that the one that we are colliding is sufficently large why?
	// so that we can use vf = - e * vi

	this->position = Vector2Add(this->position, Vector2Scale(Vector2Normalize(collisionDirection), depth));
	// considering the direction of collison init to be towards the center
	// normalizing the direction vector
	Vector2 collisionDirectionNm = Vector2Scale(Vector2Normalize(collisionDirection),-1);
	// considering the direction of collision Normalized to be away from the center

	float dot = Vector2DotProduct(velocity, collisionDirectionNm);

	if (dot > 0) {
		// now giving impluse to update the velocity   [ delta v = vf - vi = -(1+e)*vi ]
		Vector2 impulse = Vector2Scale(collisionDirectionNm, (1.0f + collisionElasticity) * dot);
		this->velocity = Vector2Subtract(this->velocity, impulse);
	}

}
