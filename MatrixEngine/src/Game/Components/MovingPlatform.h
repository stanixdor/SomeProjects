#pragma once

#include "GameObject\Component.h"
#include "Physics\CollisionEvents\OnCollisionEvent.h"

class MovingPlatform : public Component
{
public:
	SERIALIZABLE_NO_PROPERTIES;

	MovingPlatform();
	virtual ~MovingPlatform() {}

	virtual void Update() override; // update

	void Initialize();

	//event System functions
	void collisionStarted(const OnCollisionStarted& mEvent);
	void collisionPersisted(const OnCollisionPersisted& mEvent);
	void collisionEnded(const OnCollisionEnded& mEvent);

	bool UpDown = false;
	float Speed = 1.0f;
	float MaxDistance = 4.0f;
	float Counter = 0.0f;
	
	GameObject * Owner;

};