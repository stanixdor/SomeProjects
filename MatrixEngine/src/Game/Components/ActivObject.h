// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior 
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:	    ActivObject.h 
//	Author:			Javier Tafur, javier.tafur
// ---------------------------------------------------------------------------

#pragma once

#include "Engine/GameObject/Component.h"
#include "Physics\CollisionEvents\OnCollisionEvent.h"

class ActivObject : public Component
{
public:

	//SERIALIZABLE_NO_PROPERTIES
		
		bool original_activate;
		bool is_active;

	//CTORS
	ActivObject();
	
	virtual ~ActivObject() {}

	
	virtual void ActivObjectInitialize() = 0;
	virtual void  ActivObjectUpdate() = 0;
	
	virtual bool ToggleActivate();
	virtual bool SetCurrentActive(bool activeState);


	virtual void Initialize();
	virtual void Update();

	virtual void ActivObject_collisionStarted(const OnCollisionStarted&) = 0;
	virtual void ActivObject_collisionPersisted(const OnCollisionPersisted&) = 0;
	virtual void ActivObject_collisionEnded(const OnCollisionEnded&) = 0;

	//event collision functions
	void collisionStarted(const OnCollisionStarted& mEvent);
	void collisionPersisted(const OnCollisionPersisted& mEvent);
	void collisionEnded(const OnCollisionEnded& mEvent);

};