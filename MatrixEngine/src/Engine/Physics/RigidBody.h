#ifndef PHYSICS_OBJ_H_
#define PHYSICS_OBJ_H_

#include "Engine/GameObject/Component.h"
#include "Engine/Core/DataTypes.h"
#include "../GLM/glm.hpp"
#include "Engine\Platform\EventSystem\event.h"
#include "Engine\Physics\CollisionEvents\OnCollisionEvent.h"
#include "Physics\CollisionTable.h"



enum EIntegrator
{
	INTEGRATOR_EULER,
	INTEGRATOR_NEWTON,
	INTEGRATOR_VERLET
};


struct RigidBody : public Component
{
	//SERIALIZABLE;
	
	SERIALIZABLE;
	// constructor
	RigidBody(bool isObjectStatic);
	RigidBody();


	// physics data
	glm::vec2	mPrevPosition;
	glm::vec2	mVelocity;
	glm::vec2	mAcceleration;
	glm::vec2	mGravity;

	bool isBouncy;
	bool mAffectedByGravity;
	f32  mInvMass;
	f32  mDrag;
	bool isStatic;
    float mRestitution;

	EIntegrator mIntegrator = INTEGRATOR_EULER;

	void setAffectedByGravity(bool effect);

	// integrate
	void Integrate(float timeStep);
	void IntegrateEuler(float timeStep);
	void IntegrateNewton(float timeStep);
	void IntegrateVerlet(float timeStep);
    void Initialize();
	void LateUpdate();
	// adding force
	void AddForce(glm::vec2 force);

	// update
	virtual void Update();
};


#endif