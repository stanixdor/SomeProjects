#pragma once
#include "Physics\CollisionResolution.h"
#include "Physics\CollisionTable.h"

class Collider;

struct CollisionData
{
    CollisionData(Collider& object1, Collider& object2, Contact* mContactData);
	Collider* obj1;
	Collider* obj2;

    Contact mContactData;

	bool eitherObjectHasACollisionGroupOf(CollisionGroup& collisionGroup) const;
    bool operator==(const CollisionData& other);
};
