// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior 
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:		playerStateMachine.cpp
//	Purpose:		
//	Author:			Jorge Alvarez, Javier Tafur
// ---------------------------------------------------------------------------


#include "GLM/glm.hpp"
#include "GameObject\GameObject.h"
#include "Physics\RigidBody.h"
#include "Engine/Physics/CollisionSystem.h"
#include "Engine\Platform\EventSystem\event_dispatcher.h"
#include "Engine/Graphics/GraphicsSystem.h"
#include "Components/Tongue.h"
#include "Physics\CollisionTable.h"
#include "Platform\Resource Manager\ResourceManager.h"
#include "Components/HeadComponent.h"
#include "Game/Logic/StateMachines/playerStateMachine.h"
#include "Engine/EngineDebug/ConsolePrinting.h"
#include "Components/CamouflageComponent.h"
#include "Components/FlyComponent.h"
#include "Components/Checkpoint.h"

REFLECTION(PlayerStateMachine, m_name, InitialPosition);

PlayerStateMachine * PlayerSM::PlayerState::PSM = nullptr;
bool PlayerStateMachine::teleportMode = false;

namespace PlayerSM
{
	//Helper Function Prototypes
	void ApplyDrag(float MaxVelocity, GameObject * Player, float drag_acceleration);
	void LimitVelocity(GameObject * Player, float max_velocity);
	void CheckJumpInput(StateMachine * playerStateMachine, bool OnAir);
	void CheckMoveRightInput(StateMachine * playerStateMachine);
	void CheckMoveLeftInput(StateMachine * playerStateMachine);
	void CheckMouseInputAndCreateTongue(GameObject * Player, GameObject * Tongue, StateMachine * playerStateMachine);
	void CreateTongue(GameObject * Tongue);
	GameObject * CreateCamouflageFly(GameObject * Player);

	//---------------------------------------------------------------------------/
	// IDLE STATE																 /
	//---------------------------------------------------------------------------/

	void Idle_State::collisionStarted(const OnCollisionStarted& mEvent)
	{
		getPlayerStateMachine()->OnAir = false;

		if (mEvent.mData.obj1->getCollisionGroup() == CollisionGroup::GetCollisionGroup("Floor") || mEvent.mData.obj2->getCollisionGroup() == CollisionGroup::GetCollisionGroup("Floor"))
		{
			PlayerRigidBody->mVelocity.y = 0.0f;
		}
	}

	void Idle_State::collisionPersisted(const OnCollisionPersisted&)
	{}

	void Idle_State::collisionEnded(const OnCollisionEnded& mEvent)
	{
		if (mEvent.mData.obj1->getCollisionGroup() == CollisionGroup::GetCollisionGroup("Floor") || mEvent.mData.obj2->getCollisionGroup() == CollisionGroup::GetCollisionGroup("Floor"))
		{
			getPlayerStateMachine()->OnAir = true;
		}
	}

	void Idle_State::Enter()
	{
		if (AnimationComp->mbAnimPlaying == false) //If the animation is paused play it
		{
			AnimationComp->PlayAnimation();
		}

		if (getPlayerStateMachine()->Player->m_transform->mScale.x > 0)
		{
			getPlayerStateMachine()->Player->m_transform->mScale = glm::vec2{ 70.0f, 100.0f };
			objectManager.FindObjectByName("PlayerHead")->m_transform->mScale = glm::vec2{ 70.0f, 100.0f };
		}
		else
		{
			getPlayerStateMachine()->Player->m_transform->mScale = glm::vec2{ -70.0f, 100.0f };
			objectManager.FindObjectByName("PlayerHead")->m_transform->mScale = glm::vec2{ -70.0f, 100.0f };
		}

		//if (getPlayerStateMachine()->Player->m_transform->mScale.x > 0)
		//{
		//	getPlayerStateMachine()->Player->m_transform->mScale = glm::vec2{ 100.0f, 100.0f };
		//	objectManager.FindObjectByName("PlayerHead")->m_transform->mScale = glm::vec2{ 100.0f, 100.0f };
		//}
		//else
		//{
		//	getPlayerStateMachine()->Player->m_transform->mScale = glm::vec2{ -100.0f, 100.0f };
		//	objectManager.FindObjectByName("PlayerHead")->m_transform->mScale = glm::vec2{ -100.0f, 100.0f };
		//}
	}

	void Idle_State::initialize()
	{
		this->mName = "Idle_State";

		getPlayerStateMachine()->Player = mOwnerStateMachine->GetOwner();
		getPlayerStateMachine()->playerStateMachine = mOwnerStateMachine;
		AnimationComp = getPlayerStateMachine()->Player->FindComponentByType<Graphics::AnimComp>();
		PlayerRigidBody = getPlayerStateMachine()->Player->FindComponentByType<RigidBody>();

		if (InitializedFlip == false && getPlayerStateMachine()->Player->m_transform->mScale.x < 0.0f)
		{
			getPlayerStateMachine()->Player->FindComponentByType<Graphics::AnimComp>()->FlipAnimation();
			getPlayerStateMachine()->Player->FindComponentByType<Graphics::AnimComp>()->flipX = Right;
			InitializedFlip = true;
		}

		//AnimationComp->flipX = false; //Set initial orientation of animation looking right

		using namespace Messaging;
		Messaging::EventHandler * EventHandler_ = &mOwnerStateMachine->GetOwner()->GetEventHandler();

		////register object and function
		EventHandler_->register_handler(this, &Idle_State::collisionStarted);
		EventHandler_->register_handler(this, &Idle_State::collisionPersisted);
		EventHandler_->register_handler(this, &Idle_State::collisionEnded);

		////subscribe this handler to this type of event
		Messaging::EventDispatcher::get_instance().subscribe(*EventHandler_, type_of<OnCollisionStarted>());
		Messaging::EventDispatcher::get_instance().subscribe(*EventHandler_, type_of<OnCollisionPersisted>());
		Messaging::EventDispatcher::get_instance().subscribe(*EventHandler_, type_of<OnCollisionEnded>());
	}

	void Idle_State::Exit()
	{
	}

	void Idle_State::Update()
	{
		if (getPlayerStateMachine()->Player != nullptr)
		{
			if (InitializedFlip == false && getPlayerStateMachine()->Player->m_transform->mScale.x < 0.0f)
			{
				getPlayerStateMachine()->Player->FindComponentByType<Graphics::AnimComp>()->FlipAnimation();
				getPlayerStateMachine()->Player->FindComponentByType<Graphics::AnimComp>()->flipX = Right;
				InitializedFlip = true;
			}
		}

		getPlayerStateMachine()->UpdatePlayerOrientation(AnimationComp);

		if (getPlayerStateMachine()->OnAir == true)
		{
			mOwnerStateMachine->ChangeState<Falling_Jump_State>();
		}

		if (!getPlayerStateMachine()->OnAir)
		{
			ApplyDrag(20.0f, getPlayerStateMachine()->Player, getPlayerStateMachine()->drag_acceleration);
			LimitVelocity(getPlayerStateMachine()->Player, getPlayerStateMachine()->max_velocity);
		}

		CheckMouseInputAndCreateTongue(getPlayerStateMachine()->Player, getPlayerStateMachine()->Tongue, getPlayerStateMachine()->playerStateMachine);
		CheckJumpInput(getPlayerStateMachine()->playerStateMachine, getPlayerStateMachine()->OnAir);
		CheckMoveRightInput(getPlayerStateMachine()->playerStateMachine);
		CheckMoveLeftInput(getPlayerStateMachine()->playerStateMachine);
	}

	//---------------------------------------------------------------------------/
	// ELEVATING JUMP STATE																 /
	//---------------------------------------------------------------------------/
	void Elevating_Jump_State::initialize()
	{
		this->mName = "Elevating_Jump_State";

		PlayerRigidBody = getPlayerStateMachine()->Player->FindComponentByType<RigidBody>();
		AnimationComp = getPlayerStateMachine()->Player->FindComponentByType<Graphics::AnimComp>();

		using namespace Messaging;
		Messaging::EventHandler* EventHandler_ = &mOwnerStateMachine->GetOwner()->GetEventHandler();

		////register object and function
		EventHandler_->register_handler(this, &Elevating_Jump_State::collisionStarted);
		EventHandler_->register_handler(this, &Elevating_Jump_State::collisionPersisted);
		EventHandler_->register_handler(this, &Elevating_Jump_State::collisionEnded);

		////subscribe this handler to this type of event
		Messaging::EventDispatcher::get_instance().subscribe(*EventHandler_, type_of<OnCollisionStarted>());
		Messaging::EventDispatcher::get_instance().subscribe(*EventHandler_, type_of<OnCollisionPersisted>());
		Messaging::EventDispatcher::get_instance().subscribe(*EventHandler_, type_of<OnCollisionEnded>());
	}

	void Elevating_Jump_State::collisionStarted(const OnCollisionStarted&)
	{}

	void Elevating_Jump_State::collisionPersisted(const OnCollisionPersisted&)
	{}

	void Elevating_Jump_State::collisionEnded(const OnCollisionEnded& mEvent)
	{
		if (mEvent.mData.obj1->getCollisionGroup() == CollisionGroup::GetCollisionGroup("Floor") || mEvent.mData.obj2->getCollisionGroup() == CollisionGroup::GetCollisionGroup("Floor"))
		{
			getPlayerStateMachine()->OnAir = true;
		}
	}

