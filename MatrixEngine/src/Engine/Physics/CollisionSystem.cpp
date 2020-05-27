// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior 
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:		CollisionSystem.cpp
//	Purpose:		This file contains the functions used to resolve collisions and
//					force collisions between objects
//	Project:		CS230_joaquin.Bilbao_3
//	Author:			Joaquin Bilbao, Joaquin.Bilbao, 540002415
// ----------------------------------------------------------------------------
#include "Engine/Physics/CollisionSystem.h"
#include "Engine/GameObject/GameObject.h"
#include "Engine\Platform\EventSystem\event_dispatcher.h"
#include "Engine/Physics/CollisionEvents/OnCollisionEvent.h"
#include "Engine/Physics/RigidBody.h"
#include "Engine\Physics\CollisionTable.h"
#include <algorithm>
#include <iostream>
#include <assert.h>
// ----------------------------------------------------------------------------


// @PROVIDED
//!----------------------------------------------------------------------------
// \fn		Init
// \brief	Called at beginning of game. Sets default and adds the collision 
//			test to the function pointer map. 
// ----------------------------------------------------------------------------
void CollisionSystem::Init()
{
	// default
	mCollisionIterations = 1;

	for (u32 i = 0; i < CSHAPE_INDEX_MAX; ++i)
		mCollisionTests[i] = NULL;

	mCollisionTests[CSHAPE_CIRCLE | CSHAPE_CIRCLE] = CollideCircles;
	mCollisionTests[CSHAPE_AABB | CSHAPE_AABB] = CollideAABBs;
	mCollisionTests[CSHAPE_OBB | CSHAPE_OBB] = CollideOBBs;
	mCollisionTests[CSHAPE_CIRCLE | CSHAPE_AABB] = CollideAABBToCircle;
	mCollisionTests[CSHAPE_OBB | CSHAPE_AABB] = CollideOBBs;
	mCollisionTests[CSHAPE_CIRCLE | CSHAPE_OBB] = CollideOBBToCircle;
}
// @PROVIDED
//!----------------------------------------------------------------------------
// \fn		Update
// \brief	Frame update. For now, only call ClearBodies.
// ----------------------------------------------------------------------------
void CollisionSystem::Update()
{

	CollideAllBodies();

	//we update LastFrameCollisions and prepare the vector for the next update
	LastFrameCollisions.clear();
	LastFrameCollisions = thisFrameCollisions;
	thisFrameCollisions.clear();
}
void CollisionSystem::changeColliderFromVector(const Collider &)
{
}
// @PROVIDED
//!----------------------------------------------------------------------------
// \fn		Shutdown
// \brief	Shutdown operation at the end of a game. 
//			For now, only call ClearBodies.
// ----------------------------------------------------------------------------
void CollisionSystem::Shutdown()
{
	ClearBodies();
}

void CollisionSystem::endOfFrameUpdate()
{
	auto dynamicBodiesIt = mDynamicBodies.begin();

	Collider* collider;
	while (dynamicBodiesIt != mDynamicBodies.end())
	{
		collider = *dynamicBodiesIt;
		if (collider->m_alive == false)
		{
			dynamicBodiesIt = mDynamicBodies.erase(dynamicBodiesIt);
			continue;
		}
		else if (collider->isOwnerStatic != nullptr && *collider->isOwnerStatic == true)
		{
			mStaticBodies.push_back(collider);
			dynamicBodiesIt = mDynamicBodies.erase(dynamicBodiesIt);
			continue;
		}
		else
			dynamicBodiesIt++;
	}
	

	//dynamicBodiesIt = mDynamicBodies.begin();
	//
	//while (dynamicBodiesIt != mDynamicBodies.end())
	//{
	//	//collider = ;
	//	if ((*dynamicBodiesIt)->isOwnerStatic != nullptr && *(*dynamicBodiesIt)->isOwnerStatic == true)
	//	{
	//		//if(dynamicBodiesIt != mDynamicBodies.end())
	//
	//
	//		RemoveCollider(**dynamicBodiesIt);
	//		mStaticBodies.push_back(collider);
	//		//continue;
	//	}
	//	else
	//		dynamicBodiesIt++;
	//}
	

	auto staticBodiesIt = mStaticBodies.begin();
	while (staticBodiesIt != mStaticBodies.end())
	{
		collider = *staticBodiesIt;

		if (collider->m_alive == false)
		{
			staticBodiesIt = mStaticBodies.erase(staticBodiesIt);
		}
		else if (collider->isOwnerStatic != nullptr && *collider->isOwnerStatic == false)
		{
			mDynamicBodies.push_back(collider);
			staticBodiesIt = mStaticBodies.erase(staticBodiesIt);
		}
		else
			staticBodiesIt++;
	
	}

	//auto bodiesIterator = mBodies.begin();
	//while (bodiesIterator != mBodies.end())
	//{
	//	if ((*bodiesIterator)->m_alive == false)
	//	{
	//		bodiesIterator = mBodies.erase(bodiesIterator);
	//	}
	//	else
	//		bodiesIterator++;
	//
	//}
}

