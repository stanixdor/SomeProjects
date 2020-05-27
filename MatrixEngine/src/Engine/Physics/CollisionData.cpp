#include "Engine\Physics\CollisionData.h"
#include "Engine\Physics\CollisionSystem.h"
#include "Engine\Physics\Collider.h"
#include "Engine\Physics\CollisionSystem.h"

CollisionData::CollisionData(Collider& object1, Collider& object2, Contact* contactData) : obj1(&object1), obj2(&object2)
{
	if (contactData != nullptr)
		mContactData = *contactData;
}

bool CollisionData::eitherObjectHasACollisionGroupOf(CollisionGroup & collisionGroup) const
{
	return (obj1->getCollisionGroup().mName == collisionGroup.mName || obj2->getCollisionGroup().mName == collisionGroup.mName);
}

bool CollisionData::operator==(const CollisionData& other)
{
	if (obj1 == nullptr || obj2 == nullptr)
		return false;

	return ((obj1 == other.obj1 && obj2 == other.obj2) || (obj2 == other.obj1 && obj1 == other.obj2));
}


