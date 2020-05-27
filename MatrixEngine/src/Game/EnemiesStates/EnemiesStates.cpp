//Mikel Fernandez

#include "EnemiesStates.h"
#include "Physics\RigidBody.h"
#include "Levels\LevelTest.h"
#include "GLM\gtx\vector_angle.hpp"

namespace EnemiesStates
{
	REFLECTION(SheepInfoComp, mDetectionDis, mAnimTimer, mDirection, mMaxMovingRange, mVelocity, mInitialPos);
	SheepInfoComp::SheepInfoComp()
	{
		mDetectionDis = 100.f;
		mAnimTimer = 0.f;
		mDirection = 1;
		mMaxMovingRange = 50.f;
		mVelocity = 1.5f;
		mInitialPos = glm::vec2(0.f, 0.f);
	}

	REFLECTION(WolfInfoComp, mDetectionDis, mAnimTimer, mDirection, mMaxMovingRange, mVelocity, mInitialPos);
	WolfInfoComp::WolfInfoComp()
	{
		mDetectionDis = 100.f;
		mAnimTimer = 0.f;
		mDirection = 1;
		mMaxMovingRange = 50.f;
		mVelocity = 1.5f;
		mInitialPos = glm::vec2(0.f, 0.f);
	}

	bool SheepdetectPlayer(GameObject* owner);
	bool WolfdetectPlayer(GameObject* owner);
	//										WOLF
	//									 IDDLE STATE

	void Sheep_Idle_State::initialize()
	{
		std::cout << "Ferni Cambiar " << std::endl;
		//mPlayer = objectManager.FindObjectByName("player");
		mOwner = mOwnerStateMachine->GetOwner();
		auto *info = mOwner->AddComponent<SheepInfoComp>();
		info->mInitialPos = mOwner->m_transform->GetPos();
	}

	void Sheep_Idle_State::Enter()
	{
	}

	void Sheep_Idle_State::Exit()
	{
	}

	void Sheep_Idle_State::Update()
	{
		if (SheepdetectPlayer(mOwner))
			mOwnerStateMachine->ChangeState<EnemiesStates::Sheep_Detection_State>();
		auto * info = mOwner->FindComponentByType<SheepInfoComp>();

		info->mAnimTimer += (GLfloat)FRC::FrameRateController::Instance()->GetFrameTime();
		//_____ or esta camuflado?.
		if (info->mAnimTimer >= 0.5f)
		{
			mOwnerStateMachine->ChangeState<EnemiesStates::Sheep_Patrol_State>();
			info->mAnimTimer = 0.0f;
		}
	}

	//                                   PATROL STATE

	void Sheep_Patrol_State::initialize()
	{
		mOwner = mOwnerStateMachine->GetOwner();
	}

	void Sheep_Patrol_State::Enter()
	{
	}

	void Sheep_Patrol_State::Exit()
	{
	}

	void Sheep_Patrol_State::Update()
	{
		if (player == nullptr)
			return;

		if (SheepdetectPlayer(mOwner))
			mOwnerStateMachine->ChangeState<EnemiesStates::Sheep_Detection_State>();

		auto * info = mOwner->FindComponentByType<SheepInfoComp>();

		//get the pos of the sheep
		GLfloat sheepXpos = mOwner->m_transform->GetPos().x;

		info->mAnimTimer += (GLfloat)FRC::FrameRateController::Instance()->GetFrameTime();
		//_____ or esta camuflado?.
		if (info->mAnimTimer >= 1.5f)
		{
			mOwnerStateMachine->ChangeState<EnemiesStates::Sheep_Idle_State>();
			info->mAnimTimer = 0.0f;
		}

		//set in which direction we are moving
		if (sheepXpos > info->mInitialPos.x + info->mMaxMovingRange)
		{
			info->mDirection = -1;
			if (!mOwner->FindComponentByType<Graphics::AnimComp>()->flipX)
			{
				mOwner->FindComponentByType<Graphics::AnimComp>()->FlipAnimation();
				mOwner->FindComponentByType<Graphics::AnimComp>()->flipX = true;
			}
		}
		else if (sheepXpos < info->mInitialPos.x - info->mMaxMovingRange)
		{
			info->mDirection = 1;
			if (mOwner->FindComponentByType<Graphics::AnimComp>()->flipX)
			{
				mOwner->FindComponentByType<Graphics::AnimComp>()->FlipAnimation();
				mOwner->FindComponentByType<Graphics::AnimComp>()->flipX = false;
			}
		}

		//update movement
		mOwner->m_transform->mPosition += glm::vec2(info->mDirection*info->mVelocity, 0.0f);
	}


	//                                   DETECTION STATE

	void Sheep_Detection_State::initialize()
	{
		mOwner = mOwnerStateMachine->GetOwner();
	}

