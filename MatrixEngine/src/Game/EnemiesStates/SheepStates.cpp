//Mikel Fernandez

#include "SheepStates.h"
#include "Physics\RigidBody.h"
#include "Engine\Platform\Resource Manager\ResourceManager.h"
#include "Engine\Platform\EventSystem\event_dispatcher.h"
#include "Engine/Physics/CollisionSystem.h"
#include "Components\Checkpoint.h"

REFLECTION(SheepStateMachine, mDetectionDis, mTimeForIdle, mTimeForPatrol, mTimeForDetect, mMaxMovingRange, mVelocity, mInitialPos, mInitGen, cg1);
void SheepStateMachine::Initialize()
{
	m_name = "SheepStateMachine";

	AddState<EnemiesStates::Sheep_Patrol_State>();		//States
	AddState<EnemiesStates::Sheep_Detection_State>();
	AddState<EnemiesStates::Sheep_Idle_State>();
	AddState<EnemiesStates::Sheep_Detected_State>();
	SetInitState<EnemiesStates::Sheep_Idle_State>();

	GetState<EnemiesStates::Sheep_Idle_State>()->CompColumns = 5; //4
	GetState<EnemiesStates::Sheep_Idle_State>()->CompRows = 1;	//2
	GetState<EnemiesStates::Sheep_Idle_State>()->CompSprites = 5;	//8
	GetState<EnemiesStates::Sheep_Idle_State>()->CompDuration = 1;	//2
	GetState<EnemiesStates::Sheep_Idle_State>()->StateTexture = ResourceManager::textures["Sheep Idle.png"].get(); //"jump.png"
	GetState<EnemiesStates::Sheep_Idle_State>()->CompDelays.resize(5);
	GetState<EnemiesStates::Sheep_Idle_State>()->mAnimSpeed = 1.0f;
	for (auto& delay : GetState<EnemiesStates::Sheep_Idle_State>()->CompDelays)
		delay = 0.12f;

	GetState<EnemiesStates::Sheep_Patrol_State>()->CompColumns = 4; //4
	GetState<EnemiesStates::Sheep_Patrol_State>()->CompRows = 3;	//2
	GetState<EnemiesStates::Sheep_Patrol_State>()->CompSprites = 12;	//8
	GetState<EnemiesStates::Sheep_Patrol_State>()->CompDuration = 1;	//2
	GetState<EnemiesStates::Sheep_Patrol_State>()->StateTexture = ResourceManager::textures["Sheep walk spritesheet.png"].get(); //"jump.png"
	GetState<EnemiesStates::Sheep_Patrol_State>()->CompDelays.resize(12);
	GetState<EnemiesStates::Sheep_Patrol_State>()->mAnimSpeed = 0.8f;
	for (auto& delay : GetState<EnemiesStates::Sheep_Patrol_State>()->CompDelays)
		delay = 0.042f;


	GetState<EnemiesStates::Sheep_Detection_State>()->CompColumns = 5; //4
	GetState<EnemiesStates::Sheep_Detection_State>()->CompRows = 1;	//2
	GetState<EnemiesStates::Sheep_Detection_State>()->CompSprites = 5;	//8
	GetState<EnemiesStates::Sheep_Detection_State>()->CompDuration = 1;	//2
	GetState<EnemiesStates::Sheep_Detection_State>()->StateTexture = ResourceManager::textures["Sheep Idle.png"].get(); //"jump.png"
	GetState<EnemiesStates::Sheep_Detection_State>()->CompDelays.resize(5);
	GetState<EnemiesStates::Sheep_Detection_State>()->mAnimSpeed = 0.8f;
	for (auto& delay : GetState<EnemiesStates::Sheep_Detection_State>()->CompDelays)
		delay = 0.042f;

	GetState<EnemiesStates::Sheep_Detected_State>()->CompColumns = 11; //4
	GetState<EnemiesStates::Sheep_Detected_State>()->CompRows = 1;	//2
	GetState<EnemiesStates::Sheep_Detected_State>()->CompSprites = 11;	//8
	GetState<EnemiesStates::Sheep_Detected_State>()->CompDuration = 1;	//2
	GetState<EnemiesStates::Sheep_Detected_State>()->StateTexture = ResourceManager::textures["Sheep L3.png"].get(); //"jump.png"
	GetState<EnemiesStates::Sheep_Detected_State>()->CompDelays.resize(11);
	GetState<EnemiesStates::Sheep_Detected_State>()->mAnimSpeed = 1.0f;
	//for (auto& delay : GetState<EnemiesStates::Sheep_Detected_State>()->CompDelays)
	//	delay = 0.042f;
	//auto * state = GetState<EnemiesStates::Sheep_Detected_State>();
	GetState<EnemiesStates::Sheep_Detected_State>()->CompDelays[0] = GetState<EnemiesStates::Sheep_Detected_State>()->CompDelays[2] = GetState<EnemiesStates::Sheep_Detected_State>()->CompDelays[3] = GetState<EnemiesStates::Sheep_Detected_State>()->CompDelays[4] = GetState<EnemiesStates::Sheep_Detected_State>()->CompDelays[5] = GetState<EnemiesStates::Sheep_Detected_State>()->CompDelays[8] = GetState<EnemiesStates::Sheep_Detected_State>()->CompDelays[9] = 0.08f;
	GetState<EnemiesStates::Sheep_Detected_State>()->CompDelays[1] = GetState<EnemiesStates::Sheep_Detected_State>()->CompDelays[6] = GetState<EnemiesStates::Sheep_Detected_State>()->CompDelays[7] = 0.04f;
	GetState<EnemiesStates::Sheep_Detected_State>()->CompDelays[10] = 999.0f;
}

