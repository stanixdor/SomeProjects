#include "WolfStates.h"
#include "Engine\Platform\Resource Manager\ResourceManager.h"
#include "Engine\Platform\EventSystem\event_dispatcher.h"
#include "Engine/Physics/CollisionSystem.h"
#include "Components\Checkpoint.h"


REFLECTION(WolfStateMachine, mDetectionDis, mAnimTimer, mMaxMovingRange, mVelocity, mInitialPos, mTimeForIdle, mTimeForPatrol, mTimeDisconcerted, mJumpHeight, mInitGen, cg1);
void WolfStateMachine::Initialize()
{
	m_name = "WolfStateMachine";
	mInitialPos = glm::vec2(.0f, .0f);

	AddState<EnemiesStates::Wolf_Idle_State>();		//States
	AddState<EnemiesStates::Wolf_JumpUp_State>();
	AddState<EnemiesStates::Wolf_Patrol_State>();
	AddState<EnemiesStates::Wolf_Disconcerted_State>();
	AddState<EnemiesStates::Wolf_JumpBack_State>();
	SetInitState<EnemiesStates::Wolf_Idle_State>();

	GetState<EnemiesStates::Wolf_Idle_State>()->CompColumns = 10; //4
	GetState<EnemiesStates::Wolf_Idle_State>()->CompRows = 1;	//2
	GetState<EnemiesStates::Wolf_Idle_State>()->CompSprites = 10;	//8
	GetState<EnemiesStates::Wolf_Idle_State>()->CompDuration = 1;	//2
	GetState<EnemiesStates::Wolf_Idle_State>()->StateTexture = ResourceManager::textures["wolf walk spritesheet.png"].get(); //"jump.png"			
	GetState<EnemiesStates::Wolf_Idle_State>()->CompDelays.resize(10);//machine->GetState<Jump_State>()->CompDelays.resize(8);
	GetState<EnemiesStates::Wolf_Idle_State>()->mAnimSpeed = 0.2f;
	for (auto& delay : GetState<EnemiesStates::Wolf_Idle_State>()->CompDelays)
		delay = 0.042f;

	GetState<EnemiesStates::Wolf_Patrol_State>()->CompColumns = 10; //4
	GetState<EnemiesStates::Wolf_Patrol_State>()->CompRows = 1;	//2
	GetState<EnemiesStates::Wolf_Patrol_State>()->CompSprites = 10;	//8
	GetState<EnemiesStates::Wolf_Patrol_State>()->CompDuration = 1;	//2
	GetState<EnemiesStates::Wolf_Patrol_State>()->StateTexture = ResourceManager::textures["wolf walk spritesheet.png"].get(); //"jump.png"			
	GetState<EnemiesStates::Wolf_Patrol_State>()->CompDelays.resize(10);//machine->GetState<Jump_State>()->CompDelays.resize(8);
	GetState<EnemiesStates::Wolf_Patrol_State>()->mAnimSpeed = 0.2f;//machine->GetState<Jump_State>()->CompDelays.resize(8);
	for (auto& delay : GetState<EnemiesStates::Wolf_Patrol_State>()->CompDelays)
		delay = 0.042f;

	GetState<EnemiesStates::Wolf_JumpUp_State>()->CompColumns = 10; //4
	GetState<EnemiesStates::Wolf_JumpUp_State>()->CompRows = 1;	//2
	GetState<EnemiesStates::Wolf_JumpUp_State>()->CompSprites = 10;	//8
	GetState<EnemiesStates::Wolf_JumpUp_State>()->CompDuration = 1;	//2
	GetState<EnemiesStates::Wolf_JumpUp_State>()->StateTexture = ResourceManager::textures["wolf walk spritesheet.png"].get(); //"jump.png"		
	GetState<EnemiesStates::Wolf_JumpUp_State>()->CompDelays.resize(10);//machine->GetState<Jump_State>()->CompDelays.resize(8);
	GetState<EnemiesStates::Wolf_JumpUp_State>()->mAnimSpeed = 0.2f;//machine->GetState<Jump_State>()->CompDelays.resize(8);
	for (auto& delay : GetState<EnemiesStates::Wolf_JumpUp_State>()->CompDelays)
		delay = 0.042f;

	GetState<EnemiesStates::Wolf_Disconcerted_State>()->CompColumns = 4; //4
	GetState<EnemiesStates::Wolf_Disconcerted_State>()->CompRows = 2;	//2
	GetState<EnemiesStates::Wolf_Disconcerted_State>()->CompSprites = 8;	//8
	GetState<EnemiesStates::Wolf_Disconcerted_State>()->CompDuration = 1;	//2
	GetState<EnemiesStates::Wolf_Disconcerted_State>()->StateTexture = ResourceManager::textures["Question.png"].get(); //"jump.png"		
	GetState<EnemiesStates::Wolf_Disconcerted_State>()->CompDelays.resize(8);//machine->GetState<Jump_State>()->CompDelays.resize(8);
	GetState<EnemiesStates::Wolf_Disconcerted_State>()->mAnimSpeed = 0.2f;//machine->GetState<Jump_State>()->CompDelays.resize(8);
	for (auto& delay : GetState<EnemiesStates::Wolf_Disconcerted_State>()->CompDelays)
		delay = 0.042f;

	GetState<EnemiesStates::Wolf_JumpBack_State>()->CompColumns = 10; //4
	GetState<EnemiesStates::Wolf_JumpBack_State>()->CompRows = 1;	//2
	GetState<EnemiesStates::Wolf_JumpBack_State>()->CompSprites = 10;	//8
	GetState<EnemiesStates::Wolf_JumpBack_State>()->CompDuration = 1;	//2
	GetState<EnemiesStates::Wolf_JumpBack_State>()->StateTexture = ResourceManager::textures["wolf walk spritesheet.png"].get(); //"jump.png"	
	GetState<EnemiesStates::Wolf_JumpBack_State>()->CompDelays.resize(10);//machine->GetState<Jump_State>()->CompDelays.resize(8);
	GetState<EnemiesStates::Wolf_JumpBack_State>()->mAnimSpeed = 0.3f;//machine->GetState<Jump_State>()->CompDelays.resize(8);
	for (auto& delay : GetState<EnemiesStates::Wolf_JumpBack_State>()->CompDelays)
		delay = 0.042f;
}

