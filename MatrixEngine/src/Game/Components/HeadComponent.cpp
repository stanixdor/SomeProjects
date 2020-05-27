#include "HeadComponent.h"
#include "Engine\Physics\CollisionEvents\OnCollisionEvent.h"
#include "Physics\CollisionTable.h"
#include "Engine\Platform\EventSystem\event_dispatcher.h"
#include "GameObject\GameObject.h"
#include "Platform\Input\Input.h"
#include "Physics\RigidBody.h"
#include "Engine/Graphics/GraphicsSystem.h"
#include "Platform\Resource Manager\ResourceManager.h"
#include "Components/CamouflageComponent.h"
#include "Components/Tongue.h"
#include "Graphics/GraphicsComponents/AnimationComponent.h"
#include "Physics\Collider.h"
#include "Game\Logic\StateMachines\PlayerStateMachine.h"

HeadComponent::HeadComponent() : Component("HeadComponent")
{
	m_name = std::string("HeadComponent");
	PosOffset = { 0.0f, 40.0f };
	Angle = 0.0f;
	ClickPos = { 0,0 };
	Head = nullptr;
	OriginalPosOffset = PosOffset;
}

void HeadComponent::Update()
{
	if (Head == nullptr)
	{
		Head = objectManager.FindObjectByName("PlayerHead");

		if (Head != nullptr)
		{
			auto AnimComp = Head->FindComponentByType<Graphics::AnimComp>();
			AnimComp->flipX = true;

			ChangeAnimation(Animations::IDLE);

			CurrentStateName = "Idle_State";

			Head->m_transform->SetScale(Owner->m_transform->GetScale());
			Head->m_transform->mScale.x = abs(Head->m_transform->mScale.x); //Head Starts looking right
			Head->FindComponentByType<Graphics::AnimComp>()->flipX = Right;

			if (Head->FindComponentByType<Collider>())
			{
				Head->FindComponentByType<Collider>()->isGhost = true;
			}
			Head->m_transform->mZorder = Owner->m_transform->mZorder + 1;


		}
	}
	if (ThrowHead == nullptr)
	{
		GetThrowTongueHead();
		if (ThrowHead != nullptr)
			ThrowHead->m_transform->mZorder = 15.0f;
	}

	auto playerstate = Owner->FindComponentByType<PlayerStateMachine>();

	if (playerstate->mCurrentState != nullptr)	//Manage in which state is the player and change the animation of the head accordingly
	{
		if (playerstate->mCurrentState->mName != CurrentStateName)
		{
			AlreadyChangedAnim = false;
		}
		else
			AlreadyChangedAnim = true;

		if (playerstate->mCurrentState->mName == "Idle_State" && AlreadyChangedAnim == false)
		{
			ChangeAnimation(Animations::IDLE);
			CurrentStateName = "Idle_State";
		}
		if ((playerstate->mCurrentState->mName == "MoveLeft_State") && AlreadyChangedAnim == false)
		{
			ChangeAnimation(Animations::WALK);
			CurrentStateName = "MoveLeft_State";
		}
		if ((playerstate->mCurrentState->mName == "MoveRight_State") && AlreadyChangedAnim == false)
		{
			ChangeAnimation(Animations::WALK);
			CurrentStateName = "MoveRight_State";
		}
		if ((playerstate->mCurrentState->mName == "Elevating_Jump_State") && AlreadyChangedAnim == false)
		{
			ChangeAnimation(Animations::ELEVATIONJUMP);
			CurrentStateName = "Elevating_Jump_State";
		}
		if ((playerstate->mCurrentState->mName == "Falling_Jump_State") && AlreadyChangedAnim == false)
		{
			ChangeAnimation(Animations::FALLINGJUMP);
			CurrentStateName = "Falling_Jump_State";
		}
		if ((playerstate->mCurrentState->mName == "Landing_Jump_State") && AlreadyChangedAnim == false)
		{
			ChangeAnimation(Animations::LANDINGJUMP);
			CurrentStateName = "Landing_Jump_State";
		}
		if ((playerstate->mCurrentState->mName == "HorizontalClimb_State") && AlreadyChangedAnim == false)
		{
			ChangeAnimation(Animations::CLIMBH);
			CurrentStateName = "HorizontalClimb_State";
		}
		if ((playerstate->mCurrentState->mName == "VerticalClimb_State") && AlreadyChangedAnim == false)
		{
			ChangeAnimation(Animations::CLIMBV);
			CurrentStateName = "VerticalClimb_State";
		}
		if ((playerstate->mCurrentState->mName == "ZClimb_State") && AlreadyChangedAnim == false)
		{
			ChangeAnimation(Animations::CLIMBZ);
			CurrentStateName = "ZClimb_State";
		}
	}

	if (!objectManager.FindObjectByName("Tongue"))	//Flip the animation to match the player
	{
		if (Owner->FindComponentByType<Graphics::AnimComp>()->flipX != Head->FindComponentByType<Graphics::AnimComp>()->flipX)
		{
			Head->FindComponentByType<Graphics::AnimComp>()->FlipAnimation();
		}
	}


	Head->m_transform->mPosition = Owner->m_transform->GetPos();	//Update head position
	HeadWorldPos = Head->m_transform->mPosition + PosOffset;
	if (ThrowHead != nullptr)
		ThrowHead->m_transform->mPosition = HeadWorldPos;


	if (ThrowTongueHeadEnabled == false)
	{
		Head->FindComponentByType<AnimComp>()->mColor[3] = 1.0f;

		if (ThrowHead != nullptr && ThrowHead->FindComponentByType<AnimComp>() != nullptr)
		{
			ThrowHead->FindComponentByType<AnimComp>()->mColor[3] = 0.0f;
		}
	}
	else
	{
		Head->FindComponentByType<AnimComp>()->mColor[3] = 0.0f;
		ThrowHead->FindComponentByType<AnimComp>()->mColor[3] = 1.0f;
	}

	if (Owner->FindComponentByType<CamouflageComponent>()->IsVisible() == false)
	{
		Head->FindComponentByType<AnimComp>()->SetAlpha(Owner->FindComponentByType<AnimComp>()->mColor[3]);
	}

	if (objectManager.FindObjectByName("Tongue") && ThrowHead != nullptr)
	{
		EnableThrowTongueHead();

		if (objectManager.FindObjectByName("Tongue")->FindComponentByType<TongueComponent>())	//Assign to the angle of the tongue
		{
			ThrowHead->m_transform->mAngle = objectManager.FindObjectByName("Tongue")->FindComponentByType<TongueComponent>()->getangle();
		}
		invisiblewaitcounter = 0.0f;
	}
	else
	{
		if (ThrowTongueHeadEnabled == true)
		{
			DisableThrowTongueHead();
		}
		if (invisiblewaitcounter <= 0.2f)
		{
			invisiblewaitcounter += 0.016f;
		}
		else
		{
			if (ThrowHead != nullptr)
				ThrowHead->m_transform->mScale.x = abs(ThrowHead->m_transform->mScale.x);
		}
	}

	if (Head->FindComponentByType<AnimComp>()->mAnimCurrentFrame != Owner->FindComponentByType<AnimComp>()->mAnimCurrentFrame)
	{
		Head->FindComponentByType<AnimComp>()->mAnimCurrentFrame = Owner->FindComponentByType<AnimComp>()->mAnimCurrentFrame;
	}
}

