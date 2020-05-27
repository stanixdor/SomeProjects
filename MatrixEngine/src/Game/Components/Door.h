// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior 
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:	    Door.h 
//	Purpose:		a component that is activable and have specific functions
//	Author:			Javier Tafur, javier.tafur
// ---------------------------------------------------------------------------

#pragma once

#include "Components\ActivObject.h"



 class Collider;

 namespace Graphics
 {
	 class Renderable;
 }

class Door : public ActivObject
{

	SERIALIZABLE_NO_PROPERTIES

	void ActivObjectInitialize() override;
	void ActivObjectUpdate() override;


	//event collision functions
	void ActivObject_collisionStarted(const OnCollisionStarted&) override;
	void ActivObject_collisionPersisted(const OnCollisionPersisted&) override;
	void ActivObject_collisionEnded(const OnCollisionEnded&) override;


	Graphics::Renderable * m_rend;
	Collider * m_collider;

};
