#pragma once
#include "Engine\Platform\EventSystem\event.h"

// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior 
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:	    DetectorComponent.cpp 
//	Purpose:		use for the detection of the enemies over the player,
//  applies, collision detection and state machines.
//	Author:			Javier Tafur, javier.tafur
// ---------------------------------------------------------------------------

class OnPlayerDetected: public Messaging::Event
{
	public:
	OnPlayerDetected() = default;
	virtual ~OnPlayerDetected() = default;
};