	void Sheep_Detection_State::Enter()
	{
	}

	void Sheep_Detection_State::Exit()
	{
	}

	void Sheep_Detection_State::Update()
	{
		if (player == nullptr)
			return;

		auto * info = mOwner->FindComponentByType<SheepInfoComp>();

		info->mAnimTimer += (GLfloat)FRC::FrameRateController::Instance()->GetFrameTime();
		//_____ or esta camuflado?.
		if (info->mAnimTimer >= 3.0f)
		{
			//mandar eveto de detectado al player
			std::cout << "Detected, 3 seconds has passed" << std::endl;
			info->mAnimTimer = 0.f;
		}

		GLfloat distance = glm::distance(mOwner->m_transform->GetPos(), player->m_transform->GetPos());

		//_____ or esta camuflado or esta detras?.
		if ((distance > info->mDetectionDis))
			mOwnerStateMachine->ChangeState<EnemiesStates::Sheep_Patrol_State>();
	}


	//										WOLF
	//									 IDDLE STATE

	void Wolf_Idle_State::initialize()
	{
		std::cout << "Ferni Cambiar " << std::endl;
		//mPlayer = objectManager.FindObjectByName("player");
		mOwner = mOwnerStateMachine->GetOwner();
		auto *info = mOwner->AddComponent<WolfInfoComp>();
		info->mInitialPos = mOwner->m_transform->GetPos();
	}

	void Wolf_Idle_State::Enter()
	{
	}

	void Wolf_Idle_State::Exit()
	{
	}

	void Wolf_Idle_State::Update()
	{
		if (WolfdetectPlayer(mOwner))
			mOwnerStateMachine->ChangeState<EnemiesStates::Wolf_JumpUp_State>();
		auto * info = mOwner->FindComponentByType<WolfInfoComp>();

		info->mAnimTimer += (GLfloat)FRC::FrameRateController::Instance()->GetFrameTime();
		//_____ or esta camuflado?.
		if (info->mAnimTimer >= 0.5f)
		{
			mOwnerStateMachine->ChangeState<EnemiesStates::Wolf_Patrol_State>();
			info->mAnimTimer = 0.0f;
		}
	}

	//									 PATROL STATE
	void Wolf_Patrol_State::initialize()
	{
		mOwner = mOwnerStateMachine->GetOwner();
	}

	void Wolf_Patrol_State::Enter()
	{

	}

	void Wolf_Patrol_State::Exit()
	{

	}

	void Wolf_Patrol_State::Update()
	{

		if (player == nullptr)
			return;

		if (WolfdetectPlayer(mOwner))
			mOwnerStateMachine->ChangeState<EnemiesStates::Wolf_JumpUp_State>();

		auto * info = mOwner->FindComponentByType<WolfInfoComp>();

		//get the pos of the sheep
		GLfloat wolfXpos = mOwner->m_transform->GetPos().x;

		info->mAnimTimer += (GLfloat)FRC::FrameRateController::Instance()->GetFrameTime();
		//_____ or esta camuflado?.
		if (info->mAnimTimer >= 1.5f)
		{
			mOwnerStateMachine->ChangeState<EnemiesStates::Wolf_Idle_State>();
			info->mAnimTimer = 0.0f;
		}

		//set in which direction we are moving
		if (wolfXpos > info->mInitialPos.x + info->mMaxMovingRange)
		{
			info->mDirection = -1;
			if (!mOwner->FindComponentByType<Graphics::AnimComp>()->flipX)
			{
				mOwner->FindComponentByType<Graphics::AnimComp>()->FlipAnimation();
				mOwner->FindComponentByType<Graphics::AnimComp>()->flipX = true;
			}
		}
		else if (wolfXpos < info->mInitialPos.x - info->mMaxMovingRange)
		{
			info->mDirection = 1;
			if (mOwner->FindComponentByType<Graphics::AnimComp>()->flipX)
			{
				mOwner->FindComponentByType<Graphics::AnimComp>()->FlipAnimation();
				mOwner->FindComponentByType<Graphics::AnimComp>()->flipX = false;
			}
		}

		//update movement
		mOwner->m_transform->mPosition += glm::vec2(info->mDirection*info->mVelocity, 0.0f);
	}

