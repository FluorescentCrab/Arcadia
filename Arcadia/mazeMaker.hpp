#pragma once 

#include "raylib.h"
#include <vector>
#include "raymath.h"
#include "entity.hpp"
#include <iostream>
 
// each block is of size 100
class MazeMaker {
	// now this will take a 2d matrix and make a maze according to it
	private :
		// size of the cell in the maze
		int cellSize;
		// number of cells in the maze
		int mazeSize;
		// maze layout
		std::vector<std::vector<int>> maze;
		// helps in the pulsing effect
		float waveTime;

		void wallCollisionX(entity& entity, float nX , float nY, float elasticity);
		void wallCollisionY(entity& entity, float nX, float nY, float elasticity);

	public:
		MazeMaker(int cellSize, int mazeSize,float waveTime, std::vector<std::vector<int>> maze) : cellSize(cellSize), mazeSize(mazeSize), maze(maze),waveTime(waveTime) {}
		bool isColliding(Vector2 position,float hitRadius);

		void draw();

		// checks for any obstruction
		bool losCheckObs(Vector2 pos, Vector2 los);

		void update(entity& interactingEntity,float elasticity);
};