	float ElevatingCounter = 0.0f;
	void Elevating_Jump_State::Enter()
	{
		if (getPlayerStateMachine()->Player->m_transform->mScale.x > 0)
		{
			getPlayerStateMachine()->Player->m_transform->mScale = glm::vec2{ 100.0f, 100.0f };
			objectManager.FindObjectByName("PlayerHead")->m_transform->mScale = glm::vec2{ 100.0f, 100.0f };
		}
		else
		{
			getPlayerStateMachine()->Player->m_transform->mScale = glm::vec2{ -100.0f, 100.0f };
			objectManager.FindObjectByName("PlayerHead")->m_transform->mScale = glm::vec2{ -100.0f, 100.0f };
		}

		ElevatingCounter = 0.0f;
	}

	void Elevating_Jump_State::Exit()
	{}


	float ElevatingDelayTime = 0.15f;
	bool change_st = false;
	float OnAirSpeed = 20.0f;
	void Elevating_Jump_State::Update()
	{
		getPlayerStateMachine()->UpdatePlayerOrientation(AnimationComp);



		if (getPlayerStateMachine()->OnAir == true && PlayerRigidBody->mVelocity.y < 0.0f)
		{
			change_st = false;
			mOwnerStateMachine->ChangeState<Falling_Jump_State>();
			return;
		}

		//Timer before jumping

		if (ElevatingCounter < ElevatingDelayTime)
		{
			ElevatingCounter += 0.016f;
		}
		else
		{
			change_st = true;
			ElevatingCounter = 0.0f;
		}

		if (change_st)
		{
			if (!getPlayerStateMachine()->OnAir)
			{
				PlayerRigidBody->AddForce(glm::vec2(0.0f, getPlayerStateMachine()->JumpForce));
				getPlayerStateMachine()->OnAir = true;
			}
			else
			{
				if (Input::Instance()->KeyTriggered('D') || Input::Instance()->KeyTriggered('A'))
				{
					PlayerRigidBody->mVelocity.x = 0;
				}
				if (Input::Instance()->KeyPressed('D'))
				{
					PlayerRigidBody->mVelocity.x += OnAirSpeed;
				}
				if (Input::Instance()->KeyPressed('A'))
				{
					PlayerRigidBody->mVelocity.x -= OnAirSpeed;
				}
			}
		}

		CheckMouseInputAndCreateTongue(getPlayerStateMachine()->Player, getPlayerStateMachine()->Tongue, getPlayerStateMachine()->playerStateMachine);
	}

	//---------------------------------------------------------------------------/
	// FALLING JUMP STATE																 /
	//---------------------------------------------------------------------------/
	void Falling_Jump_State::initialize()
	{
		this->mName = "Falling_Jump_State";

		PlayerRigidBody = getPlayerStateMachine()->Player->FindComponentByType<RigidBody>();
		AnimationComp = getPlayerStateMachine()->Player->FindComponentByType<Graphics::AnimComp>();

		using namespace Messaging;
		Messaging::EventHandler* EventHandler_ = &mOwnerStateMachine->GetOwner()->GetEventHandler();

		////register object and function
		EventHandler_->register_handler(this, &Falling_Jump_State::collisionStarted);
		EventHandler_->register_handler(this, &Falling_Jump_State::collisionPersisted);
		EventHandler_->register_handler(this, &Falling_Jump_State::collisionEnded);

		////subscribe this handler to this type of event
		Messaging::EventDispatcher::get_instance().subscribe(*EventHandler_, type_of<OnCollisionStarted>());
		Messaging::EventDispatcher::get_instance().subscribe(*EventHandler_, type_of<OnCollisionPersisted>());
		Messaging::EventDispatcher::get_instance().subscribe(*EventHandler_, type_of<OnCollisionEnded>());
	}

	void Falling_Jump_State::collisionStarted(const OnCollisionStarted& mEvent)
	{
		getPlayerStateMachine()->OnAir = false;

		if (mEvent.mData.obj1->getCollisionGroup() == CollisionGroup::GetCollisionGroup("Floor") || mEvent.mData.obj2->getCollisionGroup() == CollisionGroup::GetCollisionGroup("Floor"))
		{
			PlayerRigidBody->mVelocity.y = 0.0f;
			mOwnerStateMachine->ChangeState<Landing_Jump_State>();
		}
	}

	void Falling_Jump_State::collisionPersisted(const OnCollisionPersisted&)
	{}

	void Falling_Jump_State::collisionEnded(const OnCollisionEnded&)
	{}

	void Falling_Jump_State::Enter()
	{
		if (getPlayerStateMachine()->Player->FindComponentByType<Graphics::AnimComp>()->flipX == Right)
		{
			getPlayerStateMachine()->Player->m_transform->mScale.x = 50.0f;
			getPlayerStateMachine()->Player->m_transform->mScale.y = 100.0f;
		}
		else
		{
			getPlayerStateMachine()->Player->m_transform->mScale.x = -50.0f;
			getPlayerStateMachine()->Player->m_transform->mScale.y = 100.0f;
		}
	}

	void Falling_Jump_State::Exit()
	{}

	void Falling_Jump_State::Update()
	{
		if (getPlayerStateMachine()->Player->FindComponentByType<AnimComp>()->mbAnimPlaying == false)
		{
			getPlayerStateMachine()->Player->FindComponentByType<AnimComp>()->mbAnimPlaying = true;
		}

		getPlayerStateMachine()->UpdatePlayerOrientation(AnimationComp);

		if (Input::Instance()->KeyTriggered('D') || Input::Instance()->KeyTriggered('A'))
		{
			PlayerRigidBody->mVelocity.x = 0;
		}
		if (Input::Instance()->KeyPressed('D'))
		{
			PlayerRigidBody->mVelocity.x += OnAirSpeed;
		}
		if (Input::Instance()->KeyPressed('A'))
		{
			PlayerRigidBody->mVelocity.x -= OnAirSpeed;
		}

		CheckMouseInputAndCreateTongue(getPlayerStateMachine()->Player, getPlayerStateMachine()->Tongue, getPlayerStateMachine()->playerStateMachine);
	}

	//---------------------------------------------------------------------------/
	// LANDING JUMP STATE																 /
	//---------------------------------------------------------------------------/
	void Landing_Jump_State::initialize()
	{
		this->mName = "Landing_Jump_State";

		PlayerRigidBody = getPlayerStateMachine()->Player->FindComponentByType<RigidBody>();
		AnimationComp = getPlayerStateMachine()->Player->FindComponentByType<Graphics::AnimComp>();

		using namespace Messaging;
		Messaging::EventHandler* EventHandler_ = &mOwnerStateMachine->GetOwner()->GetEventHandler();

		////register object and function
		EventHandler_->register_handler(this, &Landing_Jump_State::collisionStarted);
		EventHandler_->register_handler(this, &Landing_Jump_State::collisionPersisted);
		EventHandler_->register_handler(this, &Landing_Jump_State::collisionEnded);

		////subscribe this handler to this type of event
		Messaging::EventDispatcher::get_instance().subscribe(*EventHandler_, type_of<OnCollisionStarted>());
		Messaging::EventDispatcher::get_instance().subscribe(*EventHandler_, type_of<OnCollisionPersisted>());
		Messaging::EventDispatcher::get_instance().subscribe(*EventHandler_, type_of<OnCollisionEnded>());
	}

	void Landing_Jump_State::collisionStarted(const OnCollisionStarted&)
	{}

	void Landing_Jump_State::collisionPersisted(const OnCollisionPersisted&)
	{}

	void Landing_Jump_State::collisionEnded(const OnCollisionEnded&)
	{}

	float LandingCounter = 0.0f;

	void Landing_Jump_State::Enter()
	{
		if (getPlayerStateMachine()->Player->FindComponentByType<Graphics::AnimComp>()->flipX == Right)
		{
			getPlayerStateMachine()->Player->m_transform->mScale.x = 100.0f;
			getPlayerStateMachine()->Player->m_transform->mScale.y = 100.0f;
		}
		else
		{
			getPlayerStateMachine()->Player->m_transform->mScale.x = -100.0f;
			getPlayerStateMachine()->Player->m_transform->mScale.y = 100.0f;
		}
		LandingCounter = 0.0f;
	}

	void Landing_Jump_State::Exit()
	{}


	float LandingDelayTime = 0.1f;

	void Landing_Jump_State::Update()
	{
		getPlayerStateMachine()->UpdatePlayerOrientation(AnimationComp);

		//CheckMouseInputAndCreateTongue(getPlayerStateMachine()->Player, getPlayerStateMachine()->Tongue, getPlayerStateMachine()->playerStateMachine);

		if (LandingCounter < LandingDelayTime)
		{
			LandingCounter += 0.016f;
		}
		else
		{
			mOwnerStateMachine->ChangeState<Idle_State>();
			LandingCounter = 0.0f;
		}

		ApplyDrag(20.0f, getPlayerStateMachine()->Player, getPlayerStateMachine()->drag_acceleration);
		LimitVelocity(getPlayerStateMachine()->Player, getPlayerStateMachine()->max_velocity);
	}