void WolfStateMachine::Update()
{
	if (mPlayer == nullptr)
		mPlayer = objectManager.FindObjectByName("player");

	if (mPlayer != nullptr && mCamuflaje == nullptr)
		mCamuflaje = mPlayer->FindComponentByType<CamouflageComponent>();
	StateMachine::Update();
}

bool WolfStateMachine::WolfdetectPlayer(GameObject* owner)
{
	if (mCamuflaje == nullptr)
		std::cout << "El player no tiene puto camuflaje de los cojones" << std::endl;

	GLfloat distance = glm::distance(owner->m_transform->GetPos(), mPlayer->m_transform->GetPos());
	/*std::cout << "Distance between player: "  << distance << std::endl;
	std::cout << "detect distance: " << mDetectionDis << std::endl;*/

	//moving direction of the sheep.
	glm::vec2 sheepPointingVec = glm::vec2(mDirection, 0.0f);
	//vector between player and sheep,
	glm::vec2 vectorBetween = mPlayer->m_transform->GetPos() - owner->m_transform->GetPos();

	GLfloat angle = glm::dot(sheepPointingVec, vectorBetween);
	//std::cout << "angle: " << angle << std::endl;

	//_____ or esta camuflado?.
	if ((distance < mDetectionDis) && (angle >= 0) /*&& mCamuflaje->IsVisible() == true*/)
	{
		//raycast-->check if the first collision is with the player.
		return true;
	}
	return false;
}

glm::vec2 WolfStateMachine::Jump(GLfloat time, glm::vec2 start, glm::vec2 end, glm::vec2 mid)
{
	//berzier formula.
	return (1 - time)*(1 - time)*start + 2 * time*(1 - time) * mid + time*time*end;
}


namespace EnemiesStates
{
	//										WOLF
	//									 IDDLE STATE

