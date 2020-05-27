// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior 
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:	    ActivableComponent.cpp 
//	Purpose:		use for the activation of level, buttons etc.
//	Author:			Javier Tafur, javier.tafur
// ---------------------------------------------------------------------------

#pragma once

#include "Engine/GameObject/Component.h"
#include "Platform\Time\FrameRateController.h"
#include "Physics\CollisionEvents\OnCollisionEvent.h"
#include "Components\ActivObject.h"
#include <stdio.h>
#include <vector>

//component detector for the enemies implies collision detection as way of detection:

namespace Graphics
{
	class Renderable;
}

class Activable : public Component
{
	public:

	SERIALIZABLE;

	//SERIALIZABLE_NO_PROPERTIES

	Activable() { m_name = "Activable"; }

	virtual void Initialize();
	virtual void Update();
	
	void LateUpdate();
	GameObject* mActivableOwner;
	float mTimerDuration = 8.0f;
	bool  mbUseTimer = false;
	bool  mbTimerRepeat = false;
	bool mbTimerActive = false;
	FRC::Timer timer;
	bool is_active;

	Graphics::Renderable * m_rend;


	// container of CogPaths for the serialization
	std::vector<CogPath> m_activableCogPaths;

	// container of lasers, doors, (activable stuff):
	std::vector<GameObject*> m_activableObjects;

	//event System functions
	void collisionStarted(const OnCollisionStarted& mEvent);
	void collisionPersisted(const OnCollisionPersisted& mEvent);
	void collisionEnded(const OnCollisionEnded& mEvent);
};