// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior 
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:	    DetectorComponent.h 
//	Purpose:		use for the detection of the enemies over the player,
//  applies, collision detection and state machines.
//	Author:			Javier Tafur, javier.tafur
// ---------------------------------------------------------------------------

#pragma once

#include "Engine/GameObject/Component.h"
#include "Utils\CogPath.h"
#include "Physics\CollisionEvents\OnCollisionEvent.h"
#include "Logic\StateMachine\AnimationStateMachine.h"

//#include <stdio.h>

//component detector for the enemies implies collision detection as way of detection:

class Detector : public Component
{
	public:

	//SERIALIZABLE;

	SERIALIZABLE;
	//SERIALIZABLE_NO_PROPERTIES


	virtual void Initialize();
	virtual void Update();
	void activate();
	void deactivate();

	Detector() { m_name = "Detector"; }
	GameObject* mDetectorOwner;
	CogPath pathDetector;
	bool activated;
	float timeToActivate;

	FRC::Timer timer;

	//event System functions
	void collisionStarted(const OnCollisionStarted& mEvent);
	void collisionPersisted(const OnCollisionPersisted& mEvent);
};