void SheepStateMachine::Update()
{
	if (mPlayer == nullptr)
		mPlayer = objectManager.FindObjectByName("player");

	if (mPlayer != nullptr && mCamuflaje == nullptr)
		mCamuflaje = mPlayer->FindComponentByType<CamouflageComponent>();
	StateMachine::Update();
}



GameObject* SheepStateMachine::CreateObject(std::string texName)
{
	GameObject*  myOwner = GetOwner();
	detectionAnim = objectManager.AddGameObject("sheepQuestion");
	detectionAnim->m_transform->SetPos(glm::vec2((myOwner->m_transform->GetPos().x + mDirection*(myOwner->m_transform->GetScale().x / 4.f)), myOwner->m_transform->GetPos().y + myOwner->m_transform->GetScale().y));
	detectionAnim->m_transform->SetScale(glm::vec2(150.0f, 150.0f));

	auto * anim = detectionAnim->AddComponent<Graphics::AnimComp>();
	anim->CompColumns = 4;
	anim->CompRows = 4;
	anim->CompSprites = 16;
	anim->CompDuration = 1;
	anim->pTexture = ResourceManager::textures[texName].get();
	anim->CompDelays.resize(16);
	anim->mAnimSpeed = 1.0f;
	anim->CompDelays[0] = anim->CompDelays[1] = anim->CompDelays[2] = anim->CompDelays[3] = anim->CompDelays[4] = 0.04f;
	anim->CompDelays[5] = anim->CompDelays[6] = anim->CompDelays[7] = anim->CompDelays[9] = 0.04f;
	anim->CompDelays[8] = anim->CompDelays[10] = anim->CompDelays[13] = 0.08f;
	anim->CompDelays[11] = anim->CompDelays[12] = 0.04f;
	anim->CompDelays[14] = 0.04f;
	anim->CompDelays[15] = 99.0f;

	return detectionAnim;
}


//Global function for detection player
bool SheepStateMachine::SheepdetectPlayer(GameObject* owner)
{
	if (mCamuflaje == nullptr)
		std::cout << "El player no tiene puto camuflaje de los cojones" << std::endl;
	GLfloat distance = glm::distance(owner->m_transform->GetPos(), mPlayer->m_transform->GetPos());

	//moving direction of the sheep.
	glm::vec2 sheepPointingVec = glm::vec2(mDirection, 0.0f);
	//vector between player and sheep,
	glm::vec2 vectorBetween = mPlayer->m_transform->GetPos() - owner->m_transform->GetPos();

	GLfloat angle = glm::dot(sheepPointingVec, vectorBetween);

	//_____ or esta camuflado?.
	if ((distance < mDetectionDis) && (angle >= 0) && mCamuflaje->IsVisible() == true)
	{
		//raycast-->check if the first collision is with the player.
		return true;
	}
	return false;
}


