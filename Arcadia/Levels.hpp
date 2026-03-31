#pragma once 
#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include <cmath>
#include <cstdint>
#include "mazeMaker.hpp"
#include <iostream>
#include "Player.hpp"
#include "shootingMech.hpp"
#include "enemies.hpp"
#include <deque>
#include "damageResolver.hpp"

// so makeing a parent class for the actual levels will act as a template or ghost class
class Level {
	// tells if the level is complete or not
	// so only a child can tell the status of the level so child can call these functions to tell whether to restart or load next
protected:
	bool isComplete;
	bool isRestart;
	void LevelComplete() { isComplete = true; }
	void GameRestart() { isRestart = true; }
public:
	// constrctor sets the level to false ; level just began so isComplete should be false
	Level() {
		isComplete = false;
		isRestart = false;
	}

	// this makes it so that the compiler always refers to the childs update functions not the parents
	virtual void update()= 0;
	virtual void draw()= 0;

	// same for destructor ( a bit complicated than that but ok )
	virtual ~Level() = default;

	// this is for game manager 
	bool getStatus() { return isComplete; }
	bool getRestartStatus() { return isRestart; };
};

// so now we have to make something class level maker or simply maze maker 

// main menu
class mainMenu : public Level {
private:
	bool isSeq;
	double timer;
	// static cords
	Vector2 tp, lp, rp, bp;
	// dynamic cords
	Vector2 tpd, lpd, rpd, bpd;

	// for text
public :
	mainMenu();
	void update() override;
	void draw() override;
};

class level1 : public Level {
private:
	MazeMaker* maze1;
	Camera2D* cam;
	Player* player;
	shooter* gun;
	damageResolver* damageRslvr;

	// could have made an array but meh now to late will do it later
	bool isOrb1Collected;
	bool isOrb2Collected;
	bool isOrb3Collected;

	Vector2 orb1;
	Vector2 orb2;
	Vector2 orb3;

	double timeStart;

	bool isStart;

	std::vector<entity*> entityList;
	
public:
	level1();
	// ovverriding the virtual functions
	void update() override;
	void draw() override;
	~level1();
};


class level2 : public Level {
private:
	MazeMaker* maze1;
	Camera2D* cam;
	Player* player;
	shooter* gun;
	damageResolver* damageRslvr;

	int timeStart;

	bool isStart;

	std::vector<entity*> entityList;
	std::vector<std::pair<Vector2, bool>> orbs;

public:
	level2();
	void update() override;
	void draw() override;
	~level2();
};


class level3 : public Level {
private:
	MazeMaker* maze1;
	Camera2D* cam;
	Player* player;
	shooter* gun;
	damageResolver* damageRslvr;

	double timeStart;

	bool isStart;

	bool isOrb1Collected;
	bool isOrb2Collected;
	bool isOrb3Collected;

	Vector2 orb1, orb1Start, orb1Pull, orb1Target;
	Vector2 orb2, orb2Start, orb2Pull, orb2Target;
	Vector2 orb3, orb3Start, orb3Pull, orb3Target;

	std::vector<entity*> entityList;

public:
	level3();
	void update() override;
	void draw() override;
	~level3();
};

class level4 : public Level {
private:
	MazeMaker* maze1;
	Camera2D* cam;
	Player* player;
	shooter* gun;
	damageResolver* damageRslvr;

	std::deque<Vector2> playerTrail; // Stores the player's path
	Vector2 shadowPos;
	int trailDelay = 35; // How many frames behind the shadow is (approx 1.5 seconds at 60fps)

	bool levelStarted;

	bool isOrbCollected;
	Vector2 orb;

	double timeStart; 
	float shadowAngle = 0.0f;
	bool isStart;
	std::vector<entity*> entityList;

public:
	level4();
	void update() override;
	void draw() override;
	~level4();
};

class endScreen : public Level {
private:
	bool isSeq;
	double timer;
	// static cords
	Vector2 tp, lp, rp, bp;
	// dynamic cords
	Vector2 tpd, lpd, rpd, bpd;

	// for text
public:
	endScreen();
	void update() override;
	void draw() override;
};


