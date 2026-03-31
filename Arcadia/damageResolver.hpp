#pragma once 

#include "raylib.h"
#include "raymath.h"
#include <utility>
#include <iostream>
#include "entity.hpp"

class damageResolver {

public:
	void damageResolve(entity& entity1, entity& entity2, float elasticty1, float elasticity2);
};

// for now this handles entity-entity collsion