	//---------------------------------------------------------------------------/
	// MOVE LEFT STATE															 /
	//---------------------------------------------------------------------------/
	void MoveLeft_State::initialize()
	{
		this->mName = "MoveLeft_State";


		PlayerRigidBody = getPlayerStateMachine()->Player->FindComponentByType<RigidBody>();
		AnimationComp = getPlayerStateMachine()->Player->FindComponentByType<Graphics::AnimComp>();

		using namespace Messaging;
		Messaging::EventHandler* EventHandler_ = &mOwnerStateMachine->GetOwner()->GetEventHandler();

		////register object and function
		EventHandler_->register_handler(this, &MoveLeft_State::collisionStarted);
		EventHandler_->register_handler(this, &MoveLeft_State::collisionPersisted);
		EventHandler_->register_handler(this, &MoveLeft_State::collisionEnded);

		////subscribe this handler to this type of event
		Messaging::EventDispatcher::get_instance().subscribe(*EventHandler_, type_of<OnCollisionStarted>());
		Messaging::EventDispatcher::get_instance().subscribe(*EventHandler_, type_of<OnCollisionPersisted>());
		Messaging::EventDispatcher::get_instance().subscribe(*EventHandler_, type_of<OnCollisionEnded>());
	}

	void MoveLeft_State::collisionStarted(const OnCollisionStarted& mEvent)
	{
		getPlayerStateMachine()->OnAir = false;

		if (mEvent.mData.obj1->getCollisionGroup() == CollisionGroup::GetCollisionGroup("floor") || mEvent.mData.obj2->getCollisionGroup() == CollisionGroup::GetCollisionGroup("floor"))
		{
			PlayerRigidBody->mVelocity.y = 0.0f;
		}
	}

	void MoveLeft_State::collisionPersisted(const OnCollisionPersisted&)
	{}

	void MoveLeft_State::collisionEnded(const OnCollisionEnded& mEvent)
	{
		if (mEvent.mData.obj1->getCollisionGroup() == CollisionGroup::GetCollisionGroup("Floor") || mEvent.mData.obj2->getCollisionGroup() == CollisionGroup::GetCollisionGroup("Floor"))
		{
			getPlayerStateMachine()->OnAir = true;
		}
	}

	void MoveLeft_State::Enter()
	{
		if (getPlayerStateMachine()->Player->m_transform->mScale.x > 0)
		{
			getPlayerStateMachine()->Player->m_transform->mScale = glm::vec2{ 100.0f, 100.0f };
			objectManager.FindObjectByName("PlayerHead")->m_transform->mScale = glm::vec2{ 100.0f, 100.0f };
		}
		else
		{
			getPlayerStateMachine()->Player->m_transform->mScale = glm::vec2{ -100.0f, 100.0f };
			objectManager.FindObjectByName("PlayerHead")->m_transform->mScale = glm::vec2{ -100.0f, 100.0f };
		}

		if (AnimationComp->mbAnimPlaying == false) //Looking right
		{
			AnimationComp->PlayAnimation();
		}
	}

	void MoveLeft_State::Exit()
	{}

	void MoveLeft_State::Update()
	{
		if (getPlayerStateMachine()->InitializedflipMovement == false && getPlayerStateMachine()->Player->m_transform->mScale.x > 0.0f)
		{
			getPlayerStateMachine()->Player->FindComponentByType<Graphics::AnimComp>()->FlipAnimation();
			getPlayerStateMachine()->Player->FindComponentByType<Graphics::AnimComp>()->flipX = Left;
			getPlayerStateMachine()->InitializedflipMovement = true;
		}

		getPlayerStateMachine()->UpdatePlayerOrientation(AnimationComp);

		LimitVelocity(getPlayerStateMachine()->Player, getPlayerStateMachine()->max_velocity);

		if (PlayerRigidBody->mVelocity.x > 0.0f)
		{
			PlayerRigidBody->mVelocity.x -= getPlayerStateMachine()->ChangeDirHelp;
		}

		PlayerRigidBody->mVelocity += glm::vec2(-getPlayerStateMachine()->max_velocity, 0.0f);

		CheckMouseInputAndCreateTongue(getPlayerStateMachine()->Player, getPlayerStateMachine()->Tongue, getPlayerStateMachine()->playerStateMachine);
		CheckJumpInput(getPlayerStateMachine()->playerStateMachine, getPlayerStateMachine()->OnAir);

		if (!Input::Instance()->KeyPressed('A'))
		{
			if (getPlayerStateMachine()->OnAir == false)
			{
				mOwnerStateMachine->ChangeState<Idle_State>();
			}
			else
			{
				mOwnerStateMachine->ChangeState<Falling_Jump_State>();
			}
		}
	}

	//---------------------------------------------------------------------------/
	// MOVE RIGHT STATE															 /
	//---------------------------------------------------------------------------/
	void MoveRight_State::initialize()
	{
		this->mName = "MoveRight_State";


		PlayerRigidBody = getPlayerStateMachine()->Player->FindComponentByType<RigidBody>();
		AnimationComp = getPlayerStateMachine()->Player->FindComponentByType<Graphics::AnimComp>();

		using namespace Messaging;
		Messaging::EventHandler* EventHandler_ = &mOwnerStateMachine->GetOwner()->GetEventHandler();

		////register object and function
		EventHandler_->register_handler(this, &MoveRight_State::collisionStarted);
		EventHandler_->register_handler(this, &MoveRight_State::collisionPersisted);
		EventHandler_->register_handler(this, &MoveRight_State::collisionEnded);

		////subscribe this handler to this type of event
		Messaging::EventDispatcher::get_instance().subscribe(*EventHandler_, type_of<OnCollisionStarted>());
		Messaging::EventDispatcher::get_instance().subscribe(*EventHandler_, type_of<OnCollisionPersisted>());
		Messaging::EventDispatcher::get_instance().subscribe(*EventHandler_, type_of<OnCollisionEnded>());
	}

	void MoveRight_State::collisionStarted(const OnCollisionStarted& mEvent)
	{
		getPlayerStateMachine()->OnAir = false;
		if (mEvent.mData.obj1->getCollisionGroup() == CollisionGroup::GetCollisionGroup("floor") || mEvent.mData.obj2->getCollisionGroup() == CollisionGroup::GetCollisionGroup("floor"))
		{
			PlayerRigidBody->mVelocity.y = 0.0f;
		}
	}

	void MoveRight_State::collisionPersisted(const OnCollisionPersisted&)
	{}

	void MoveRight_State::collisionEnded(const OnCollisionEnded& mEvent)
	{
		if (mEvent.mData.obj1->getCollisionGroup() == CollisionGroup::GetCollisionGroup("Floor") || mEvent.mData.obj2->getCollisionGroup() == CollisionGroup::GetCollisionGroup("Floor"))
		{
			getPlayerStateMachine()->OnAir = true;
		}
	}

	void MoveRight_State::Enter()
	{
		if (getPlayerStateMachine()->Player->m_transform->mScale.x > 0)
		{
			getPlayerStateMachine()->Player->m_transform->mScale = glm::vec2{ 100.0f, 100.0f };
			objectManager.FindObjectByName("PlayerHead")->m_transform->mScale = glm::vec2{ 100.0f, 100.0f };
		}
		else
		{
			getPlayerStateMachine()->Player->m_transform->mScale = glm::vec2{ -100.0f, 100.0f };
			objectManager.FindObjectByName("PlayerHead")->m_transform->mScale = glm::vec2{ -100.0f, 100.0f };
		}

		//set animation to default (right)
		//if (!AnimationComp->flipX)
		//{
		//	AnimationComp->FlipAnimation();
		//}

		if (AnimationComp->mbAnimPlaying == false) //Looking right
		{
			AnimationComp->PlayAnimation();
		}
	}

	void MoveRight_State::Exit()
	{}

	void MoveRight_State::Update()
	{
		if (getPlayerStateMachine()->InitializedflipMovement == false && getPlayerStateMachine()->Player->m_transform->mScale.x < 0.0f)
		{
			getPlayerStateMachine()->Player->FindComponentByType<Graphics::AnimComp>()->FlipAnimation();
			getPlayerStateMachine()->Player->FindComponentByType<Graphics::AnimComp>()->flipX = Right;
			getPlayerStateMachine()->InitializedflipMovement = true;
		}

		getPlayerStateMachine()->UpdatePlayerOrientation(AnimationComp);

		LimitVelocity(getPlayerStateMachine()->Player, getPlayerStateMachine()->max_velocity);

		if (PlayerRigidBody->mVelocity.x < 0.0f)
		{
			PlayerRigidBody->mVelocity.x += getPlayerStateMachine()->ChangeDirHelp;
		}

		PlayerRigidBody->mVelocity += glm::vec2(getPlayerStateMachine()->max_velocity, 0.0f);

		CheckMouseInputAndCreateTongue(getPlayerStateMachine()->Player, getPlayerStateMachine()->Tongue, getPlayerStateMachine()->playerStateMachine);
		CheckJumpInput(getPlayerStateMachine()->playerStateMachine, getPlayerStateMachine()->OnAir);

		if (!Input::Instance()->KeyPressed('D'))
		{
			if (getPlayerStateMachine()->OnAir == false)
			{
				mOwnerStateMachine->ChangeState<Idle_State>();
			}
			else
			{
				mOwnerStateMachine->ChangeState<Falling_Jump_State>();
			}
		}
	}

