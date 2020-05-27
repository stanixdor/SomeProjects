#pragma once

#include "GameObject\Component.h"
#include "Transform\Transform2D.h"
#include "Physics\CollisionTable.h"

enum ECollisionShape
{
	CSHAPE_AABB = 1,	// 001
	CSHAPE_CIRCLE = 2,	// 010
	CSHAPE_OBB = 4,	// 100
	CSHAPE_INDEX_MAX = (CSHAPE_OBB | CSHAPE_CIRCLE) + 1
};

class Collider: public Component
{
public:
	SERIALIZABLE;
	Collider();
	~Collider();
	ECollisionShape mCollisionShape = CSHAPE_AABB;
	
	void Initialize();
	void Update();
	void LateUpdate();

	void ComputeAABB(glm::vec2 & outPos, glm::vec2& outSize);
	Transform2D getTransform() const;

	void drawCollider();
	void addCollisionGroup(CollisionGroup group);
	CollisionGroup& getCollisionGroup();

	std::string collisionGroupName;
	CollisionGroup mCollisionGroup;

	bool isGhost;
	bool* isOwnerStatic;
	Transform2D colliderTransform;


};