// @PROVIDED
//!----------------------------------------------------------------------------
// \fn		AddCollider
// \brief	Adds the Collider to the appropriate container (based on is_dynamic). 
// ----------------------------------------------------------------------------
void CollisionSystem::AddCollider(Collider& obj)
{
	if (obj.isOwnerStatic != nullptr && *obj.isOwnerStatic)
	    mStaticBodies.push_back(&obj);
	else
	    mDynamicBodies.push_back(&obj);

	//mBodies.push_back(&obj);
}
// @PROVIDED
//!----------------------------------------------------------------------------
// \fn		RemoveCollider
// \brief	Removes the Collider from the containers (brute force). 
// ----------------------------------------------------------------------------
void CollisionSystem::RemoveCollider(Collider &obj)
{
	auto it = std::find(mDynamicBodies.begin(), mDynamicBodies.end(), &obj);
	
	if (it != mDynamicBodies.end())
	{
		//remove collider from the vector of colliders to compute
		mDynamicBodies.erase(std::remove(mDynamicBodies.begin(), mDynamicBodies.end(), &obj), mDynamicBodies.end());
	}
	else
	{
		it = std::find(mStaticBodies.begin(), mStaticBodies.end(), &obj);
		if (it != mStaticBodies.end())
		{
			//remove collider from the vector of colliders to compute
			mStaticBodies.erase(std::remove(mStaticBodies.begin(), mStaticBodies.end(), &obj), mStaticBodies.end());
		}
		//otherwise the collider is not in the vector, wtf bruh.
		else
			return;
	}

	auto iteratorForThisFrameCollisions = thisFrameCollisions.begin();

	//look for any saved CollisionData that contains said collider and remove it
	iteratorForThisFrameCollisions = std::remove_if(iteratorForThisFrameCollisions, thisFrameCollisions.end(), [&](CollisionData& data)
	{
		return (&obj == data.obj1 || &obj == data.obj2);
	}

	);

	thisFrameCollisions.erase(iteratorForThisFrameCollisions, thisFrameCollisions.end());

	iteratorForThisFrameCollisions = LastFrameCollisions.begin();

	//look for any saved CollisionData that contains said collider and remove it
	iteratorForThisFrameCollisions = std::remove_if(iteratorForThisFrameCollisions, LastFrameCollisions.end(), [&](CollisionData& data)
	{
		return (&obj == data.obj1 || &obj == data.obj2);
	}

	);

	LastFrameCollisions.erase(iteratorForThisFrameCollisions, LastFrameCollisions.end());
}

// @PROVIDED
//!----------------------------------------------------------------------------
// \fn		ClearBodies
// \brief	Clears the static and dynamic body containers.
// ----------------------------------------------------------------------------
void CollisionSystem::ClearBodies()
{
	mDynamicBodies.clear();
	mStaticBodies.clear();
	//mBodies.clear();
}

// @PROVIDED
//!----------------------------------------------------------------------------
// \fn		GetCollisionFn
// \brief	returns the appropriate collision function based on the shape of the 
//			passed rigid bodies, b1 and b2.
// ----------------------------------------------------------------------------
CollisionFn CollisionSystem::GetCollisionFn(Collider & b1, Collider & b2)
{
	u32 collision_index = b1.mCollisionShape | b2.mCollisionShape;
	return mCollisionTests[collision_index];
}

bool CollideCircles(Collider& body1, Collider& body2, Contact * c)
{
	Transform2D* body1Transform = body1.GetOwner()->m_transform;
	Transform2D* body2Transform = body2.GetOwner()->m_transform;

	glm::vec2 pos1 = body1Transform->GetPos();
	glm::vec2 pos2 = body2Transform->GetPos();

	return StaticCircleToStaticCircleEx(body1.getTransform().GetPos(), abs(body1.getTransform().GetScale().x), body2.getTransform().GetPos(), abs(body1.getTransform().GetScale().x), c);
}

