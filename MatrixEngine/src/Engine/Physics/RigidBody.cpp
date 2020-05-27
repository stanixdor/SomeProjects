// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior 
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:		RigidBody.cpp
//	Purpose:		This file contains the functions used to compute acceleration and position of
//					an object after a certain amount of time
//	Project:		CS230_joaquin.Bilbao_3
//	Author:			Joaquin Bilbao, Joaquin.Bilbao, 540002415
// ----------------------------------------------------------------------------
#include "Engine/Physics/RigidBody.h"
#include "Engine/Platform/Time/FrameRateController.h"
#include "Engine/GameObject/GameObject.h"
#include "Engine\Platform\EventSystem\event_dispatcher.h"
#include "Engine/Physics/CollisionSystem.h"
#include "Engine\Physics\CollisionEvents\OnCollisionEvent.h"
#include "Physics\CollisionTable.h"
#include "Graphics\GraphicsSystem.h"

FRC::FrameRateController* frameRateController = FRC::FrameRateController::Instance();
// ----------------------------------------------------------------------------
#pragma region // @PROVIDED

REFLECTION(RigidBody, mPrevPosition, mVelocity, mAcceleration, mGravity, isBouncy, mAffectedByGravity, mInvMass, mDrag, isStatic, mRestitution);

// @PROVIDED
//!----------------------------------------------------------------------------
// \fn		Constructor
// \brief	Sets default values
// ----------------------------------------------------------------------------
RigidBody::RigidBody(bool isObjectStatic)
	: Component("RigidBody")
	, mInvMass(0.1f)
	, mDrag(0.990f)
	, mGravity(glm::vec2(0, -500.f))
	, isBouncy(false)
	, mAffectedByGravity(true)
	, mRestitution(0.f)
{
	isStatic = isObjectStatic;
}

RigidBody::RigidBody()
	: Component("RigidBody")
	, mInvMass(0.1f)
	, mDrag(0.990f)
	, mGravity(glm::vec2(0, -500.f))
	, isBouncy(false)
	, mAffectedByGravity(true)
	, mRestitution(0.f)
{

	isStatic = true;
}

void RigidBody::Initialize()
{
	updateInEditMode = true;
}

void RigidBody::LateUpdate()
{
	if (m_alive == false)
	{
		Collider* objectsCollider = m_GameObject->FindComponentByType<Collider>();
		//since collider depends on RigidBody, removing the rigidbody of an object also removes its collider
		if (objectsCollider != nullptr)
		{
			objectsCollider->Destroy();
		}
	}
}

void RigidBody::setAffectedByGravity(bool effect)
{
	mAffectedByGravity = effect;
}

// @PROVIDED
//!----------------------------------------------------------------------------
// \fn		Integrate
// \brief	Calls the correct integration method and clears the forces. 
// ----------------------------------------------------------------------------
void RigidBody::Integrate(float timeStep)
{
	if (timeStep != 0.0f && mInvMass != 0.0f) {

		// Integrate based on the integrator
		switch (mIntegrator) {
		case INTEGRATOR_EULER: IntegrateEuler(timeStep);
			break;
		case INTEGRATOR_NEWTON: IntegrateNewton(timeStep);
			break;
		case INTEGRATOR_VERLET: IntegrateVerlet(timeStep);
			break;
		}

		// clear the forces applied on the object
		mAcceleration = glm::vec2(0, 0);
	}
}

glm::vec2 pastVel;
// @PROVIDED
//!----------------------------------------------------------------------------
// \fn		Update
// \brief	Updates the game object and its physics
// ----------------------------------------------------------------------------
void RigidBody::Update()
{
	if (isStatic)
	{
		mInvMass = 0;
	}
	/*If its not static and the inverse mass is 0 (meaning its actually static)*/
	else if (mInvMass == 0)
	{
		mInvMass = 1;
	}

	if (!isBouncy)
		mRestitution = 0.f;

	if (isBouncy && mRestitution == 0.f)
		mRestitution = 1.0f;


	if (playMode && !PAUSE)
	{
		// add gravity
		if (mInvMass && mAffectedByGravity)
			AddForce(mGravity / mInvMass);
	}

	// Integrate physics
	Integrate(static_cast<f32>(frameRateController->GetFrameTime()));

	//pastVel = mVelocity;

}