	//									 DETECT STATE

	
	/*void Wolf_Detection_State::initialize()
	{
		mOwner = mOwnerStateMachine->GetOwner();
	}

	void Wolf_Detection_State::Enter()
	{
	}

	void Wolf_Detection_State::Exit()
	{
	}

	void Wolf_Detection_State::Update()
	{
		if (player == nullptr)
			return;

		auto * info = mOwner->FindComponentByType<WolfInfoComp>();

		info->mAnimTimer += (GLfloat)FRC::FrameRateController::Instance()->GetFrameTime();
		//_____ or esta camuflado?.
		if (info->mAnimTimer >= 3.0f)
		{
			//mandar eveto de detectado al player
			std::cout << "Detected, 3 seconds has passed" << std::endl;
			info->mAnimTimer = 0.f;
		}

		GLfloat distance = glm::distance(mOwner->m_transform->GetPos(), player->m_transform->GetPos());

		//_____ or esta camuflado or esta detras?.
		if ((distance > info->mDetectionDis))
			mOwnerStateMachine->ChangeState<EnemiesStates::Wolf_Patrol_State>();
	}*/

	void Wolf_JumpUp_State::initialize()
	{
		mOwner = mOwnerStateMachine->GetOwner();
		info = mOwner->FindComponentByType<WolfInfoComp>();
	}

	void Wolf_JumpUp_State::Enter()
	{
		info->mDistance = glm::distance(mOwner->m_transform->GetPos(), player->m_transform->GetPos());
		info->mAngle = calculateAngle(player->m_transform->GetPos());
		info->mGrav = mOwner->FindComponentByType<RigidBody>()->mGravity.y;
	}

	void Wolf_JumpUp_State::Exit()
	{

	}

	void Wolf_JumpUp_State::Update()
	{
		info->mAnimTimer += (GLfloat)FRC::FrameRateController::Instance()->GetFrameTime();

		GLfloat time = (info->mDistance / info->mVelocity) / 2.0f;

		mOwner->m_transform->mPosition.x = info->mJumpVelocity * glm::cos(info->mAngle) * info->mAnimTimer;
		mOwner->m_transform->mPosition.y = info->mJumpVelocity * glm::sin(info->mAngle) * info->mAnimTimer + 0.5f * info->mGrav * (info->mAnimTimer * info->mAnimTimer);

		info->mAnimTimer = 0;

		if(info->mAnimTimer >= time)
			mOwnerStateMachine->ChangeState<EnemiesStates::Wolf_Idle_State>();
	}

	//Global function for detection player
	bool SheepdetectPlayer(GameObject* owner)
	{
		GLfloat distance = glm::distance(owner->m_transform->GetPos(), player->m_transform->GetPos());

		auto * info = owner->FindComponentByType<SheepInfoComp>();

		//moving direction of the sheep.
		glm::vec2 sheepPointingVec = glm::vec2(info->mDirection, 0.0f);
		//vector between player and sheep,
		glm::vec2 vectorBetween = player->m_transform->GetPos() - owner->m_transform->GetPos();

		GLfloat angle = glm::dot(sheepPointingVec, vectorBetween);

		//_____ or esta camuflado?.
		if ((distance < info->mDetectionDis) && (angle >= 0))
		{
			//raycast-->check if the first collision is with the player.
			return true;
		}
		return false;
	}

	bool WolfdetectPlayer(GameObject* owner)
	{
		GLfloat distance = glm::distance(owner->m_transform->GetPos(), player->m_transform->GetPos());

		auto * info = owner->FindComponentByType<WolfInfoComp>();

		//moving direction of the sheep.
		glm::vec2 sheepPointingVec = glm::vec2(info->mDirection, 0.0f);
		//vector between player and sheep,
		glm::vec2 vectorBetween = player->m_transform->GetPos() - owner->m_transform->GetPos();

		GLfloat angle = glm::dot(sheepPointingVec, vectorBetween);

		//_____ or esta camuflado?.
		if ((distance < info->mDetectionDis) && (angle >= 0))
		{
			//raycast-->check if the first collision is with the player.
			return true;
		}
		return false;
	}

	GLfloat Wolf_JumpUp_State::calculateAngle(glm::vec2 targetPos)
	{
		glm::vec2 ownerPos = mOwner->m_transform->GetPos();

		GLfloat grav = mOwner->FindComponentByType<RigidBody>()->mGravity.y;

		GLfloat k = (grav / 2.0f) * (((targetPos.x * targetPos.x) - (2.0f * targetPos.x * ownerPos.x) + (ownerPos.x * ownerPos.x)) / (info->mJumpVelocity * info->mJumpVelocity));

		glm::vec2 dif = targetPos - ownerPos;

		GLfloat discriminant = (dif.x * dif.x) - (4 * k * (k + dif.y));

		if (discriminant < 0)
		{
			std::cout << "OJO aqui" << std::endl;
			mOwnerStateMachine->ChangeState<EnemiesStates::Wolf_Patrol_State>();
			return 0;
		}

		GLfloat m = glm::sqrt(discriminant);

		std::cout << "Ojo aqui que puede ser dif.x + m" << std::endl;

		return glm::atan((dif.x - m) / (2 * k));
	}
}