bool CollideAABBs(Collider& body1, Collider& body2, Contact * c)
{
	//Transform2D* body1Transform = body1.GetOwner()->m_transform;
	//Transform2D* body2Transform = body2.GetOwner()->m_transform;

	//Transform2D body1Transform = body1.getTransform();
	//Transform2D body2Transform = body2.getTransform();

	Transform2D temp = body1.getTransform();

	glm::vec2 p1 = body1.getTransform().GetPos();
	glm::vec2 s1 = { glm::abs(body1.getTransform().GetScale()) };
	glm::vec2 p2 = body2.getTransform().GetPos();
	glm::vec2 s2 = { glm::abs(body2.getTransform().GetScale())};

	return StaticRectToStaticRectEx(p1, s1, p2, s2, c);
}

bool CollideOBBs(Collider& body1, Collider& body2, Contact * c)
{
	Transform2D body1Transform = body1.getTransform();
	Transform2D body2Transform = body2.getTransform();
	//

	//fakebody is a copy of the transform of the object, but with the new offset
	//Transform2D fakeBody1 = *body1.GetOwner()->m_transform;
	//Transform2D fakeBody2 = *body2.GetOwner()->m_transform;
	//
	//fakeBody1.SetPos(body1.getTransform());
	//fakeBody2.SetPos(body2.getTransform());



	//Transform2D obb1(body1Transform->GetPos(), body1Transform->GetScale() , body1Transform->GetRotation());
	//Transform2D obb2(body2Transform->GetPos() , body2Transform->GetScale() , body1Transform->GetRotation());

	return OrientedRectToOrientedRectEx(body1Transform, body2Transform, c);
}

bool CollideAABBToCircle(Collider& body1, Collider& body2, Contact * c)
{
	Transform2D* body1Transform = body1.GetOwner()->m_transform;
	Transform2D* body2Transform = body2.GetOwner()->m_transform;

	Transform2D* rect = body1.mCollisionShape == CSHAPE_AABB ? body1Transform : body2Transform;
	Transform2D* circle = body1.mCollisionShape == CSHAPE_CIRCLE ? body1Transform : body2Transform;

	if (StaticRectToStaticCircleEx(glm::vec2(rect->GetPos()), abs(rect->GetScale().x), abs(rect->GetScale().y), glm::vec2(circle->GetPos()), abs(circle->GetScale().x), c))
	{
		if (circle == body1Transform) // flip normal to match our convention
			c->mNormal = -c->mNormal;
		return true;
	}
	return false;
}

bool CollideOBBToCircle(Collider& body1, Collider& body2, Contact * c)
{
	Transform2D* body1Transform = body1.GetOwner()->m_transform;
	Transform2D* body2Transform = body2.GetOwner()->m_transform;

	// which is which
	Transform2D obb = body1.mCollisionShape == CSHAPE_OBB
		? Transform2D(body1Transform->GetPos(), body1Transform->GetScale(), body1Transform->GetRotation())
		: Transform2D(body2Transform->GetPos(), body2Transform->GetScale(), body2Transform->GetRotation());

	Transform2D * circle = body1.mCollisionShape == CSHAPE_CIRCLE ? body1Transform : body2Transform;
	if (StaticOBBToStaticCircleEx(obb, glm::vec2(circle->GetPos()), circle->GetScale().x, c))
	{
		if (circle == body1Transform) // flip normal to match our convention
			c->mNormal = -c->mNormal;
		return true;
	}
	return false;
}

// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------

// @TODO
//!----------------------------------------------------------------------------
// \fn		ResolveContactPenetration
// \brief	Given the contact data, resolves the penetration of the two given bodies.
//	\details
//			- Contact viewer is always body1. 
//			- After this function returns, the bodies position should be modified, such
//			  that they are at the minimum distance where they are no longer overlapping.
//			- The amount of penetration corrected for each body must depend on 
//			  their relative masses. 
//			- The bodies' positions are corrected along the contact normal.
// ----------------------------------------------------------------------------
void CollisionSystem::ResolveContactPenetration(Collider & body1, Collider & body2, Contact * contact)
{
	//Security check
	if (contact == nullptr)
		return;


	assert(body1.m_GameObject->FindComponentByType<RigidBody>() != nullptr && "THIS SHOULDN'T HAPPEN"); // break if not true

	float body1InverseMass = body1.m_GameObject->FindComponentByType<RigidBody>()->mInvMass;
	float body2InverseMass = body2.m_GameObject->FindComponentByType<RigidBody>()->mInvMass;
	//compute the total inverse mass
	float TotalInvMass = body1InverseMass + body2InverseMass;

	Transform2D* body1Transform = body1.GetOwner()->m_transform;
	Transform2D* body2Transform = body2.GetOwner()->m_transform;

	glm::vec2 result = body1Transform->GetPos() - contact->mNormal * contact->mPenetration * (body1InverseMass / TotalInvMass);
	//the new position of the body depends on its relative mass
	if (/*body1.isStatic || body2.isStatic*/ TotalInvMass == 0)
	{
		return;
	}

	body1Transform->SetPos(body1Transform->GetPos() - contact->mNormal * contact->mPenetration * (body1InverseMass / TotalInvMass));
	body2Transform->SetPos(body2Transform->GetPos() + contact->mNormal * contact->mPenetration * (body2InverseMass / TotalInvMass));

	//std::cout << "Object 1 change: " << body1Transform->GetPos().x << " " << body1Transform->GetPos().y << std::endl;


	//std::cout << "Object 2 change: " << body2Transform->GetPos().x << " " << body2Transform->GetPos().y << std::endl;

}

