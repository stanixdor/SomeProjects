#pragma once

#include "GameObject\Component.h"
#include "Physics\CollisionEvents\OnCollisionEvent.h"

class ClimbableObject : public Component
{
public:
	SERIALIZABLE_NO_PROPERTIES;
	ClimbableObject();
	virtual ~ClimbableObject() {}

	virtual void Update() override; // update

	void Initialize();

	//event System functions
	void collisionStarted(const OnCollisionStarted& mEvent);
	void collisionPersisted(const OnCollisionPersisted& mEvent);
	void collisionEnded(const OnCollisionEnded& mEvent);

private:
	GameObject * Owner;
	GameObject * Playerptr;
};