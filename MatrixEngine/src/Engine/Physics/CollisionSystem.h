#pragma once
// ---------------------------------------------------------------------------
#include "Engine/Physics/Collisions.h"
#include "Engine/Physics/CollisionResolution.h"
#include "Engine/Physics/Collider.h"
#include "Engine\Physics\CollisionData.h" //CollisionData struct
#include "Engine/Utils/ISingleton.h"
#include <vector>

// Collision restitution for velocity resolution
#define DFLT_RESTITUTION 0.908f;

/* @README
The functions below are provided for you. They are "wrapper functions" around collision tests.
These serve as the link between the collision library (the tests) and the physics/collision system.
In fact, they are the ones that actually call the functions in ContactCollisions.h.
Note that they all have the same signature. This is so that we can refer to them
via function pointers (see typedef below). This allows us to store these functions
as function pointers in the collision system following the method discussed in class.
(see mCollisionTests member variables of CollisionSystem below).
Finally	the functions guarantee that contact viewer is always body1. This is the
convention we followed in class.
*/


bool CollideCircles(Collider& body1, Collider& body2, Contact * c);
bool CollideAABBs(Collider& body1, Collider& body2, Contact * c);
bool CollideOBBs(Collider& body1, Collider& body2, Contact * c);
bool CollideAABBToCircle(Collider& body1, Collider& body2, Contact * c);
bool CollideOBBToCircle(Collider& body1, Collider& body2, Contact * c);
// typedef for function pointer CollisionFn


typedef bool(*CollisionFn)(Collider&, Collider&, Contact *);
/*!
*/
struct CollisionSystem : public ISingleton<CollisionSystem>
{
	// ------------------------------------------------------------------------
	// Member Variables
	std::vector<Collider*> mStaticBodies;
	std::vector<Collider*> mDynamicBodies;

	//std::vector<Collider*> mBodies;

	// Collision iterations
	u32 mCollisionIterations;
	u32 mCollisionsThisFrame; // collisions this frame

							  // Collision Tests -They are added to the collision system at initialize. 
							  // (see CollisionSystem::Init) for more details.
	CollisionFn mCollisionTests[CSHAPE_INDEX_MAX];

	// ------------------------------------------------------------------------
	// Member Functions

	// System Functions
	void Init();
	void Update();
	void changeColliderFromVector(const Collider& collider);
	void Shutdown();
	void endOfFrameUpdate();
	// Rigid Body Management
	void AddCollider(Collider& obj);
	void RemoveCollider(Collider& obj);
	void ClearBodies();
	// findeing the collision tests
    CollisionFn GetCollisionFn(Collider& mainBody, Collider& otherBody);

	// Collides and resolve all rigidbodies 
	void CollideAllBodies();

    /*!
    *****************************************************************************
    \fn void performCollision(Collider& obj1, Collider& obj2);
    
    \brief
        performs the correct type of collision depending on the Collision Group of the objects.
    \param
        Collider& obj1: first object to collide.
        Collider& obj2: second object to collide.
    \return
        None.
    *******************************************************************************/ 
    void performCollision(Collider& obj1, Collider& obj2);
	// Exposed solver
	void ResolveContactPenetration(Collider & obj1, Collider & obj2, Contact * contact);
	void ResolveContactVelocity(Collider & obj1, Collider & obj2, Contact * contact);

	void GetCollisionEvents();

	std::vector<CollisionData> thisFrameCollisions;
	std::vector<CollisionData> LastFrameCollisions;

};