	//---------------------------------------------------------------------------/
	// THROW TONGUE STATE														 /
	//---------------------------------------------------------------------------/

	void ThrowTongue_State::collisionStarted(const OnCollisionStarted&)
	{}

	void ThrowTongue_State::collisionPersisted(const OnCollisionPersisted&)
	{}

	void ThrowTongue_State::collisionEnded(const OnCollisionEnded&)
	{}

	void ThrowTongue_State::Enter()
	{
		if (getPlayerStateMachine()->Player->m_transform->mScale.x > 0)
		{
			getPlayerStateMachine()->Player->m_transform->mScale = glm::vec2{ 100.0f, 100.0f };
			objectManager.FindObjectByName("PlayerHead")->m_transform->mScale = glm::vec2{ 100.0f, 100.0f };
		}
		else
		{
			getPlayerStateMachine()->Player->m_transform->mScale = glm::vec2{ -100.0f, 100.0f };
			objectManager.FindObjectByName("PlayerHead")->m_transform->mScale = glm::vec2{ -100.0f, 100.0f };
		}

		if (objectManager.FindObjectByName("ThrowHead") && objectManager.FindObjectByName("ThrowHead")->FindComponentByType<AnimComp>())
		{
			objectManager.FindObjectByName("ThrowHead")->FindComponentByType<AnimComp>()->ResetAnimation();
		}

		auto AnimComp = getPlayerStateMachine()->Player->FindComponentByType<Graphics::AnimComp>();

		if (getPlayerStateMachine()->mPrevState->mName == "Elevating_Jump_State")
		{
			//Elevating_Jump_State
			AnimComp->CompColumns = 5;
			AnimComp->CompRows = 1;
			AnimComp->CompSprites = 5;
			AnimComp->CompDuration = 1;
			AnimComp->CompDelays.resize(5);

			AnimComp->SetLoop(true);
			AnimComp->pTexture = ResourceManager::textures["Body anticipation walkidle sprite sheet.png"].get();

			AnimComp->CompDelays[0] = 0.084f;
			AnimComp->CompDelays[2] = 0.042f;
			AnimComp->CompDelays[1] = 0.126f;
			AnimComp->CompDelays[3] = 999.0f;
		}
		else if (getPlayerStateMachine()->mPrevState->mName == "Falling_Jump_State")
		{
			//Falling_Jump_State
			AnimComp->CompColumns = 5;
			AnimComp->CompRows = 1;
			AnimComp->CompSprites = 5;
			AnimComp->CompDuration = 1;
			AnimComp->CompDelays.resize(5);
			AnimComp->SetLoop(true);
			AnimComp->pTexture = ResourceManager::textures["Body falling loop sprite sheet.png"].get();

			for (auto& delay : AnimComp->CompDelays)
				delay = 0.08333f;
		}
		else if (getPlayerStateMachine()->mPrevState->mName == "Landing_Jump_State")
		{
			//Landing_Jump_State
			AnimComp->CompColumns = 3;
			AnimComp->CompRows = 1;
			AnimComp->CompSprites = 3;
			AnimComp->CompDuration = 1;
			AnimComp->CompDelays.resize(3);
			AnimComp->SetLoop(true);
			AnimComp->pTexture = ResourceManager::textures["Body landing sprite sheet.png"].get();

			AnimComp->CompDelays[0] = 0.042f;
			AnimComp->CompDelays[1] = 999.0f;
		}
		else if (getPlayerStateMachine()->mPrevState->mName == "HorizontalClimb_State")
		{
			//HorizontalClimb_State
			AnimComp->CompColumns = 10;
			AnimComp->CompRows = 1;
			AnimComp->CompDuration = 1;
			AnimComp->CompSprites = 10;
			AnimComp->CompDelays.resize(10);
			AnimComp->SetLoop(true);
			AnimComp->pTexture = ResourceManager::textures["Body sprite sheet.png"].get();

			for (auto& delay : AnimComp->CompDelays)
				delay = 0.084f;
		}
		else if (getPlayerStateMachine()->mPrevState->mName == "VerticalClimb_State")
		{
			//VerticalClimb_State
			AnimComp->CompColumns = 12;
			AnimComp->CompRows = 1;
			AnimComp->CompDuration = 1;
			AnimComp->CompSprites = 12;
			AnimComp->CompDelays.resize(12);
			AnimComp->SetLoop(true);
			AnimComp->pTexture = ResourceManager::textures["Stick pared spritesheet.png"].get();

			AnimComp->CompDelays[0] = AnimComp->CompDelays[5] = AnimComp->CompDelays[6] = AnimComp->CompDelays[11] = 0.042f;
			AnimComp->CompDelays[1] = AnimComp->CompDelays[2] = AnimComp->CompDelays[3] = AnimComp->CompDelays[4] = AnimComp->CompDelays[7] = AnimComp->CompDelays[8] = AnimComp->CompDelays[9] = AnimComp->CompDelays[10] = 0.084f;
		}
		else if (getPlayerStateMachine()->mPrevState->mName == "ZClimb_State")
		{
			//ZClimb_State
			AnimComp->CompColumns = 10;
			AnimComp->CompRows = 1;
			AnimComp->CompSprites = 10;
			AnimComp->CompDuration = 1;
			AnimComp->CompDelays.resize(10);
			AnimComp->SetLoop(true);
			AnimComp->pTexture = ResourceManager::textures["Escalar sprite sheet.png"].get();

			AnimComp->CompDelays[0] = AnimComp->CompDelays[1] = AnimComp->CompDelays[2] = AnimComp->CompDelays[4] = AnimComp->CompDelays[5] = AnimComp->CompDelays[6] = AnimComp->CompDelays[7] = AnimComp->CompDelays[9] = 0.084f;
			AnimComp->CompDelays[3] = AnimComp->CompDelays[8] = 0.042f;
		}
		else if (getPlayerStateMachine()->mPrevState->mName == "MoveLeft_State")
		{
			//MoveLeft_State
			AnimComp->CompColumns = 10;
			AnimComp->CompRows = 1;
			AnimComp->CompSprites = 10;
			AnimComp->CompDuration = 1;
			AnimComp->CompDelays.resize(10);
			AnimComp->SetLoop(true);
			AnimComp->pTexture = ResourceManager::textures["Dibo-Walk_SpriteSheet_Body.png"].get();

			for (auto& delay : AnimComp->CompDelays)
				delay = 0.084f;
		}
		else if (getPlayerStateMachine()->mPrevState->mName == "MoveRight_State")
		{
			//MoveRight_State
			AnimComp->CompColumns = 10;
			AnimComp->CompRows = 1;
			AnimComp->CompSprites = 10;
			AnimComp->CompDuration = 1;
			AnimComp->CompDelays.resize(10);
			AnimComp->SetLoop(true);
			AnimComp->pTexture = ResourceManager::textures["Dibo-Walk_SpriteSheet_Body.png"].get();

			for (auto& delay : AnimComp->CompDelays)
				delay = 0.084f;
		}
		else if (getPlayerStateMachine()->mPrevState->mName == "Idle_State")
		{
			//Idle_State
			AnimComp->CompColumns = 17;
			AnimComp->CompRows = 1;
			AnimComp->CompSprites = 17;
			AnimComp->CompDuration = 1;
			AnimComp->CompDelays.resize(17);
			AnimComp->SetLoop(true);
			AnimComp->pTexture = ResourceManager::textures["idle prite sheet.png"].get();

			for (auto& delay : AnimComp->CompDelays)
				delay = 0.042f;
		}
	}

	void ThrowTongue_State::initialize()
	{
		this->mName = "ThrowTongue_State";


		using namespace Messaging;
		Messaging::EventHandler* EventHandler_ = &mOwnerStateMachine->GetOwner()->GetEventHandler();

		////register object and function
		EventHandler_->register_handler(this, &ThrowTongue_State::collisionStarted);
		EventHandler_->register_handler(this, &ThrowTongue_State::collisionPersisted);
		EventHandler_->register_handler(this, &ThrowTongue_State::collisionEnded);

		////subscribe this handler to this type of event
		Messaging::EventDispatcher::get_instance().subscribe(*EventHandler_, type_of<OnCollisionStarted>());
		Messaging::EventDispatcher::get_instance().subscribe(*EventHandler_, type_of<OnCollisionPersisted>());
		Messaging::EventDispatcher::get_instance().subscribe(*EventHandler_, type_of<OnCollisionEnded>());
	}

	void ThrowTongue_State::Exit()
	{
		objectManager.FindObjectByName("PlayerHead")->FindComponentByType<Renderable>()->mTextureName = "headclosed.png";
	}

	void ThrowTongue_State::Update()
	{
		if (getPlayerStateMachine()->Tongue == nullptr)
		{
			mOwnerStateMachine->mNextState = mOwnerStateMachine->mPrevState;

			mOwnerStateMachine->mPrevState = mOwnerStateMachine->mCurrentState;

			//mOwnerStateMachine->ChangeState<VerticalClimb_State>()
			//
			//if (getPlayerStateMachine()->IsClimbing_V)
			//{
			//	mOwnerStateMachine->ChangeState<VerticalClimb_State>();
			//}
			//else if (getPlayerStateMachine()->IsClimbing_H)
			//{
			//	mOwnerStateMachine->ChangeState<HorizontalClimb_State>();				
			//}
			//else if (getPlayerStateMachine()->OnAir == false)
			//{
			//	mOwnerStateMachine->ChangeState<Idle_State>();
			//}
			//else
			//{
			//	mOwnerStateMachine->ChangeState<Falling_Jump_State>();
			//}
		}

		CheckJumpInput(getPlayerStateMachine()->playerStateMachine, getPlayerStateMachine()->OnAir);
		CheckMoveRightInput(getPlayerStateMachine()->playerStateMachine);
		CheckMoveLeftInput(getPlayerStateMachine()->playerStateMachine);
	}