// @TODO
//!----------------------------------------------------------------------------
// \fn		ResolveContactVelocity
// \brief	Given the contact data, resolves the velocity of the two given bodies.
//	\details
//			- After this function returns, the bodies velocities should be modified
//			  such that the bodies are separating. YOU MUST IMPLEMENT THE ALGORITHM DESCRIBED IN CLASS. 
//			- the algorithm depends on a user-defined restitution. Here you should use
//			  the maccro DFLT_RESTITUTION (default value is 0.908f).
//			- Contact viewer is always body1. 
//			- The amount of velocity corrected for each body must depend on 
//			  their relative masses. 
//			- The velocities are corrected along the contact normal.
// ----------------------------------------------------------------------------
void CollisionSystem::ResolveContactVelocity(Collider & body1, Collider & body2, Contact * contact)
{
	//Security check
	if (contact == nullptr)
		return;

	RigidBody* body1RigidBody = body1.m_GameObject->FindComponentByType<RigidBody>();
	RigidBody* body2RigidBody = body2.m_GameObject->FindComponentByType<RigidBody>();

	float body1InverseMass = body1RigidBody->mInvMass;
	float body2InverseMass = body2RigidBody->mInvMass;

	//compute the total inverse mass
	float TotalInvMass = body1InverseMass + body2InverseMass;

	glm::vec2 relativeVelocity = body2RigidBody->mVelocity - body1RigidBody->mVelocity;
	float separatingVelocity = glm::dot(relativeVelocity, contact->mNormal);

	//get separating velocity
	if (separatingVelocity < 0)
	{
		//we perform the operations to compute the new separation velocity and its delta
		float newSeparatingVelForObject1 = -separatingVelocity * body1RigidBody->mRestitution;
		float deltaSeparatingVelForObject1 = newSeparatingVelForObject1 - separatingVelocity;
		float impulseForObject1 = deltaSeparatingVelForObject1 / TotalInvMass;

		float newSeparatingVelForObject2 = -separatingVelocity * body2RigidBody->mRestitution;
		float deltaSeparatingVelForObject2 = newSeparatingVelForObject2 - separatingVelocity;
		float impulseForObject2 = deltaSeparatingVelForObject2 / TotalInvMass;
		//compute impulse per inverse mass
		//with this data, now we compute the final velocity of each body

		body1RigidBody->mVelocity -= contact->mNormal * body1InverseMass / TotalInvMass * body1InverseMass * impulseForObject1;
		body2RigidBody->mVelocity += contact->mNormal * body2InverseMass / TotalInvMass * body2InverseMass * impulseForObject2;
	}
	//if (!body1.isBouncy)
	//	body1.mVelocity = { 0, 0 };
	//
	//if (!body2.isBouncy)
	//	body2.mVelocity = { 0, 0};
}

