// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior 
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:	    SteamComponent.cpp 
//	Author:			Javier Tafur, javier.tafur
// ---------------------------------------------------------------------------

#pragma once

#include "ActivObject.h"

namespace Graphics
{
	class AnimComp;
}


class Steam : public ActivObject
{
public:

	SERIALIZABLE_NO_PROPERTIES


	void ActivObjectInitialize() override;
	void ActivObjectUpdate() override;


	//event collision functions
	void ActivObject_collisionStarted(const OnCollisionStarted&) override;
	void ActivObject_collisionPersisted(const OnCollisionPersisted&) override;
	void ActivObject_collisionEnded(const OnCollisionEnded&) override;

	Graphics::AnimComp * m_Anim;
	Collider * m_collider;
};