	//---------------------------------------------------------------------------/
	// PROPULSED STATE														 /
	//---------------------------------------------------------------------------/

	void Propulsed_State::Enter()
	{
		if (getPlayerStateMachine()->Player->m_transform->mScale.x > 0)
		{
			getPlayerStateMachine()->Player->m_transform->mScale = glm::vec2{ 100.0f, 100.0f };
			objectManager.FindObjectByName("PlayerHead")->m_transform->mScale = glm::vec2{ 100.0f, 100.0f };
		}
		else
		{
			getPlayerStateMachine()->Player->m_transform->mScale = glm::vec2{ -100.0f, 100.0f };
			objectManager.FindObjectByName("PlayerHead")->m_transform->mScale = glm::vec2{ -100.0f, 100.0f };
		}

		PlayerRigidBody->mVelocity = { 0.0f, 0.0f };

		PropulsionVector = objectManager.FindObjectByName("Tongue")->FindComponentByType<TongueComponent>()->GetDirVec();
	}

	void Propulsed_State::initialize()
	{
		this->mName = "Propulsed_State";


		PlayerRigidBody = getPlayerStateMachine()->Player->FindComponentByType<RigidBody>();
		AnimationComp = getPlayerStateMachine()->Player->FindComponentByType<Graphics::AnimComp>();

		using namespace Messaging;
		Messaging::EventHandler* EventHandler_ = &mOwnerStateMachine->GetOwner()->GetEventHandler();

		////register object and function
		EventHandler_->register_handler(this, &Propulsed_State::collisionStarted);
		EventHandler_->register_handler(this, &Propulsed_State::collisionPersisted);
		EventHandler_->register_handler(this, &Propulsed_State::collisionEnded);

		////subscribe this handler to this type of event
		Messaging::EventDispatcher::get_instance().subscribe(*EventHandler_, type_of<OnCollisionStarted>());
		Messaging::EventDispatcher::get_instance().subscribe(*EventHandler_, type_of<OnCollisionPersisted>());
		Messaging::EventDispatcher::get_instance().subscribe(*EventHandler_, type_of<OnCollisionEnded>());
	}

	void Propulsed_State::Exit()
	{}

	void Propulsed_State::Update()
	{
		getPlayerStateMachine()->UpdatePlayerOrientation(AnimationComp);

		//When tongue is destroyed quit from this state (go to idle)
		if (!objectManager.FindObjectByName("Tongue"))
		{
			mOwnerStateMachine->ChangeState<Falling_Jump_State>();
		}
		else
		{
			//Move in the direction vector towards the propulsion obj
			PlayerRigidBody->mVelocity = PropulsionVector * 2100.0f/*Propulsion speed*/;
		}
	}

	void Propulsed_State::collisionStarted(const OnCollisionStarted&)
	{
	}

	void Propulsed_State::collisionPersisted(const OnCollisionPersisted&)
	{}

	void Propulsed_State::collisionEnded(const OnCollisionEnded& mEvent)
	{
		if (mEvent.mData.obj1->getCollisionGroup() == CollisionGroup::GetCollisionGroup("Floor")
			|| mEvent.mData.obj2->getCollisionGroup() == CollisionGroup::GetCollisionGroup("Floor")
			|| mEvent.mData.obj1->getCollisionGroup() == CollisionGroup::GetCollisionGroup("ClimbableObj")
			|| mEvent.mData.obj2->getCollisionGroup() == CollisionGroup::GetCollisionGroup("ClimbableObj"))
		{
			getPlayerStateMachine()->OnAir = true;
		}
	}

	//---------------------------------------------------------------------------/
	// HORIZONTAL CLIMB STATE														 /
	//---------------------------------------------------------------------------/
	void HorizontalClimb_State::Enter()
	{
		//if (getPlayerStateMachine()->mPrevState->mName == "ThrowTongue_State")
		//{
		//	std::cout << "Prevoius works" << std::endl;
		//	getPlayerStateMachine()->Player->m_transform->mPosition.y += 0.5f;
		//}

		if (getPlayerStateMachine()->Player->m_transform->mScale.x > 0)
		{
			getPlayerStateMachine()->Player->m_transform->mScale = glm::vec2{ 130.0f, 130.0f };
			objectManager.FindObjectByName("PlayerHead")->m_transform->mScale = glm::vec2{ 130.0f, 130.0f };
		}
		else
		{
			getPlayerStateMachine()->Player->m_transform->mScale = glm::vec2{ -130.0f, 130.0f };
			objectManager.FindObjectByName("PlayerHead")->m_transform->mScale = glm::vec2{ -130.0f, 130.0f };
		}



		PlayerRigidBody->mVelocity = { 0.0f, 0.0f };
		PlayerRigidBody->mAffectedByGravity = false;
		PlayerRigidBody->mAcceleration = { 0.0f, 0.0f };

		getPlayerStateMachine()->IsClimbing_H = true;

		getPlayerStateMachine()->OnAir = true;
	}

	void HorizontalClimb_State::initialize()
	{
		this->mName = "HorizontalClimb_State";


		AnimationComp = getPlayerStateMachine()->Player->FindComponentByType<Graphics::AnimComp>();
		PlayerRigidBody = getPlayerStateMachine()->Player->FindComponentByType<RigidBody>();

		using namespace Messaging;
		Messaging::EventHandler* EventHandler_ = &mOwnerStateMachine->GetOwner()->GetEventHandler();

		////register object and function
		EventHandler_->register_handler(this, &HorizontalClimb_State::collisionStarted);
		EventHandler_->register_handler(this, &HorizontalClimb_State::collisionPersisted);
		EventHandler_->register_handler(this, &HorizontalClimb_State::collisionEnded);

		////subscribe this handler to this type of event
		Messaging::EventDispatcher::get_instance().subscribe(*EventHandler_, type_of<OnCollisionStarted>());
		Messaging::EventDispatcher::get_instance().subscribe(*EventHandler_, type_of<OnCollisionPersisted>());
		Messaging::EventDispatcher::get_instance().subscribe(*EventHandler_, type_of<OnCollisionEnded>());
	}

	void HorizontalClimb_State::Exit()
	{
		if (getPlayerStateMachine()->mNextState->mName == "ThrowTongue_State")
		{
			//PlayerRigidBody->mVelocity = { 0.0f, 20.0f };
			PlayerRigidBody->mAffectedByGravity = false;
			//PlayerRigidBody->mAcceleration = { 0.0f, 0.0f };
			//getPlayerStateMachine()->Player->FindComponentByType<RigidBody>()->mGravity.y = -getPlayerStateMachine()->Player->FindComponentByType<RigidBody>()->mGravity.y;
		}
		else
		{
			//if (getPlayerStateMachine()->Player->FindComponentByType<RigidBody>()->mGravity.y < 0.0f)
			//{
			//	getPlayerStateMachine()->Player->FindComponentByType<RigidBody>()->mGravity.y = abs(getPlayerStateMachine()->Player->FindComponentByType<RigidBody>()->mGravity.y);
			//}

			PlayerRigidBody->mAffectedByGravity = true;
		}


		if (mOwnerStateMachine->mCurrentState == mOwnerStateMachine->GetState<ThrowTongue_State>())
		{
			getPlayerStateMachine()->IsClimbing_H = true;
		}
		else
			getPlayerStateMachine()->IsClimbing_H = false;

		getPlayerStateMachine()->OnAir = true;
	}

	void HorizontalClimb_State::Update()
	{
		getPlayerStateMachine()->UpdatePlayerOrientation(AnimationComp);

		CheckMouseInputAndCreateTongue(getPlayerStateMachine()->Player, getPlayerStateMachine()->Tongue, getPlayerStateMachine()->playerStateMachine);


		if (WallClimbing->m_transform->mPosition.x + (WallClimbing->m_transform->mScale.x / 2.0f) < (getPlayerStateMachine()->Player->m_transform->mPosition.x + getPlayerStateMachine()->Player->m_transform->GetScale().x / 4.0f))
		{
			DontMoveRight = true;
		}
		else
		{
			DontMoveRight = false;
		}

		if (WallClimbing->m_transform->mPosition.x - (WallClimbing->m_transform->mScale.x / 2.0f) > (getPlayerStateMachine()->Player->m_transform->mPosition.x + getPlayerStateMachine()->Player->m_transform->GetScale().x / 4.0f))
		{
			DontMoveLeft = true;
		}
		else
		{
			DontMoveLeft = false;
		}


		if (Input::Instance()->KeyPressed('A') && DontMoveLeft == false)
		{
			//	if (AnimationComp->flipX) //Looking right
			//	{
			//		AnimationComp->FlipAnimation();
			//	}

			if (AnimationComp->mbAnimPlaying == false) //Looking right
			{
				AnimationComp->PlayAnimation();
			}

			getPlayerStateMachine()->Player->m_transform->IncreasePosBy({ -4.0f, 0.0f });
		}
		else if (Input::Instance()->KeyPressed('D') && DontMoveRight == false)
		{
			//	if (!AnimationComp->flipX) //Looking left
			//	{
			//		AnimationComp->FlipAnimation();
			//	}

			if (AnimationComp->mbAnimPlaying == false) //Looking right
			{
				AnimationComp->PlayAnimation();
			}

			getPlayerStateMachine()->Player->m_transform->IncreasePosBy({ 4.0f, 0.0f });
		}
		else if (Input::Instance()->KeyPressed('S'))
		{
			mOwnerStateMachine->ChangeState<Falling_Jump_State>();
		}
		else
		{
			if (AnimationComp->mbAnimPlaying) //Looking right
			{
				AnimationComp->PauseAnimation();
			}
		}
	}

