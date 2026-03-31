#include "mazeMaker.hpp"

// basically a function that tells us about the collision
// give it the position and hit radius and it tells if the thing is colliding with the wall
bool MazeMaker::isColliding(Vector2 position,float hitRadius) {
   
    float nX = floorf(position.x / cellSize);
    float nY = floorf(position.y / cellSize);

    Rectangle r1 = {
      0.0f - cellSize / 2.0f + nX * cellSize,
      0.0f - cellSize / 2.0f + nY * cellSize,
      (float)cellSize,
      (float)cellSize
    };


    Rectangle r2 = {
      0.0f - cellSize / 2.0f +( nX + 1) * cellSize,
      0.0f - cellSize / 2.0f + nY * cellSize,
      (float)cellSize,
      (float)cellSize
    };


    Rectangle r3 = {
      0.0f - cellSize / 2.0f + nX * cellSize,
      0.0f - cellSize / 2.0f + (nY+1) * cellSize,
      (float)cellSize,
      (float)cellSize
    };


    Rectangle r4 = {
      0.0f - cellSize / 2.0f + (nX+1) * cellSize,
      0.0f - cellSize / 2.0f + (nY+1) * cellSize,
      (float)cellSize,
      (float)cellSize
    };

    // bounds-checked wall lookup
    int ix = (int)nX;
    int iy = (int)nY;
    auto isWall = [&](int x, int y) -> bool {
        return (x >= 0 && x < mazeSize && y >= 0 && y < mazeSize && maze[x][y] == 1);
    };

    //if it is colliding with any of them , then we return true
    if ((CheckCollisionCircleRec(position, hitRadius, r1) && isWall(ix, iy)) ||
        (CheckCollisionCircleRec(position, hitRadius, r2) && isWall(ix + 1, iy)) ||
        (CheckCollisionCircleRec(position, hitRadius, r3) && isWall(ix, iy + 1)) ||
        (CheckCollisionCircleRec(position, hitRadius, r4) && isWall(ix + 1, iy + 1))
        ) return true;

    else return false;
}

// checks if the line of sight is obstructed or not

bool MazeMaker::losCheckObs(Vector2 pos, Vector2 los)
{


    // not gonnna lie just copying  the line algo
    // calculate dx & dy
    int dx = (pos.x + los.x) - pos.x;
    int dy = (pos.y + los.y) - pos.y;

    // calculate steps required for generating pixels
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    // calculate increment in x & y for each steps
    float Xinc = dx / (float)steps;
    float Yinc = dy / (float)steps;

    // Put pixel for each step
    float X = pos.x;
    float Y = pos.y;

    for (int i = 0; i <= steps; i++) {
       
        // check if there is any collsion we do this by checking fdr collsion with a point
        if (isColliding({ X,Y }, 0.0f)) return true;

        X += Xinc; // increment in x at each step
        Y += Yinc; // increment in y at each step
       
    }

    return false;
}

// normal just drawing the maze 
void MazeMaker::draw() {

	for (int i = 0; i < mazeSize; i++) {
		for (int j = 0; j < mazeSize; j++) {
			if (maze.at(i).at(j) == 1) {

                // pulsing walls
                DrawRectangleV({ static_cast<float>(0 - cellSize / 2 + i * cellSize),static_cast<float>(0.0f - cellSize / 2 + j * cellSize) }, { static_cast<float>(cellSize),static_cast<float>(cellSize) }, {255,0,0,255});
                DrawRectangleV({ static_cast<float>(0 - cellSize / 2 + i * cellSize - waveTime * sin(GetTime()) / 2),static_cast<float>(0.0f - cellSize / 2 + j * cellSize - waveTime * sin(GetTime()) / 2) }, { static_cast<float>(cellSize + waveTime * sin(GetTime())),static_cast<float>(cellSize + waveTime * sin(GetTime())) }, { 200,0,0,255 });/*
                
                DrawRectangleV({ static_cast<float>(0 - cellSize / 6 + i * cellSize - wave * cos(GetTime()) / 2),static_cast<float>(0.0f - cellSize / 6 + j * cellSize - wave * cos(GetTime()) / 2) }, { static_cast<float>(cellSize/1.5f + wave * cos(GetTime())),static_cast<float>(cellSize/1.5f + wave * cos(GetTime())) }, {200,0,0,255});
                DrawRectangleV({ static_cast<float>(0 - cellSize / 8 + i * cellSize - wave * sin(GetTime()) / 2),static_cast<float>(0.0f - cellSize / 8 + j * cellSize - wave * sin(GetTime()) / 2) }, { static_cast<float>(cellSize/2 + wave * sin(GetTime())),static_cast<float>(cellSize/2 + wave * sin(GetTime())) }, { 180,0,0,255 });
                DrawRectangleV({ static_cast<float>(0 - cellSize / 10 + i * cellSize - wave * cos(GetTime()) / 2),static_cast<float>(0.0f - cellSize / 10 + j * cellSize - wave * cos(GetTime()) / 2) }, { static_cast<float>(cellSize/2.5f + wave * cos(GetTime())),static_cast<float>(cellSize/2.5f + wave * cos(GetTime())) }, { 160,0,0,255 });*/
			}
		}
	}
}

// checking if the given cell cords is colliding with the current maze