// @TODO
//!----------------------------------------------------------------------------
// \fn		CollideAllBodies
// \brief	Perform brute force collision detection/resolution between dynamic and static
//			bodies. Repeats N times, where N = mCollisionIterations.
//	\details
//			- Must store the number collisions that occured in mCollisionsThisFrame
//			- Must use "GetCollisionFn" to retrieve the correct collision function.
//			- Dynamic bodies check collision with EVERY OTHER dynamic body.
//			- Dynamic bodies check collision with EVERY static body.
//			- Static bodies should NOT check collisions against other bodies.
//			- Checks should be in this order: Dyn-vs-Dyn, Dyn-vs-Static
// ----------------------------------------------------------------------------
void CollisionSystem::CollideAllBodies()
{
	//those other bodies
	std::vector<Collider*>::iterator otherBodyIterator;

	Collider* mainBody;
	Collider* otherBody;
	Collider* staticBody;
	mCollisionIterations = 1;
	//reset collisions in this frame
	mCollisionsThisFrame = 0;
	//we loop as many times as collisions we have
	for (u32 N = 0; N < mCollisionIterations; N++)
	{
		for (auto mainBodyIterator = mDynamicBodies.begin(); mainBodyIterator != mDynamicBodies.end(); mainBodyIterator += 1)
		{
		    //other body will always be the next to main body at the start of every loop
		    otherBodyIterator = mainBodyIterator + 1;
		
		    mainBody = *mainBodyIterator;
		
		    //iterate through all the bodies
		    while (otherBodyIterator != mDynamicBodies.end())
		    {
		        //iterate doing the correct collision fn
		        otherBody = *otherBodyIterator;
			
				performCollision(*mainBody, *otherBody);
		
		        //go to the next body
		        otherBodyIterator += 1;
		    }
		
		    //check collisions of the main body with the static bodies
		    for (auto staticBodyIterator = mStaticBodies.begin(); staticBodyIterator != mStaticBodies.end(); staticBodyIterator += 1)
		    {
		
		        staticBody = *staticBodyIterator;
				  
		        performCollision(*mainBody, *staticBody);
		    }
		}
	}

	GetCollisionEvents();
}

void CollisionSystem::performCollision(Collider& mainBody, Collider& otherBody)
{
	if (mainBody.m_alive == false || otherBody.m_alive == false)
		return;

	CollisionPolicy policy = CollisionTable::Instance()->GetCollisionPolicy(mainBody.getCollisionGroup().getId(), otherBody.getCollisionGroup().getId());

	//No need to do collisions
	if (policy == CollisionPolicy::SkipDetection)
		return;

	//retarded stuff
	Contact collision;
	Contact* contactData = &collision;

	//save the respective function
	CollisionFn collisionToPerform = GetCollisionFn(mainBody, otherBody);

	if (mainBody.isGhost || otherBody.isGhost)
		policy = CollisionPolicy::SkipResolution;

	if (policy == CollisionPolicy::SkipResolution)
		contactData = nullptr;

	//if it collides resolve contact penetration and stuff
	if (collisionToPerform(mainBody, otherBody, contactData))
	{
		if (policy != CollisionPolicy::SkipEvents)
			thisFrameCollisions.push_back(CollisionData(mainBody, otherBody, contactData));

		//Resolve collisions
		ResolveContactPenetration(mainBody, otherBody, contactData);

		ResolveContactVelocity(mainBody, otherBody, contactData);

		//increase number of collisions this frame
		mCollisionsThisFrame++;
	}
}


void CollisionSystem::GetCollisionEvents()
{
	//for every collision that happened on the last frame
	for (auto & collision : LastFrameCollisions)
	{
		//check if it has happened on this frame
		auto collisionIterator = std::find(thisFrameCollisions.begin(), thisFrameCollisions.end(), collision);

		//if it hasn't happened on this frame, the collision has ended
		if (collisionIterator == thisFrameCollisions.end())
		{
			//on collision ended
			Messaging::EventDispatcher::get_instance().trigger_event_for_handler(collision.obj1->GetEventHandler(), OnCollisionEnded(collision));
			Messaging::EventDispatcher::get_instance().trigger_event_for_handler(collision.obj2->GetEventHandler(), OnCollisionEnded(collision));
			return;
		}
	}


	//for every collision on this frame
	for (auto & collision : thisFrameCollisions)
	{
		//look if this collision happened on the last frame
		auto collisionIterator = std::find(LastFrameCollisions.begin(), LastFrameCollisions.end(), collision);

		//if it didnt happen its a new collision
		if (collisionIterator == LastFrameCollisions.end())
		{
			//on collision started
			Messaging::EventDispatcher::get_instance().trigger_event_for_handler(collision.obj1->GetEventHandler(), OnCollisionStarted(collision));
			Messaging::EventDispatcher::get_instance().trigger_event_for_handler(collision.obj2->GetEventHandler(), OnCollisionStarted(collision));
			return;
		}
		else
		{
			//on collision persisted
			Messaging::EventDispatcher::get_instance().trigger_event_for_handler(collision.obj1->GetEventHandler(), OnCollisionPersisted(collision));
			Messaging::EventDispatcher::get_instance().trigger_event_for_handler(collision.obj2->GetEventHandler(), OnCollisionPersisted(collision));
		}
	}



}


// ----------------------------------------------------------------------------