	void HorizontalClimb_State::collisionStarted(const OnCollisionStarted&)
	{}

	void HorizontalClimb_State::collisionPersisted(const OnCollisionPersisted&)
	{}

	void HorizontalClimb_State::collisionEnded(const OnCollisionEnded& mEvent)
	{
		if (mOwnerStateMachine->mCurrentState == mOwnerStateMachine->GetState<HorizontalClimb_State>())
		{
			if (mEvent.mData.obj1->getCollisionGroup() == CollisionGroup::GetCollisionGroup("ClimbableObj") || mEvent.mData.obj2->getCollisionGroup() == CollisionGroup::GetCollisionGroup("ClimbableObj"))
			{
				if (mOwnerStateMachine->mCurrentState != mOwnerStateMachine->GetState<Propulsed_State>() && mOwnerStateMachine->mCurrentState != mOwnerStateMachine->GetState<HorizontalClimb_State>())
					mOwnerStateMachine->ChangeState<Falling_Jump_State>();
			}
		}
	}


	//---------------------------------------------------------------------------/
	// VERTICAL CLIMB STATE														 /
	//---------------------------------------------------------------------------/
	void VerticalClimb_State::Enter()
	{
		PlayerRigidBody->mVelocity = { 0.0f, 0.0f };
		PlayerRigidBody->mAffectedByGravity = false;
		PlayerRigidBody->mAcceleration = { 0.0f, 0.0f };

		getPlayerStateMachine()->IsClimbing_V = true;
		getPlayerStateMachine()->OnAir = true;

		if (WallClimbing == nullptr)
		{
			return;
		}
	}

	void VerticalClimb_State::initialize()
	{
		this->mName = "VerticalClimb_State";


		AnimationComp = getPlayerStateMachine()->Player->FindComponentByType<Graphics::AnimComp>();
		PlayerRigidBody = getPlayerStateMachine()->Player->FindComponentByType<RigidBody>();

		using namespace Messaging;
		Messaging::EventHandler* EventHandler_ = &mOwnerStateMachine->GetOwner()->GetEventHandler();

		////register object and function
		EventHandler_->register_handler(this, &VerticalClimb_State::collisionStarted);
		EventHandler_->register_handler(this, &VerticalClimb_State::collisionPersisted);
		EventHandler_->register_handler(this, &VerticalClimb_State::collisionEnded);

		////subscribe this handler to this type of event
		Messaging::EventDispatcher::get_instance().subscribe(*EventHandler_, type_of<OnCollisionStarted>());
		Messaging::EventDispatcher::get_instance().subscribe(*EventHandler_, type_of<OnCollisionPersisted>());
		Messaging::EventDispatcher::get_instance().subscribe(*EventHandler_, type_of<OnCollisionEnded>());
	}

	void VerticalClimb_State::Exit()
	{
		if (getPlayerStateMachine()->mNextState->mName == "ThrowTongue_State")
			PlayerRigidBody->mAffectedByGravity = false;
		else
			PlayerRigidBody->mAffectedByGravity = true;

		if (mOwnerStateMachine->mNextState == mOwnerStateMachine->GetState<ThrowTongue_State>())
		{
			getPlayerStateMachine()->IsClimbing_V = true;
		}
		else
			getPlayerStateMachine()->IsClimbing_V = false;

		getPlayerStateMachine()->OnAir = true;
	}

	void VerticalClimb_State::Update()
	{
		//getPlayerStateMachine()->UpdatePlayerOrientation(AnimationComp);

		CheckMouseInputAndCreateTongue(getPlayerStateMachine()->Player, getPlayerStateMachine()->Tongue, getPlayerStateMachine()->playerStateMachine);


		if (WallClimbing->m_transform->mPosition.y + (WallClimbing->m_transform->mScale.y / 2.0f) < (getPlayerStateMachine()->Player->m_transform->mPosition.y + getPlayerStateMachine()->Player->m_transform->GetScale().y / 4.0f))
		{
			DontMoveUp = true;
		}
		else
		{
			DontMoveUp = false;
		}

		if (WallClimbing->m_transform->mPosition.y - (WallClimbing->m_transform->mScale.y / 2.0f) > (getPlayerStateMachine()->Player->m_transform->mPosition.y - getPlayerStateMachine()->Player->m_transform->GetScale().y / 4.0f))
		{
			DontMoveDown = true;
		}
		else
		{
			DontMoveDown = false;
		}


		if (getPlayerStateMachine()->Player->m_transform->mPosition.x - WallClimbing->m_transform->mPosition.x < 0.0f)
		{
			if (AnimationComp->flipX == Left)
			{
				AnimationComp->FlipAnimation();
			}
		}
		else
		{
			if (AnimationComp->flipX == Right)
			{
				AnimationComp->FlipAnimation();
			}
		}

		if (Input::Instance()->KeyPressed('W') && DontMoveUp == false)
		{
			if (AnimationComp->mbAnimPlaying == false) //Looking right
			{
				AnimationComp->PlayAnimation();
			}

			getPlayerStateMachine()->Player->m_transform->IncreasePosBy({ 0.0f, 4.0f });
		}
		else if (Input::Instance()->KeyPressed('S') && DontMoveDown == false)
		{
			if (AnimationComp->mbAnimPlaying == false) //Looking right
			{
				AnimationComp->PlayAnimation();
			}

			getPlayerStateMachine()->Player->m_transform->IncreasePosBy({ 0.0f, -4.0f });
		}
		else if (Input::Instance()->KeyPressed('A') && (getPlayerStateMachine()->Player->m_transform->mPosition.x < WallClimbing->m_transform->mPosition.x))
		{
			mOwnerStateMachine->ChangeState<Falling_Jump_State>();
		}
		else if (Input::Instance()->KeyPressed('D') && (getPlayerStateMachine()->Player->m_transform->mPosition.x > WallClimbing->m_transform->mPosition.x))
		{
			mOwnerStateMachine->ChangeState<Falling_Jump_State>();
		}
		else
		{
			if (AnimationComp->mbAnimPlaying) //Looking right
			{
				AnimationComp->PauseAnimation();
			}
		}
	}

	void VerticalClimb_State::collisionStarted(const OnCollisionStarted&)
	{}

	void VerticalClimb_State::collisionPersisted(const OnCollisionPersisted&)
	{}

	void VerticalClimb_State::collisionEnded(const OnCollisionEnded& mEvent)
	{
		if (mOwnerStateMachine->mCurrentState == mOwnerStateMachine->GetState<VerticalClimb_State>())
		{
			if (mEvent.mData.obj1->getCollisionGroup() == CollisionGroup::GetCollisionGroup("ClimbableObj") || mEvent.mData.obj2->getCollisionGroup() == CollisionGroup::GetCollisionGroup("ClimbableObj"))
			{
				if (mOwnerStateMachine->mCurrentState != mOwnerStateMachine->GetState<Propulsed_State>())
					mOwnerStateMachine->ChangeState<Falling_Jump_State>();
			}
		}
	}

	//---------------------------------------------------------------------------/
	// Z-Climbing STATE														 /
	//---------------------------------------------------------------------------/
	void ZClimb_State::Enter()
	{
		PlayerRigidBody->mVelocity = { 0.0f, 0.0f };
		PlayerRigidBody->mAffectedByGravity = false;
		PlayerRigidBody->mAcceleration = { 0.0f, 0.0f };

		//getPlayerStateMachine()->Player->m_transform->mScale.y += 50.0f;
		//Player->m_transform->mScale.x += 30.0f;
		//getPlayerStateMachine()->OnAir = false;
	}

	void ZClimb_State::initialize()
	{
		this->mName = "ZClimb_State";


		AnimationComp = getPlayerStateMachine()->Player->FindComponentByType<Graphics::AnimComp>();
		PlayerRigidBody = getPlayerStateMachine()->Player->FindComponentByType<RigidBody>();

		using namespace Messaging;
		Messaging::EventHandler* EventHandler_ = &mOwnerStateMachine->GetOwner()->GetEventHandler();

		////register object and function
		EventHandler_->register_handler(this, &ZClimb_State::collisionStarted);
		EventHandler_->register_handler(this, &ZClimb_State::collisionPersisted);
		EventHandler_->register_handler(this, &ZClimb_State::collisionEnded);

		////subscribe this handler to this type of event
		Messaging::EventDispatcher::get_instance().subscribe(*EventHandler_, type_of<OnCollisionStarted>());
		Messaging::EventDispatcher::get_instance().subscribe(*EventHandler_, type_of<OnCollisionPersisted>());
		Messaging::EventDispatcher::get_instance().subscribe(*EventHandler_, type_of<OnCollisionEnded>());
	}

