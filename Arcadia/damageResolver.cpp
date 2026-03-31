#include "damageResolver.hpp"

void damageResolver::damageResolve(entity& entity1, entity& entity2,float elasticty1 , float elasticity2)
{
	std::pair<Vector2, float> colDataEntity1 = entity1.collisionData();
	std::pair<Vector2, float> colDataEntity2 = entity2.collisionData();
	

	//std::cout << colDataEntity1.first.x << " " << colDataEntity1.first.y << " " << colDataEntity2.first.x << " " << colDataEntity2.first.y << std::endl;

	if (CheckCollisionCircles(colDataEntity1.first,colDataEntity1.second,colDataEntity2.first,colDataEntity2.second)) {
		entity1.takeDmg(entity2.giveDmg());
		entity2.takeDmg(entity1.giveDmg());
		float distance = Vector2Distance(colDataEntity1.first, colDataEntity2.first);
		float penetration = colDataEntity1.second + colDataEntity2.second - distance;
		entity1.collisionHandler(Vector2Subtract(colDataEntity1.first, colDataEntity2.first), elasticty1,penetration);
		entity2.collisionHandler(Vector2Subtract(colDataEntity2.first, colDataEntity1.first), elasticity2,penetration);
	}
}
