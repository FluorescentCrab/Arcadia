#pragma once 
#include <memory>
#include "Levels.hpp"
#include "global.hpp"

 // Note to self : next time dont implement functions in hpp , during building the solution it does not re commpile the hpp files so i had to 
 // manually remove the solution and remake it


// main game manager
class GameManager {

	//=================================================================================================================
	private :

	// current gameStatus
	GameStatus gameStatus; // intially kept the enum for gameStatus here but it later led to circular dependancy so keeping it in global 

	// points to the current level call for update and draw
	std::unique_ptr<Level> curLvl;

	// loading the next level
	void LoadNextLevel(){
		gameStatus = static_cast<GameStatus>(static_cast<int>(gameStatus) + 1);
		switch (gameStatus) {
		case GameStatus::level_1 :
				// create a pointer to Level 1
				curLvl = std::make_unique<level1>();
				break;
			case  GameStatus::level_2:
				// new pointer to Level 2 
				curLvl = std::make_unique<level2>();
				break;
			case  GameStatus::level_3:
				// new pointer to level 3
				curLvl = std::make_unique<level3>();
				break;
			case  GameStatus::level_4:
				// new pointer to level 4
				curLvl = std::make_unique<level4>();
				break;
			default:
				// end credits
				curLvl = std::make_unique<endScreen>();
				break;
		}
	}

	//=================================================================================================================
	public :

	// so initally point to mainMenu
	GameManager() {
		gameStatus = GameStatus::main_Menu;
		curLvl = std::make_unique<mainMenu>();
	}

	// so this function does all the updation of variable and stuff
	void manageUpdate() {

		// update the current lvl
		curLvl->update();

		// now if the current level is complete move to the next level
		if (curLvl->getStatus()) {
			LoadNextLevel();
		}
		// also if the player dies or level fails we want to restart it
		if (curLvl->getRestartStatus()) {
			switch (gameStatus) {
			case GameStatus::level_1:
				// create a pointer to Level 1
				// this thing basically destroies the current level points to mainMenu ; then imm points to fresh level
				curLvl = std::make_unique<mainMenu>();
				curLvl = std::make_unique<level1>();
				break;
			case  GameStatus::level_2:
				// new pointer to Level 2 
				curLvl = std::make_unique<mainMenu>();
				curLvl = std::make_unique<level2>();
				break;
			case  GameStatus::level_3:
				// new pointer to level 3
				curLvl = std::make_unique<mainMenu>();
				curLvl = std::make_unique<level3>();
				break;
			case  GameStatus::level_4:
				// new pointer to level 4
				curLvl = std::make_unique<mainMenu>();
				curLvl = std::make_unique<level4>();
				break;
			default:
				// end credits
				break;
			}
		}
	}

	void manageDraw() {
		// draw the current lvl
		curLvl->draw();
	}

	~GameManager() {

	/*	delete curLvl;*/ // so apparently the curlvl dont need delete

	}
	//=================================================================================================================
};