	void ZClimb_State::Exit()
	{
		PlayerRigidBody->mAffectedByGravity = true;
		//getPlayerStateMachine()->Player->m_transform->mScale.y -= 50.0f;
		//Player->m_transform->mScale.x -= 30.0f;
		//getPlayerStateMachine()->OnAir = true;
	}

	void ZClimb_State::Update()
	{
		getPlayerStateMachine()->UpdatePlayerOrientation(AnimationComp);

		if (Input::Instance()->KeyPressed('A'))
		{
			if (AnimationComp->mbAnimPlaying == false) //Looking right
			{
				AnimationComp->PlayAnimation();
			}

			getPlayerStateMachine()->Player->m_transform->IncreasePosBy({ -5.0f, 0.0f });
		}
		else if (Input::Instance()->KeyPressed('D'))
		{
			if (AnimationComp->mbAnimPlaying == false) //Looking right
			{
				AnimationComp->PlayAnimation();
			}

			getPlayerStateMachine()->Player->m_transform->IncreasePosBy({ 5.0f, 0.0f });
		}
		else if (Input::Instance()->KeyPressed('W'))
		{
			if (AnimationComp->mbAnimPlaying == false) //Looking right
			{
				AnimationComp->PlayAnimation();
			}

			getPlayerStateMachine()->Player->m_transform->IncreasePosBy({ 0.0f, 5.0f });
		}
		else if (Input::Instance()->KeyPressed('S'))
		{
			if (AnimationComp->mbAnimPlaying == false) //Looking right
			{
				AnimationComp->PlayAnimation();
			}

			getPlayerStateMachine()->Player->m_transform->IncreasePosBy({ 0.0f, -5.0f });
		}
		else
		{
			if (AnimationComp->mbAnimPlaying) //Looking right
			{
				AnimationComp->PauseAnimation();
			}
		}
	}

	void ZClimb_State::collisionStarted(const OnCollisionStarted&)
	{

	}

	void ZClimb_State::collisionPersisted(const OnCollisionPersisted&)
	{
	}

	void ZClimb_State::collisionEnded(const OnCollisionEnded&)
	{}

	//Helper Functions
	void ApplyDrag(float MaxVelocity, GameObject * Player, float drag_acceleration)
	{
		if (Player == nullptr)
		{
			return;
		}

		auto PlayerRigidBody = Player->FindComponentByType<RigidBody>();

		if (PlayerRigidBody != nullptr)
		{
			if (PlayerRigidBody->mVelocity.x > 0.0f)	//Player is moving to the right
			{
				PlayerRigidBody->mVelocity.x -= drag_acceleration;
			}
			if (PlayerRigidBody->mVelocity.x < 0.0f)	//Player is moving to the left
			{
				PlayerRigidBody->mVelocity.x += drag_acceleration;
			}
			if (abs(PlayerRigidBody->mVelocity.x) < MaxVelocity)
			{
				PlayerRigidBody->mVelocity.x = 0.0f;
			}
		}
	}
	void LimitVelocity(GameObject * Player, float max_velocity)
	{
		if (Player == nullptr)
			return;

		auto PlayerRigidBody = Player->FindComponentByType<RigidBody>();

		if (PlayerRigidBody != nullptr)
		{
			if (PlayerRigidBody->mVelocity.x > max_velocity)
			{
				PlayerRigidBody->mVelocity.x = max_velocity;
			}
			if (PlayerRigidBody->mVelocity.x < -max_velocity)
			{
				PlayerRigidBody->mVelocity.x = -max_velocity;
			}
		}
	}
	void CheckJumpInput(StateMachine * playerStateMachine, bool OnAir)
	{
		if (Input::Instance()->KeyPressed('W') && OnAir == false)
		{
			playerStateMachine->ChangeState<Elevating_Jump_State>();
		}
	}
	void CheckMoveRightInput(StateMachine * playerStateMachine)
	{
		if (Input::Instance()->KeyPressed('D'))
		{
			playerStateMachine->ChangeState<MoveRight_State>();
		}
	}
	void CheckMoveLeftInput(StateMachine * playerStateMachine)
	{
		if (Input::Instance()->KeyPressed('A'))
		{
			playerStateMachine->ChangeState<MoveLeft_State>();
		}
	}
	GameObject * CamoflageFly = nullptr;
	glm::vec2 DirectionVector;
	float counter = 0.0f;
	float maxtime = 0.2f;
	float TongueThrowCounter = 5.0f;
	bool enablethrow = true;
	void CheckMouseInputAndCreateTongue(GameObject * Player, GameObject * Tongue, StateMachine * playerStateMachine)
	{

		if (PlayerStateMachine::teleportMode)
		{
			if (Input::Instance()->MouseTriggered(Input::eMouseRight))
			{
				if (Input::Instance()->KeyPressed(Input::Keys::eControl))
					objectManager.FindObjectByName("player")->m_transform->SetPos(Input::Instance()->ScreenToWorld());
			}
		}


		if (CamoflageFly != nullptr)
		{
			if (counter < maxtime)
			{
				CamoflageFly->m_transform->mPosition += DirectionVector * 20.0f;
				counter += 0.016f;
			}
			else
			{
				CamoflageFly = nullptr;
			}
		}
		else
		{
			counter = 0.0f;
		}

		if (Input::Instance()->MouseTriggered(Input::eMouseLeft))
		{
			if (Player->FindComponentByType<CamouflageComponent>()->IsVisible() == false)
			{
				CamoflageFly = CreateCamouflageFly(Player);
				Player->FindComponentByType<CamouflageComponent>()->ReleaseFly();

				auto PlayerHead = Player->FindComponentByType<HeadComponent>()->GetOwner();

				glm::vec2 mouseWorldPosition = Input::Instance()->ScreenToWorld();	//Position of mouse

																					//Compute the vector between player and mouse
				DirectionVector = glm::normalize(mouseWorldPosition - PlayerHead->m_transform->GetPos());


				return;
			}

			playerStateMachine->ChangeState<ThrowTongue_State>();

			if (!objectManager.FindObjectByName("Tongue") && enablethrow)
			{
				auto PlayerHeadComp = Player->FindComponentByType<HeadComponent>();

				CreateTongue(Tongue);
				PlayerHeadComp->ReadyToRotHead = true;
				PlayerHeadComp->UpdateMouse = true;
				TongueThrowCounter = 0.0f;
				enablethrow = false;
			}
		}

		if (enablethrow == false)
		{
			if (TongueThrowCounter >= 0.35f)
			{
				enablethrow = true;
			}
			else
				TongueThrowCounter += 0.016f;
		}

	}
	void CreateTongue(GameObject * Tongue)
	{
		Tongue = objectManager.AddGameObject("Tongue");
		Tongue->EditableByGizmos = false;
		Tongue->m_transform->SetScale(glm::vec2(60.0f, 60.0f));

		//auto* rend = Tongue->AddComponent<Graphics::Renderable>();
		////rend->pTransform->SetPos(Position);
		//rend->pTexture = ResourceManager::textures["Default"].get();


		auto * TongueRigidBody = Tongue->AddComponent<RigidBody>();
		TongueRigidBody->setAffectedByGravity(false);
		TongueRigidBody->isStatic = false;
		TongueRigidBody->mInvMass = 0.0f;

		auto* TongueCollider = Tongue->AddComponent<Collider>();
		TongueCollider->isGhost = true;
		TongueCollider->addCollisionGroup(CollisionGroup::GetCollisionGroup("TongueCollisionGroup"));

		Tongue->AddComponent<TongueComponent>();
	}

	GameObject * CreateCamouflageFly(GameObject * Player)
	{
		//Find the archetype of the camouflage fly
		auto archetype = ResourceManager::archetypes.find("CamoulageFly.archetype");
		auto * gameObject = objectManager.AddGameObject(archetype->first, false);
		gameObject->from_json(archetype->second);

		auto PlayerHead = Player->FindComponentByType<HeadComponent>()->GetOwner();

		//Move it to the position of the object
		gameObject->m_transform->mPosition = PlayerHead->m_transform->mPosition;

		gameObject->m_transform->mPosition += 30.0f;

		//Move the fly towards the mouse so that it doesn't spawn on the position of the player
		//gameObject->m_transform->mPosition += DirectionVector * 150.0f;

		return gameObject;
	}

	PlayerStateMachine * PlayerState::getPlayerStateMachine()
	{
		if (PSM == nullptr && mOwnerStateMachine != nullptr)
		{
			PSM = static_cast<PlayerStateMachine*>(this->mOwnerStateMachine);
		}
		return PSM;
	}
}

PlayerStateMachine::PlayerStateMachine()
{
	m_name = "player state machine";

	playerStateMachine = nullptr;
}

PlayerStateMachine::~PlayerStateMachine()
{
	PlayerSM::PlayerState::PSM = nullptr;
}

