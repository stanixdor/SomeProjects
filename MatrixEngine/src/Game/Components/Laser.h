// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior 
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:	    Laser.cpp 
//	Author:			Javier Tafur, javier.tafur
// ---------------------------------------------------------------------------

#pragma once

#include "ActivObject.h"

namespace Graphics
{
	class Renderable;
}

class Collider;

class Laser : public ActivObject
{
	public:

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
