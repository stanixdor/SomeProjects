// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior 
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:	    Checkpoint.h
//	Author:			Javier Tafur, javier.tafur
// ---------------------------------------------------------------------------

#pragma once


#include "Engine/GameObject/Component.h"

#include "Physics\CollisionEvents\OnCollisionEvent.h"
#include "Logic\StateMachine\AnimationStateMachine.h"

#include "GLM\glm.hpp"

class Checkpoint : public Component
{
	public:

	//SERIALIZABLE;

	SERIALIZABLE;
	Checkpoint();
	virtual void Initialize();
	virtual void Update();

	//boolena that give us the info if the player have been collinding with a 
	// checkpoint.
	bool is_check = false;

	//bool get_check(bool checkinfo);
	static glm::vec2 respawnPosition;

	//event System functions
	void collisionStarted(const OnCollisionStarted& mEvent);
	void collisionPersisted(const OnCollisionPersisted& mEvent);
	void collisionEnded(const OnCollisionEnded& mEvent);

};