void PlayerStateMachine::Initialize()
{
	Checkpoint::respawnPosition = InitialPosition;

	m_GameObject->m_transform->mPosition = InitialPosition;

	playerStateMachine = this;
	//Add states to the Player:
	AddState<PlayerSM::Idle_State>();
	AddState<PlayerSM::Elevating_Jump_State>();
	AddState<PlayerSM::Falling_Jump_State>();
	AddState<PlayerSM::Landing_Jump_State>();
	AddState<PlayerSM::MoveRight_State>();
	AddState<PlayerSM::MoveLeft_State>();
	AddState<PlayerSM::ThrowTongue_State>();
	AddState<PlayerSM::Propulsed_State>();
	AddState<PlayerSM::HorizontalClimb_State>();
	AddState<PlayerSM::VerticalClimb_State>();
	AddState<PlayerSM::ZClimb_State>();
	SetInitState<PlayerSM::Idle_State>();	//Initial State


											//Elevating_Jump_State
	auto Elevating_Jump_State = GetState<PlayerSM::Elevating_Jump_State>();
	Elevating_Jump_State->CompColumns = 5;
	Elevating_Jump_State->CompRows = 1;
	Elevating_Jump_State->CompSprites = 5;
	Elevating_Jump_State->CompDuration = 1;
	Elevating_Jump_State->CompDelays.resize(5);
	Elevating_Jump_State->loop = true;
	Elevating_Jump_State->StateTexture = ResourceManager::textures["Body anticipation walkidle sprite sheet.png"].get();

	Elevating_Jump_State->CompDelays[0] = 0.084f;
	Elevating_Jump_State->CompDelays[2] = 0.042f;
	Elevating_Jump_State->CompDelays[1] = 0.126f;
	Elevating_Jump_State->CompDelays[3] = 999.0f;


	//Falling_Jump_State
	auto Falling_Jump_State = GetState<PlayerSM::Falling_Jump_State>();
	Falling_Jump_State->CompColumns = 5;
	Falling_Jump_State->CompRows = 1;
	Falling_Jump_State->CompSprites = 5;
	Falling_Jump_State->CompDuration = 1;
	Falling_Jump_State->CompDelays.resize(5);
	Falling_Jump_State->loop = true;
	Falling_Jump_State->StateTexture = ResourceManager::textures["Body falling loop sprite sheet.png"].get();

	for (auto& delay : Falling_Jump_State->CompDelays)
		delay = 0.08333f;


	//Landing_Jump_State
	auto Landing_Jump_State = GetState<PlayerSM::Landing_Jump_State>();
	Landing_Jump_State->CompColumns = 3;
	Landing_Jump_State->CompRows = 1;
	Landing_Jump_State->CompSprites = 3;
	Landing_Jump_State->CompDuration = 1;
	Landing_Jump_State->CompDelays.resize(3);
	Landing_Jump_State->loop = true;
	Landing_Jump_State->StateTexture = ResourceManager::textures["Body landing sprite sheet.png"].get();

	Landing_Jump_State->CompDelays[0] = 0.042f;
	Landing_Jump_State->CompDelays[1] = 999.0f;


	//ThrowTongue_State
	auto ThrowTongueState = GetState<PlayerSM::ThrowTongue_State>();
	ThrowTongueState->CompColumns = 5;
	ThrowTongueState->CompRows = 2;
	ThrowTongueState->CompSprites = 10;
	ThrowTongueState->CompDuration = 1;
	ThrowTongueState->loop = true;
	ThrowTongueState->StateTexture = ResourceManager::textures["fake.png"].get();

	for (auto& delay : GetState<PlayerSM::ThrowTongue_State>()->CompDelays)
		delay = 0.08333f;


	//Propulsed_State
	auto PupulsedState = GetState<PlayerSM::Propulsed_State>();
	PupulsedState->CompColumns = 17;
	PupulsedState->CompRows = 1;
	PupulsedState->CompSprites = 17;
	PupulsedState->CompDuration = 1;
	PupulsedState->CompDelays.resize(17);
	PupulsedState->mAnimSpeed = 1.0f;
	PupulsedState->loop = true;
	PupulsedState->StateTexture = ResourceManager::textures["idle prite sheet.png"].get();


	//HorizontalClimb_State
	auto HorizontalCimbState = GetState<PlayerSM::HorizontalClimb_State>();
	HorizontalCimbState->CompColumns = 10;
	HorizontalCimbState->CompRows = 1;
	HorizontalCimbState->CompDuration = 1;
	HorizontalCimbState->CompSprites = 10;
	HorizontalCimbState->CompDelays.resize(10);
	HorizontalCimbState->loop = true;
	HorizontalCimbState->StateTexture = ResourceManager::textures["Body sprite sheet.png"].get();

	for (auto& delay : GetState<PlayerSM::HorizontalClimb_State>()->CompDelays)
		delay = 0.084f;


	//VerticalClimb_State
	auto VerticalClimbState = GetState<PlayerSM::VerticalClimb_State>();
	VerticalClimbState->CompColumns = 12;
	VerticalClimbState->CompRows = 1;
	VerticalClimbState->CompDuration = 1;
	VerticalClimbState->CompSprites = 12;
	VerticalClimbState->CompDelays.resize(12);
	VerticalClimbState->loop = true;
	VerticalClimbState->StateTexture = ResourceManager::textures["stick paredcuerpo.png"].get();

	VerticalClimbState->CompDelays[0] = VerticalClimbState->CompDelays[5] = VerticalClimbState->CompDelays[6] = VerticalClimbState->CompDelays[11] = 0.042f;
	VerticalClimbState->CompDelays[1] = VerticalClimbState->CompDelays[2] = VerticalClimbState->CompDelays[3] = VerticalClimbState->CompDelays[4] = VerticalClimbState->CompDelays[7] = VerticalClimbState->CompDelays[8] = VerticalClimbState->CompDelays[9] = VerticalClimbState->CompDelays[10] = 0.084f;


	//ZClimb_State
	auto ZClimbState = GetState<PlayerSM::ZClimb_State>();
	ZClimbState->CompColumns = 10;
	ZClimbState->CompRows = 1;
	ZClimbState->CompSprites = 10;
	ZClimbState->CompDuration = 1;
	ZClimbState->CompDelays.resize(10);
	ZClimbState->loop = true;
	ZClimbState->StateTexture = ResourceManager::textures["Dibo body escalar sprite sheet.png"].get();

	ZClimbState->CompDelays[0] = ZClimbState->CompDelays[1] = ZClimbState->CompDelays[2] = ZClimbState->CompDelays[4] = ZClimbState->CompDelays[5] = ZClimbState->CompDelays[6] = ZClimbState->CompDelays[7] = ZClimbState->CompDelays[9] = 0.084f;
	ZClimbState->CompDelays[3] = ZClimbState->CompDelays[8] = 0.042f;


	//MoveLeft_State
	auto MoveLeftState = GetState<PlayerSM::MoveLeft_State>();
	MoveLeftState->CompColumns = 10;
	MoveLeftState->CompRows = 1;
	MoveLeftState->CompSprites = 10;
	MoveLeftState->CompDuration = 1;
	MoveLeftState->CompDelays.resize(10);
	//MoveLeftState->mAnimSpeed = 0.5f;
	MoveLeftState->loop = true;
	MoveLeftState->StateTexture = ResourceManager::textures["Dibo-Walk_SpriteSheet_Body.png"].get();

	for (auto& delay : MoveLeftState->CompDelays)
		delay = 0.084f;


	//MoveRight_State
	auto MoveRightState = GetState<PlayerSM::MoveRight_State>();
	MoveRightState->CompColumns = 10;
	MoveRightState->CompRows = 1;
	MoveRightState->CompSprites = 10;
	MoveRightState->CompDuration = 1;
	MoveRightState->CompDelays.resize(10);
	//MoveRightState->mAnimSpeed = 0.5f;
	MoveRightState->loop = true;
	MoveRightState->StateTexture = ResourceManager::textures["Dibo-Walk_SpriteSheet_Body.png"].get();

	for (auto& delay : MoveRightState->CompDelays)
		delay = 0.084f;


	//Idle_State
	auto IdleState = GetState<PlayerSM::Idle_State>();
	IdleState->CompColumns = 17;
	IdleState->CompRows = 1;
	IdleState->CompSprites = 17;
	IdleState->CompDuration = 1;
	IdleState->CompDelays.resize(17);
	IdleState->loop = true;
	IdleState->StateTexture = ResourceManager::textures["idle prite sheet.png"].get();

	for (auto& delay : IdleState->CompDelays)
		delay = 0.042f;


	SetInitState<PlayerSM::Idle_State>();
}

void PlayerStateMachine::UpdatePlayerOrientation(Graphics::AnimComp * AnimationComp)
{
	PreviousPos = CurrentPos;
	CurrentPos = Player->m_transform->mPosition.x;

	if (PreviousPos == CurrentPos) //Don't change orientation if player doesn't move
	{
		return;
	}

	if (CurrentPos < PreviousPos)
	{
		if (AnimationComp->flipX != Left)
		{
			AnimationComp->FlipAnimation();
		}

		//std::cout << "Left" << std::endl;
		//AnimationComp->flipX = Left;
		//std::cout << "flipX: " << AnimationComp->flipX << std::endl;
	}
	else
	{
		if (AnimationComp->flipX != Right)
		{
			AnimationComp->FlipAnimation();
		}

		//std::cout << "Right" << std::endl;
		//AnimationComp->flipX = Right;
		//std::cout << "flipX: " << AnimationComp->flipX << std::endl;
	}
}