/*
// @PROVIDED
//!----------------------------------------------------------------------------
// \fn		Render
// \brief	Renders using lines only.
// ----------------------------------------------------------------------------
void RigidBody::Render()
{
// render just the aabb
AEGfxReset();
switch (mCollisionShape)
{
case CSHAPE_OBB:
case CSHAPE_AABB:{
Transform2D obb(mPosition, mScale, mRotation);
AEGfxRectRot(mPosition.x, mPosition.y, 0, mScale.x, mScale.y, mRotation, mColor);
break;
}
case CSHAPE_CIRCLE:{
AEGfxCircle(mPosition.x, mPosition.y, mScale.x, mColor);
break;
}
}
AEGfxFlush();
}

// @PROVIDED
//!----------------------------------------------------------------------------
// \fn		Render(Camera)
// \brief	Renders using a camera matrix
// ----------------------------------------------------------------------------
void RigidBody::Render(AEMtx33 * pCamMtx)
{
// render just the aabb
AEGfxReset();
if (pCamMtx)
AEGfxSetTransform(pCamMtx);
switch (mCollisionShape)
{
case CSHAPE_OBB:
case CSHAPE_AABB:{
AEGfxRectRot(mPosition.x, mPosition.y, 0, mScale.x, mScale.y, mRotation, mColor);
break;
}
case CSHAPE_CIRCLE:{
AEGfxCircle(mPosition.x, mPosition.y, mScale.x, mColor);
break;
}
}
AEGfxFlush();
}
*/

// @PROVIDED
//!----------------------------------------------------------------------------
// \fn		AddForce
// ----------------------------------------------------------------------------
void RigidBody::AddForce(glm::vec2 force)
{
	mAcceleration.x += force.x;
	mAcceleration.y += force.y;
}
#pragma endregion
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
#pragma region // @TODO


//!----------------------------------------------------------------------------
// \fn		IntegrateEuler
// \brief	Integrates using symplectic Euler
// ----------------------------------------------------------------------------
void RigidBody::IntegrateEuler(float /*timeStep*/)
{
	//acceleration
	glm::vec2 acceleration = mAcceleration * mInvMass;

	//apply Euler's equation
	mVelocity += acceleration * 0.016f;//timeStep;

	GameObject* owner = GetOwner();

	mPrevPosition = owner->m_transform->GetPos();
	//new pos is the old pos + the movement
	owner->m_transform->SetPos(owner->m_transform->GetPos() + (mVelocity * 0.016f));// timeStep));

}


//!----------------------------------------------------------------------------
// \fn		IntegrateNewton
// \brief	Integrates using newton integration (gravity only)
// ----------------------------------------------------------------------------
void RigidBody::IntegrateNewton(float timeStep)
{
	//acceleration
	glm::vec2 acceleration = mAcceleration * mInvMass;


	GameObject* owner = GetOwner();
	mPrevPosition = owner->m_transform->GetPos();

	//apply Newton's equation
	owner->m_transform->SetPos(owner->m_transform->GetPos() + mVelocity * timeStep + acceleration * 0.5f * timeStep * timeStep);
	mAcceleration = mGravity * mInvMass;
}


//!----------------------------------------------------------------------------
// \fn		IntegrateNewton
// \brief	Integrates using Verlet integration described in class. 
// ----------------------------------------------------------------------------
void RigidBody::IntegrateVerlet(float timeStep)
{
	//acceleration
	glm::vec2 accleration = mAcceleration * mInvMass;

	GameObject* owner = GetOwner();

	mPrevPosition = owner->m_transform->GetPos();
	//apply verlet's equation
	owner->m_transform->SetPos((owner->m_transform->GetPos() * 2.0f) - mPrevPosition + accleration * timeStep * timeStep);

	//this originally was here
	//mPrevPosition = owner->m_transform->GetPos();
}

#pragma endregion
// ----------------------------------------------------------------------------