void HeadComponent::LateUpdate()
{
	if (m_alive == false)
	{
		Owner = nullptr;
		Head = nullptr;
	}
}

void HeadComponent::Initialize()
{
	Owner = this->GetOwner();	//Save the owner of the component

	using namespace Messaging;
	Messaging::EventHandler* temp = &Owner->GetEventHandler();

	Owner->GetEventHandler().register_handler(this, &HeadComponent::collisionStarted);
	Owner->GetEventHandler().register_handler(this, &HeadComponent::collisionEnded);

	////subscribe this handler to this type of event
	Messaging::EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionStarted>());
	Messaging::EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionPersisted>());
	Messaging::EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionEnded>());
}

void HeadComponent::collisionStarted(const OnCollisionStarted&)
{}

void HeadComponent::collisionPersisted(const OnCollisionPersisted&)
{}

void HeadComponent::collisionEnded(const OnCollisionEnded&)
{}

void HeadComponent::ComputeVectorandAngle()
{
	glm::vec2 mouseWorldPosition = Input::Instance()->ScreenToWorld();	//Position of mouse

	ClickPos = mouseWorldPosition;

	//Compute the vector
	DirectionVector = glm::normalize(mouseWorldPosition - Head->m_transform->GetPos());

	//Compute height and width
	float h = mouseWorldPosition.y - Head->m_transform->GetPos().y;
	float w = mouseWorldPosition.x - Head->m_transform->GetPos().x;

	Angle = glm::atan(h / w);	//Compute angle
}
void HeadComponent::ComputeAngleWithClickPos()
{
	//Compute the vector
	DirectionVector = glm::normalize(ClickPos - Head->m_transform->GetPos());

	//Compute height and width
	float h = ClickPos.y - Head->m_transform->GetPos().y;
	float w = ClickPos.x - Head->m_transform->GetPos().x;

	Angle = glm::atan(h / w);	//Compute angle
}

