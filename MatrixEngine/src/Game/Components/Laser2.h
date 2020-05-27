// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior 
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:	    Laser.cpp 
//	Author:			Javier Tafur, javier.tafur
// ---------------------------------------------------------------------------

#pragma once

#include "Engine/GameObject/Component.h"
#include "Platform\Time\FrameRateController.h"
#include "Physics\CollisionEvents\OnCollisionEvent.h"
#include "Graphics\GraphicsComponents\Renderable.h"

class Laser2 : public Component
{
	public:

		SERIALIZABLE_NO_PROPERTIES

		bool is_active = true;

		Laser2();
		Graphics::Renderable* tex = nullptr;
		virtual void Initialize();
		virtual void Update();

		FRC::Timer timer;
		//event collision functions
		void collisionStarted(const OnCollisionStarted& mEvent);
		void collisionPersisted(const OnCollisionPersisted& mEvent);
		void collisionEnded(const OnCollisionEnded& mEvent);

};