void MazeMaker::wallCollisionX(entity& entity, float nX, float nY,float elasticity)
{
    // 1. Define the wall rectangle
    Rectangle r1 = {
        0.0f - cellSize / 2.0f + nX * cellSize,
        0.0f - cellSize / 2.0f + nY * cellSize,
        (float)cellSize,
        (float)cellSize
    };

    // 2. Bounds and collision check
    if ((nX >= 0) && (nX < mazeSize) && (nY >= 0) && (nY < mazeSize) &&
        (maze.at(nX).at(nY)==1) &&
        CheckCollisionCircleRec(entity.collisionData().first, entity.collisionData().second, r1))
    {
        Vector2 pPos = entity.collisionData().first;
        float radius = entity.collisionData().second;

        // 3. Find the closest point on this specific wall to the player center
        float closestX = std::max(r1.x, std::min(pPos.x, r1.x + r1.width));
        float closestY = std::max(r1.y, std::min(pPos.y, r1.y + r1.height));
        Vector2 closestPoint = { closestX, closestY };

        // 4. Calculate the vector from the wall to the player
        Vector2 collisionVec = Vector2Subtract(pPos, closestPoint);
        float distance = Vector2Length(collisionVec);

        if (distance == 0.0f) {
            // Player center is inside the wall — find nearest X edge and push out
            float distToLeft = pPos.x - r1.x;
            float distToRight = (r1.x + r1.width) - pPos.x;

            if (distToLeft <= distToRight) {
                entity.collisionHandler({ -1.0f, 0.0f }, elasticity, distToLeft + radius);
            } else {
                entity.collisionHandler({ 1.0f, 0.0f }, elasticity, distToRight + radius);
            }
        } else {
            // 5. Calculate real penetration depth
            float penetrationDepth = radius - distance;

            // 6. Send the real data to the handler
            Vector2 collisionNormal = Vector2Scale(collisionVec, 1.0f / distance);
            entity.collisionHandler({ collisionNormal.x, 0.0f }, elasticity, penetrationDepth);
        }
    }
}


void MazeMaker::wallCollisionY(entity& entity, float nX, float nY, float elasticity)
{
    // 1. Define the wall rectangle
    Rectangle r1 = {
        0.0f - cellSize / 2.0f + nX * cellSize,
        0.0f - cellSize / 2.0f + nY * cellSize,
        (float)cellSize,
        (float)cellSize
    };

    // 2. Bounds and collision check
    if ((nX >= 0) && (nX < mazeSize) && (nY >= 0) && (nY < mazeSize) &&
        (maze.at(nX).at(nY) == 1) &&
        CheckCollisionCircleRec(entity.collisionData().first, entity.collisionData().second, r1))
    {
        Vector2 pPos = entity.collisionData().first;
        float radius = entity.collisionData().second;

        // 3. Find the closest point on this specific wall to the player center
        float closestX = std::max(r1.x, std::min(pPos.x, r1.x + r1.width));
        float closestY = std::max(r1.y, std::min(pPos.y, r1.y + r1.height));
        Vector2 closestPoint = { closestX, closestY };

        // 4. Calculate the vector from the wall to the player
        Vector2 collisionVec = Vector2Subtract(pPos, closestPoint);
        float distance = Vector2Length(collisionVec);

        if (distance == 0.0f) {
            // Player center is inside the wall — find nearest Y edge and push out
            float distToTop = pPos.y - r1.y;
            float distToBottom = (r1.y + r1.height) - pPos.y;

            if (distToTop <= distToBottom) {
                entity.collisionHandler({ 0.0f, -1.0f }, elasticity, distToTop + radius);
            } else {
                entity.collisionHandler({ 0.0f, 1.0f }, elasticity, distToBottom + radius);
            }
        } else {
            // 5. Calculate real penetration depth
            float penetrationDepth = radius - distance;

            // 6. Send the real data to the handler
            Vector2 collisionNormal = Vector2Scale(collisionVec, 1.0f / distance);
            entity.collisionHandler({ 0.0f, collisionNormal.y }, elasticity, penetrationDepth);
        }
    }
}


void MazeMaker::update(entity& interactingEntity, float elasticity)
{
	std::pair<Vector2, float> colDataEntity = interactingEntity.collisionData();
	float nX = floorf(colDataEntity.first.x / cellSize);
	float nY = floorf(colDataEntity.first.y / cellSize);

	
	// (nX,nY)==========(nX+1,nY)
	//	 ||                 ||
	//	 ||                 ||
	//	 ||                 ||
	//(nX,nY+1)==========(nX+1,nY+1)

    // so there is a bug if it gets exactly b.w two walls then thier effect kind of cancels out some how and so it kind
    // of just phases through

    wallCollisionY(interactingEntity, nX, nY, elasticity);
    wallCollisionY(interactingEntity, nX + 1, nY, elasticity);
    wallCollisionY(interactingEntity, nX, nY + 1, elasticity);
    wallCollisionY(interactingEntity, nX + 1, nY + 1, elasticity);

    // Recalculate grid position after Y resolution using CURRENT position
    colDataEntity = interactingEntity.collisionData();
    nX = floorf(colDataEntity.first.x / cellSize);
    nY = floorf(colDataEntity.first.y / cellSize);

    // X pass second
	wallCollisionX(interactingEntity, nX, nY, elasticity);
	wallCollisionX(interactingEntity, nX + 1, nY, elasticity);
	wallCollisionX(interactingEntity, nX, nY+ 1, elasticity);
	wallCollisionX(interactingEntity, nX+1, nY+1, elasticity);
}