	void Wolf_Idle_State::initialize()
	{
		mOwner = mOwnerStateMachine->GetOwner();
		 
		mWolfMachine = dynamic_cast<WolfStateMachine*>(mOwnerStateMachine);

		mWolfMachine->mInitialPos = mWolfMachine->mInitGen;
		mOwner->m_transform->mPosition = mWolfMachine->mInitGen;

		mWolfMachine->mPlayer = objectManager.FindObjectByName("player");
		//Events stuff
		using namespace Messaging;

		EventHandler* temp = &mOwner->GetEventHandler();

		mOwnerStateMachine->GetOwner()->GetEventHandler().register_handler(this, &Wolf_Idle_State::collisionStarted);

		EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionStarted>());
	}
	
	void Wolf_Idle_State::collisionStarted(const OnCollisionStarted& mEvent)
	{
		if (mEvent.mData.obj1->getCollisionGroup() == CollisionGroup::GetCollisionGroup("player") || mEvent.mData.obj2->getCollisionGroup() == CollisionGroup::GetCollisionGroup("player"))
		{
			mWolfMachine->mPlayer->m_transform->SetPos(Checkpoint::respawnPosition);
		}
	}

	void Wolf_Idle_State::Enter()
	{
		mWolfMachine->mAnimTimer = 0.0f;
	}

	void Wolf_Idle_State::Exit()
	{
	}

	void Wolf_Idle_State::Update()
	{
		if (mWolfMachine->mPlayer == nullptr)
			return;
		if (mWolfMachine->WolfdetectPlayer(mOwner))
			mOwnerStateMachine->ChangeState<EnemiesStates::Wolf_JumpUp_State>();

		mWolfMachine->mAnimTimer += (GLfloat)FRC::FrameRateController::Instance()->GetFrameTime();
		//_____ or esta camuflado?.
		if (mWolfMachine->mAnimTimer >= mWolfMachine->mTimeForPatrol)
			mOwnerStateMachine->ChangeState<EnemiesStates::Wolf_Patrol_State>();
	}

	//									 PATROL STATE
	void Wolf_Patrol_State::initialize()
	{
		mOwner = mOwnerStateMachine->GetOwner();

		mWolfMachine = dynamic_cast<WolfStateMachine*>(mOwnerStateMachine);

		//Events stuff
		using namespace Messaging;

		EventHandler* temp = &mOwner->GetEventHandler();

		mOwnerStateMachine->GetOwner()->GetEventHandler().register_handler(this, &Wolf_Patrol_State::collisionStarted);

		EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionStarted>());
	}

	void Wolf_Patrol_State::collisionStarted(const OnCollisionStarted& mEvent)
	{
		if (mEvent.mData.obj1->getCollisionGroup() == CollisionGroup::GetCollisionGroup("player") || mEvent.mData.obj2->getCollisionGroup() == CollisionGroup::GetCollisionGroup("player"))
		{
			mWolfMachine->mPlayer->m_transform->SetPos(Checkpoint::respawnPosition);
		}
	}

	void Wolf_Patrol_State::Enter()
	{
		mWolfMachine->mAnimTimer = 0.0f;
		mAnim = mOwner->FindComponentByType<Graphics::AnimComp>();

		/*if (mOwner->m_transform->GetPos().x > mWolfMachine->mInitialPos.x)
			mWolfMachine->mDirection = -1;
		else
			mWolfMachine->mDirection = 1;*/
	}

	void Wolf_Patrol_State::Exit()
	{

	}

	void Wolf_Patrol_State::Update()
	{
		if (mWolfMachine->mPlayer == nullptr || mAnim == nullptr)
			return;

		if (mWolfMachine->WolfdetectPlayer(mOwner))
			mOwnerStateMachine->ChangeState<EnemiesStates::Wolf_JumpUp_State>();

		//get the pos of the sheep
		GLfloat wolfXpos = mOwner->m_transform->GetPos().x;

		mWolfMachine->mAnimTimer += (GLfloat)FRC::FrameRateController::Instance()->GetFrameTime();
		//_____ or esta camuflado?.
		if (mWolfMachine->mAnimTimer >= mWolfMachine->mTimeForIdle)
			mOwnerStateMachine->ChangeState<EnemiesStates::Wolf_Idle_State>();

		//set in which direction we are moving
		if (wolfXpos >= (mWolfMachine->mInitGen.x + mWolfMachine->mMaxMovingRange) - 0.1f)
		{
			mWolfMachine->mDirection *= -1;
			//if (mAnim->flipX)
				mAnim->FlipAnimation();
		}
		else if (wolfXpos <= (mWolfMachine->mInitGen.x - mWolfMachine->mMaxMovingRange) + 0.1f)
		{
			mWolfMachine->mDirection *= -1;
			//if (!mAnim->flipX)
				mAnim->FlipAnimation();
		}

		//update movement
		mOwner->m_transform->mPosition += glm::vec2(mWolfMachine->mDirection*mWolfMachine->mVelocity, 0.0f);
	}


	//									 JUMP STATE
	void Wolf_JumpUp_State::initialize()
	{
		mOwner = mOwnerStateMachine->GetOwner();

		mWolfMachine = dynamic_cast<WolfStateMachine*>(mOwnerStateMachine);

		//Events stuff
		using namespace Messaging;

		EventHandler* temp = &mOwner->GetEventHandler();

		mOwnerStateMachine->GetOwner()->GetEventHandler().register_handler(this, &Wolf_JumpUp_State::collisionStarted);
		mOwnerStateMachine->GetOwner()->GetEventHandler().register_handler(this, &Wolf_JumpUp_State::collisionEnded);

		EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionStarted>());
		EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionEnded>());
	}

	void Wolf_JumpUp_State::collisionStarted(const OnCollisionStarted& mEvent)
	{
		if (mEvent.mData.obj1->getCollisionGroup() == CollisionGroup::GetCollisionGroup("player") || mEvent.mData.obj2->getCollisionGroup() == CollisionGroup::GetCollisionGroup("player"))
		{
			mWolfMachine->mPlayer->m_transform->SetPos(Checkpoint::respawnPosition);
		}
		else if((onAir == true) && mEvent.mData.obj1->getCollisionGroup() == CollisionGroup::GetCollisionGroup("floor") || mEvent.mData.obj2->getCollisionGroup() == CollisionGroup::GetCollisionGroup("floor"))
		{
			mOwnerStateMachine->ChangeState<Wolf_Disconcerted_State>();
			onAir = false;
		}
	}

	void Wolf_JumpUp_State::collisionEnded(const OnCollisionPersisted& mEvent)
	{
		if (mEvent.mData.obj1->getCollisionGroup() == CollisionGroup::GetCollisionGroup("floor") || mEvent.mData.obj2->getCollisionGroup() == CollisionGroup::GetCollisionGroup("floor"))
		{
			onAir = true;
		}
	}

	void Wolf_JumpUp_State::Enter()
	{
		if (mWolfMachine->mPlayer == nullptr)
			return;
		mWolfMachine->mDistance = glm::distance(mOwner->m_transform->GetPos(), mWolfMachine->mPlayer->m_transform->GetPos());
		mWolfMachine->mAnimTimer = 0.0f;

		//needed points for computing the parabola with Bezier algorithm.
		mWolfMachine->mJumpP0 = mOwner->m_transform->GetPos();
		mWolfMachine->mJumpP2 = mWolfMachine->mPlayer->m_transform->GetPos();
		glm::vec2 PM = (mWolfMachine->mJumpP0 + mWolfMachine->mJumpP2) / 2.0f;
		mWolfMachine->mJumpP1 = PM;
		mWolfMachine->mJumpP1.y = PM.y + mWolfMachine->mJumpHeight;
	}

	void Wolf_JumpUp_State::Exit()
	{
	}
	void Wolf_JumpUp_State::Update()
	{
		if (mWolfMachine->mPlayer == nullptr)
			return;
		mOwner->m_transform->SetPos(mWolfMachine->Jump(mWolfMachine->mAnimTimer, mWolfMachine->mJumpP0, mWolfMachine->mJumpP2, mWolfMachine->mJumpP1));
		
		mWolfMachine->mAnimTimer += 0.025f;
		onAir = true;
	}


	//									 DISCONCERTED STATE
	void Wolf_Disconcerted_State::initialize()
	{
		mOwner = mOwnerStateMachine->GetOwner();

		mWolfMachine = dynamic_cast<WolfStateMachine*>(mOwnerStateMachine);

		//Events stuff
		using namespace Messaging;

		EventHandler* temp = &mOwner->GetEventHandler();

		mOwnerStateMachine->GetOwner()->GetEventHandler().register_handler(this, &Wolf_Disconcerted_State::collisionStarted);

		EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionStarted>());
	}

	void Wolf_Disconcerted_State::collisionStarted(const OnCollisionStarted& mEvent)
	{
		if (mEvent.mData.obj1->getCollisionGroup() == CollisionGroup::GetCollisionGroup("player") || mEvent.mData.obj2->getCollisionGroup() == CollisionGroup::GetCollisionGroup("player"))
		{
			mWolfMachine->mPlayer->m_transform->SetPos(Checkpoint::respawnPosition);
		}
	}

	void Wolf_Disconcerted_State::Enter()
	{
		mWolfMachine->mAnimTimer = 0.0f;
	}

	void Wolf_Disconcerted_State::Update()
	{
		mWolfMachine->mAnimTimer += static_cast<GLfloat>(FRC::FrameRateController::Instance()->GetFrameTime());

		if (mWolfMachine->mAnimTimer >= mWolfMachine->mTimeDisconcerted)
		{
			mOwnerStateMachine->ChangeState<Wolf_JumpBack_State>();
		}
	}

	void Wolf_Disconcerted_State::Exit()
	{

	}

	void Wolf_JumpBack_State::initialize()
	{
		mOwner = mOwnerStateMachine->GetOwner();

		mWolfMachine = dynamic_cast<WolfStateMachine*>(mOwnerStateMachine);

		//Events stuff
		using namespace Messaging;

		EventHandler* temp = &mOwner->GetEventHandler();

		mOwnerStateMachine->GetOwner()->GetEventHandler().register_handler(this, &Wolf_JumpBack_State::collisionStarted);

		EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionStarted>());
	}

	void Wolf_JumpBack_State::collisionStarted(const OnCollisionStarted& mEvent)
	{
		if (mEvent.mData.obj1->getCollisionGroup() == CollisionGroup::GetCollisionGroup("player") || mEvent.mData.obj2->getCollisionGroup() == CollisionGroup::GetCollisionGroup("player"))
		{
			mWolfMachine->mPlayer->m_transform->SetPos(Checkpoint::respawnPosition);
		}
		//else
		//{
		//	mOwnerStateMachine->ChangeState<Wolf_Disconcerted_State>();
		//}
	}
	void Wolf_JumpBack_State::Enter()
	{
		if (mWolfMachine->mPlayer == nullptr)
			return;
		mWolfMachine->mAnimTimer = 0.0f;
		mAnim = mOwner->FindComponentByType<Graphics::AnimComp>();

		if (mOwner->m_transform->GetPos().x > mWolfMachine->mInitGen.x)
		{
			//if (mAnim->flipX)
				mAnim->FlipAnimation();
			mWolfMachine->mDirection *= -1;
		}
		else {
			//if (!mAnim->flipX)
				mAnim->FlipAnimation();
			mWolfMachine->mDirection *= -1;
		}


	}

	void Wolf_JumpBack_State::Update()
	{
		if (mWolfMachine->mPlayer == nullptr || mAnim == nullptr)
			return;

		if ((mOwner->m_transform->GetPos().x >= (mWolfMachine->mInitGen.x - 5.f)) && (mOwner->m_transform->GetPos().x <= (mWolfMachine->mInitGen.x + 5.f)))
			mOwnerStateMachine->ChangeState<EnemiesStates::Wolf_Patrol_State>();

		//GLfloat wolfXpos = mOwner->m_transform->GetPos().x;

		mOwner->m_transform->mPosition += glm::vec2(mWolfMachine->mDirection*mWolfMachine->mVelocity, 0.0f);
	}

	void Wolf_JumpBack_State::Exit()
	{

	}

}