namespace EnemiesStates
{
	//										WOLF
	//									 IDDLE STATE
	void Sheep_Idle_State::initialize()
	{
		mOwner = mOwnerStateMachine->GetOwner();

		mSheepMachine = dynamic_cast<SheepStateMachine*>(mOwnerStateMachine);

		//if(mSheepMachine->mInitialPos.x == 0.0f && mSheepMachine->mInitialPos.y == 0.0f)
		mSheepMachine->mInitialPos = mSheepMachine->mInitGen;
		mOwner->m_transform->mPosition = mSheepMachine->mInitGen;

		//Events stuff
		using namespace Messaging;

		EventHandler* temp = &mOwner->GetEventHandler();

		mOwnerStateMachine->GetOwner()->GetEventHandler().register_handler(this, &Sheep_Idle_State::collisionStarted);

		EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionStarted>());
	}

	void Sheep_Idle_State::collisionStarted(const OnCollisionStarted& mEvent)
	{
		if (mEvent.mData.obj1->getCollisionGroup() == CollisionGroup::GetCollisionGroup("player") || mEvent.mData.obj2->getCollisionGroup() == CollisionGroup::GetCollisionGroup("player"))
		{
			mOwnerStateMachine->ChangeState<EnemiesStates::Sheep_Detected_State>();
		}
	}

	void Sheep_Idle_State::Enter()
	{
		mSheepMachine->mAnimTimer = 0.f;

		mAnim = mOwner->FindComponentByType<Graphics::AnimComp>();
		
		if (mSheepMachine->mDirection == 1)
		{
			fliped = true;
			mAnim->FlipAnimation();
		}
	}

	void Sheep_Idle_State::Exit()
	{
		if (fliped)
		{
			fliped = false;
			mAnim->FlipAnimation();
		}
	}

	void Sheep_Idle_State::Update()
	{
		if (mSheepMachine->mPlayer == nullptr || mAnim == nullptr)
			return;

		if (mSheepMachine->SheepdetectPlayer(mOwner))
			mOwnerStateMachine->ChangeState<EnemiesStates::Sheep_Detection_State>();

		mSheepMachine->mAnimTimer += (GLfloat)FRC::FrameRateController::Instance()->GetFrameTime();

		if (mSheepMachine->mAnimTimer >= mSheepMachine->mTimeForPatrol)
			mOwnerStateMachine->ChangeState<EnemiesStates::Sheep_Patrol_State>();
	}

	//                                   PATROL STATE

	void Sheep_Patrol_State::initialize()
	{
		mOwner = mOwnerStateMachine->GetOwner();

		mSheepMachine = dynamic_cast<SheepStateMachine*>(mOwnerStateMachine);

		//Events stuff
		using namespace Messaging;

		EventHandler* temp = &mOwner->GetEventHandler();

		mOwnerStateMachine->GetOwner()->GetEventHandler().register_handler(this, &Sheep_Patrol_State::collisionStarted);

		EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionStarted>());
	}

	void Sheep_Patrol_State::collisionStarted(const OnCollisionStarted& mEvent)
	{
		if (mEvent.mData.obj1->getCollisionGroup() == CollisionGroup::GetCollisionGroup("player") || mEvent.mData.obj2->getCollisionGroup() == CollisionGroup::GetCollisionGroup("player"))
		{
			mOwnerStateMachine->ChangeState<EnemiesStates::Sheep_Detected_State>();
		}
	}

	void Sheep_Patrol_State::Enter()
	{
		mSheepMachine->mAnimTimer = 0.f;
		mAnim = mOwner->FindComponentByType<Graphics::AnimComp>();

		if (mSheepMachine->mDirection == -1)
		{
			fliped = true;
			mAnim->FlipAnimation();
		}
	}

	void Sheep_Patrol_State::Exit()
	{
		if (fliped)
		{
			fliped = false;
			mAnim->FlipAnimation();
		}
	}

	void Sheep_Patrol_State::Update()
	{
		if (mSheepMachine->mPlayer == nullptr || mAnim == nullptr)
			return;

		if (mSheepMachine->SheepdetectPlayer(mOwner))
			mOwnerStateMachine->ChangeState<EnemiesStates::Sheep_Detection_State>();

		//get the pos of the sheep
		GLfloat sheepXpos = mOwner->m_transform->GetPos().x;

		mSheepMachine->mAnimTimer += (GLfloat)FRC::FrameRateController::Instance()->GetFrameTime();

		if (mSheepMachine->mAnimTimer >= mSheepMachine->mTimeForIdle)
			mOwnerStateMachine->ChangeState<EnemiesStates::Sheep_Idle_State>();

		//set in which direction we are moving
		if (sheepXpos >= (mSheepMachine->mInitGen.x + mSheepMachine->mMaxMovingRange) - 0.1f)
		{
			fliped = true;
			mSheepMachine->mDirection *= -1;
			//if (!mAnim->flipX)
			mAnim->FlipAnimation();
		}
		else if (sheepXpos <= (mSheepMachine->mInitGen.x - mSheepMachine->mMaxMovingRange) + 0.1f)
		{
			fliped = false;
			mSheepMachine->mDirection *= -1;
			//if (mAnim->flipX)
			mAnim->FlipAnimation();
		}

		//update movement
		mOwner->m_transform->mPosition += glm::vec2(mSheepMachine->mDirection*mSheepMachine->mVelocity, 0.0f);
	}


	//                                   DETECTION STATE

	void Sheep_Detection_State::initialize()
	{
		mOwner = mOwnerStateMachine->GetOwner();

		mSheepMachine = dynamic_cast<SheepStateMachine*>(mOwnerStateMachine);

		//Events stuff
		using namespace Messaging;

		EventHandler* temp = &mOwner->GetEventHandler();

		mOwnerStateMachine->GetOwner()->GetEventHandler().register_handler(this, &Sheep_Detection_State::collisionStarted);

		EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionStarted>());
	}

	void Sheep_Detection_State::collisionStarted(const OnCollisionStarted& mEvent)
	{
		if (mEvent.mData.obj1->getCollisionGroup() == CollisionGroup::GetCollisionGroup("player") || mEvent.mData.obj2->getCollisionGroup() == CollisionGroup::GetCollisionGroup("player"))
		{
			mOwnerStateMachine->ChangeState<EnemiesStates::Sheep_Detected_State>();
		}
	}

	void Sheep_Detection_State::Enter()
	{
		mSheepMachine->mAnimTimer = 0.f;
		mAnim = mOwner->FindComponentByType<Graphics::AnimComp>();
		if (mSheepMachine->mDirection == 1)
		{
			fliped = true;
			mAnim->FlipAnimation();
		}
		mCamuflaje = mOwner->FindComponentByType<SheepStateMachine>()->mPlayer->FindComponentByType<CamouflageComponent>();
		question = mSheepMachine->CreateObject("Interrogation animation.png");
		questionAnim = question->FindComponentByType<Graphics::AnimComp>();
	}

	void Sheep_Detection_State::Exit()
	{
		if (fliped)
		{
			fliped = false;
			mAnim->FlipAnimation();
		}
		//objectManager.FindObjectByName("sheepQuestion")->Destroy();
		
		questionAnim->SetAnimationFrame(0);
		question->Destroy();
	}

	void Sheep_Detection_State::Update()
	{
		if (mSheepMachine->mPlayer == nullptr || mAnim == nullptr || mCamuflaje == nullptr)
			return;
		//forceBounceTimer += (GLfloat)FRC::FrameRateController::Instance()->GetFrameTime();
		if (questionAnim->mAnimCurrentFrame == 15)
		{
			forceBounceTimer += (GLfloat)FRC::FrameRateController::Instance()->GetFrameTime();
			if (forceBounceTimer > 0.56f)
			{
				questionAnim->SetAnimationFrame(8);
				forceBounceTimer = 0;
			}

			return;
		}
		mSheepMachine->mAnimTimer += (GLfloat)FRC::FrameRateController::Instance()->GetFrameTime();
		//_____ or esta camuflado?.
		if (mSheepMachine->mAnimTimer >= mSheepMachine->mTimeForDetect)
			mOwnerStateMachine->ChangeState<EnemiesStates::Sheep_Detected_State>();

		GLfloat distance = glm::distance(mOwner->m_transform->GetPos(), mSheepMachine->mPlayer->m_transform->GetPos());

		//_____ or esta camuflado or esta detras?.
		if ((distance > mSheepMachine->mDetectionDis) && mCamuflaje->IsVisible() == true)
			mOwnerStateMachine->ChangeState<EnemiesStates::Sheep_Patrol_State>();
	}

	//                                   DETECTED STATE

	void Sheep_Detected_State::initialize()
	{
		mOwner = mOwnerStateMachine->GetOwner();

		mSheepMachine = dynamic_cast<SheepStateMachine*>(mOwnerStateMachine);
	}

	void Sheep_Detected_State::Enter()
	{
		mSheepMachine->mAnimTimer = 0.f;
		mAnim = mOwner->FindComponentByType<Graphics::AnimComp>();
		if (mSheepMachine->mDirection == 1)
		{
			fliped = true;
			mAnim->FlipAnimation();
		}
		exclamation = mSheepMachine->CreateObject("Exclamation animation.png");
		exclamationAnim = exclamation->FindComponentByType<Graphics::AnimComp>();
	}

	void Sheep_Detected_State::Exit()
	{
		if (fliped)
		{
			fliped = false;
			mAnim->FlipAnimation();
		}
		mSheepMachine->mAnimTimer = 0.f;
		exclamation->Destroy();
		exclamationAnim->SetAnimationFrame(0);
	}

	void Sheep_Detected_State::Update()
	{
		if (mSheepMachine->mPlayer == nullptr || mAnim == nullptr)
			return;

		if (exclamationAnim->mAnimCurrentFrame == 15)
		{

			forceBounceTimer += (GLfloat)FRC::FrameRateController::Instance()->GetFrameTime();
			if (forceBounceTimer > 0.56f)
			{
				exclamationAnim->SetAnimationFrame(8);
				forceBounceTimer = 0;
			}

			return;
		}

		mSheepMachine->mAnimTimer += (GLfloat)FRC::FrameRateController::Instance()->GetFrameTime();

		//TODO: PLAY ANIMATION FOR DETECTED DURING A CERTAIN TIME(ALARM).
		if (mSheepMachine->mAnimTimer >= 1.5f)
		{
			mSheepMachine->mPlayer->m_transform->SetPos(Checkpoint::respawnPosition);
			mOwnerStateMachine->ChangeState<EnemiesStates::Sheep_Patrol_State>();
		}
	}
}