void HeadComponent::ChangeAnimation(Animations Anim)	//Change the animation of the head
{
	switch (Anim)
	{
	case Animations::IDLE:
	{
		auto AnimComp = Head->FindComponentByType<Graphics::AnimComp>();

		AnimComp->ResetAnimation();
		AnimComp->CompColumns = 17;
		AnimComp->CompRows = 1;
		AnimComp->CompSprites = 17;
		AnimComp->CompDuration = 1;
		AnimComp->CompDelays.resize(17);
		AnimComp->SetLoop(true);
		AnimComp->pTexture = ResourceManager::textures["Idle head sprite sheet 2.png"].get();

		for (auto& delay : AnimComp->CompDelays)
			delay = 0.084f;

		if (PosOffset != OriginalPosOffset)
			PosOffset = OriginalPosOffset;
	}break;
	case Animations::WALK:
	{
		auto AnimComp = Head->FindComponentByType<Graphics::AnimComp>();

		AnimComp->ResetAnimation();
		AnimComp->CompColumns = 10;
		AnimComp->CompRows = 1;
		AnimComp->CompSprites = 10;
		AnimComp->CompDuration = 1;
		AnimComp->CompDelays.resize(10);
		AnimComp->SetLoop(true);
		AnimComp->pTexture = ResourceManager::textures["Dibo-Walk_SpriteSheet_Head.png"].get();

		for (auto& delay : AnimComp->CompDelays)
			delay = 0.084f;

		if (PosOffset != OriginalPosOffset)
			PosOffset = OriginalPosOffset;
	}break;
	case Animations::ELEVATIONJUMP:
	{
		auto AnimComp = Head->FindComponentByType<Graphics::AnimComp>();

		AnimComp->ResetAnimation();
		AnimComp->CompColumns = 5;
		AnimComp->CompRows = 1;
		AnimComp->CompSprites = 5;
		AnimComp->CompDuration = 1;
		AnimComp->CompDelays.resize(5);
		AnimComp->SetLoop(true);
		AnimComp->pTexture = ResourceManager::textures["Cleanup jump anticipation from floor head.png"].get();

		for (auto& delay : AnimComp->CompDelays)
			delay = 0.084f;

		if (PosOffset != OriginalPosOffset)
			PosOffset = OriginalPosOffset;
	}break;
	case Animations::FALLINGJUMP:
	{
		auto AnimComp = Head->FindComponentByType<Graphics::AnimComp>();

		AnimComp->ResetAnimation();
		AnimComp->CompColumns = 5;
		AnimComp->CompRows = 1;
		AnimComp->CompSprites = 5;
		AnimComp->CompDuration = 1;
		AnimComp->CompDelays.resize(5);
		AnimComp->SetLoop(true);
		AnimComp->pTexture = ResourceManager::textures["fallingloopheadSS.png"].get();

		for (auto& delay : AnimComp->CompDelays)
			delay = 0.084f;

		if (Head->FindComponentByType<Graphics::AnimComp>()->flipX == Right)
		{
			Head->m_transform->mScale.x = 60.0f;
		}
		else
		{
			Head->m_transform->mScale.x = -60.0f;
		}

		if (PosOffset != OriginalPosOffset)
			PosOffset = OriginalPosOffset;
	}break;
	case Animations::LANDINGJUMP:
	{
		auto AnimComp = Head->FindComponentByType<Graphics::AnimComp>();

		AnimComp->ResetAnimation();
		AnimComp->CompColumns = 3;
		AnimComp->CompRows = 1;
		AnimComp->CompSprites = 3;
		AnimComp->CompDuration = 1;
		AnimComp->CompDelays.resize(3);
		AnimComp->SetLoop(true);
		AnimComp->pTexture = ResourceManager::textures["Dibo jump landing head.png"].get();

		for (auto& delay : AnimComp->CompDelays)
			delay = 0.084f;

		if (Head->FindComponentByType<Graphics::AnimComp>()->flipX == Right)
		{
			Head->m_transform->mScale.x = 100.0f;
		}
		else
		{
			Head->m_transform->mScale.x = -100.0f;
		}

		if (PosOffset != OriginalPosOffset)
			PosOffset = OriginalPosOffset;

	}break;
	case Animations::CLIMBH:
	{
		auto AnimComp = Head->FindComponentByType<Graphics::AnimComp>();

		AnimComp->ResetAnimation();
		AnimComp->CompColumns = 10;
		AnimComp->CompRows = 1;
		AnimComp->CompSprites = 10;
		AnimComp->CompDuration = 1;
		AnimComp->CompDelays.resize(10);
		AnimComp->SetLoop(true);
		AnimComp->pTexture = ResourceManager::textures["Dibo stick head.png"].get();

		for (auto& delay : AnimComp->CompDelays)
			delay = 0.084f;

		PosOffset -= glm::vec2{ 0.0f, 90.0f };

	}break;
	case Animations::CLIMBV:
	{
		auto AnimComp = Head->FindComponentByType<Graphics::AnimComp>();

		AnimComp->ResetAnimation();
		AnimComp->CompColumns = 12;
		AnimComp->CompRows = 1;
		AnimComp->CompSprites = 12;
		AnimComp->CompDuration = 1;
		AnimComp->CompDelays.resize(12);
		AnimComp->SetLoop(true);
		AnimComp->pTexture = ResourceManager::textures["stick paredhead.png"].get();

		if (PosOffset != OriginalPosOffset)
			PosOffset = OriginalPosOffset;

	}break;
	case Animations::CLIMBZ:
	{
		auto AnimComp = Head->FindComponentByType<Graphics::AnimComp>();

		AnimComp->ResetAnimation();
		AnimComp->CompColumns = 10;
		AnimComp->CompRows = 1;
		AnimComp->CompSprites = 10;
		AnimComp->CompDuration = 1;
		AnimComp->CompDelays.resize(10);
		AnimComp->SetLoop(true);
		AnimComp->pTexture = ResourceManager::textures["Dibo head escalar sprite sheet.png"].get();

		for (auto& delay : AnimComp->CompDelays)
			delay = 0.084f;

		if (PosOffset != OriginalPosOffset)
			PosOffset = OriginalPosOffset;
	}break;
	}
}

void HeadComponent::GetThrowTongueHead()
{
	ThrowHead = objectManager.FindObjectByName("ThrowHead");
}

void HeadComponent::DisableThrowTongueHead()
{
	ThrowTongueHeadEnabled = false;
	//ThrowHead->FindComponentByType<Graphics::Renderable>()->SetVisible(ThrowTongueHeadEnabled);
}

void HeadComponent::EnableThrowTongueHead()
{
	ThrowTongueHeadEnabled = true;
	//ThrowHead->FindComponentByType<Graphics::Renderable>()->SetVisible(ThrowTongueHeadEnabled);
}