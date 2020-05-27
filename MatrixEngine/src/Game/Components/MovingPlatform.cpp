#include "MovingPlatform.h"
#include "Engine\Platform\EventSystem\event_dispatcher.h"
#include "GameObject\GameObject.h"
#include "Physics/CollisionData.h"
#include "Physics\RigidBody.h"
#include "Physics\CollisionTable.h"
#include "Logic\StateMachine\StateMachine.h"
#include "Logic\StateMachine\PlayerStateMachine.h"
#include "Platform\Time\FrameRateController.h"

MovingPlatform::MovingPlatform() : Component("MovingPlatform")
{
}

void MovingPlatform::Update()
{
	if (UpDown)
	{
		if (Counter < MaxDistance)
		{
			Owner->m_transform->IncreasePosBy({ 0.0f, Speed });
			Counter += static_cast<float>(FRC::FrameRateController::Instance()->GetFrameTime());
		}
		else if(Counter < MaxDistance * 2.0f)
		{
			Owner->m_transform->IncreasePosBy({ 0.0f, -Speed });
			Counter += static_cast<float>(FRC::FrameRateController::Instance()->GetFrameTime());
		}
		else
		{
			Counter = 0.0f;
		}

		std::cout << "Counter MOV PLAT: " << Counter << std::endl;

	}
	else
	{
		if (Counter < MaxDistance)
		{
			Owner->m_transform->IncreasePosBy({ Speed, 0.0f });
			Counter += static_cast<float>(FRC::FrameRateController::Instance()->GetFrameTime());
		}
		else if (Counter < MaxDistance * 2.0f)
		{
			Owner->m_transform->IncreasePosBy({ -Speed, 0.0f });
			Counter += static_cast<float>(FRC::FrameRateController::Instance()->GetFrameTime());
		}
		else
		{
			Counter = 0.0f;
		}
	}
}

void MovingPlatform::Initialize()
{
	Owner = this->GetOwner();	//Save the object

	using namespace Messaging;
	Messaging::EventHandler* temp = &Owner->GetEventHandler();

	Owner->GetEventHandler().register_handler(this, &MovingPlatform::collisionStarted);
	Owner->GetEventHandler().register_handler(this, &MovingPlatform::collisionPersisted);
	Owner->GetEventHandler().register_handler(this, &MovingPlatform::collisionEnded);

	////subscribe this handler to this type of event
	Messaging::EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionStarted>());
	Messaging::EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionPersisted>());
	Messaging::EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionEnded>());
}

void MovingPlatform::collisionStarted(const OnCollisionStarted&)
{
}

void MovingPlatform::collisionPersisted(const OnCollisionPersisted&)
{
}

void MovingPlatform::collisionEnded(const OnCollisionEnded&)
{
}