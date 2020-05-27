// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior 
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:	    CollectibleComponent.h
//	Purpose:		use for the collect object fly for the player like coins
//	Author:			Javier Tafur, javier.tafur
// ---------------------------------------------------------------------------

#pragma once

#include "GameObject\Component.h"
#include "GLM\glm.hpp"
#include "Engine\Physics\CollisionEvents\OnCollisionEvent.h"


class CollectibleComponent : public Component
{
public:

	SERIALIZABLE_NO_PROPERTIES;

	CollectibleComponent();

	~CollectibleComponent();

	// update
	virtual void Update() override;

	void Initialize();

	unsigned getCollectiblesTaken() const;


	//event System functions
	void collisionStarted(const OnCollisionStarted& mEvent);

	
private:
	static unsigned currentTotalCollectibles;	// the number of collectibles (coins)
	static unsigned originalCollectibleNumber; // the nuber of collectibles taken by the player


};
