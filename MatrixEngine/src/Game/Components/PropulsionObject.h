#pragma once

#include "GameObject\Component.h"
#include "Physics\CollisionEvents\OnCollisionEvent.h"

class PropulsionObject : public Component
{
public:
	SERIALIZABLE_NO_PROPERTIES;
	PropulsionObject();
	virtual ~PropulsionObject() {}
	
	virtual void Update() override; // update

	void Initialize();

	//event System functions
	void collisionStarted(const OnCollisionStarted& mEvent);
	void collisionPersisted(const OnCollisionPersisted& mEvent);
	void collisionEnded(const OnCollisionEnded& mEvent);

private:
	GameObject * Owner;	
};