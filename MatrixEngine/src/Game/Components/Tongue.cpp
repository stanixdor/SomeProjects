#include "Platform\Time\FrameRateController.h"
#include "Platform\Input\Input.h"
#include "Physics\CollisionTable.h"
#include "Engine/Physics/CollisionSystem.h"
#include "Engine\Platform\EventSystem\event_dispatcher.h"
#include "Logic\StateMachine\StateMachine.h"
//#include "Logic\StateMachine\PlayerStateMachine.h"
#include "Levels\LevelTest.h"
#include "Components\HeadComponent.h"
#include "Platform\Resource Manager\ResourceManager.h"
#include "Physics\Collider.h"
#include "Components/FlyComponent.h"
#include "Components/CamouflageComponent.h"
#include "Tongue.h"

bool TongueComponent::paused = false;

TongueComponent::TongueComponent() : Component("TongueComponent")
{
	IncreaseSizeSpeed = 70.0f;//50.0f	//Initalize the variables
	HalfIncreaseSizeSpeed = IncreaseSizeSpeed / 2.0f;
	DistanceCounter = 0.0f;
	MaxDistance = 0.2f;
	Return = false;
	MaxLength = 500.0f;
	MaxTime = 2.0f;

	m_name = std::string("TongueComponent");
}

void TongueComponent::Update()
{
	if (paused)
		return;
	if (ThrowHead == nullptr)
	{
		ThrowHead = objectManager.FindObjectByName("ThrowHead");

		if (ThrowHead != nullptr)
		{
			Begginning->m_transform->mPosition = ThrowHead->m_transform->GetPos() + DirectionVector * 20.0f;
			Ending->m_transform->mPosition.x = Begginning->m_transform->mPosition.x + InitialLength;
			Ending->m_transform->mPosition.y = Begginning->m_transform->mPosition.y;
			MidPart->m_transform->mPosition = ComputeMidpoint(Begginning->m_transform->mPosition, Ending->m_transform->mPosition);

			StartingMidPartSize = MidPart->m_transform->mScale;
			MidPartMinSize = StartingMidPartSize * 0.2f;
			MidPartMaxSize = StartingMidPartSize * 1.2f;
			StartingMidPartPos = MidPart->m_transform->mPosition;
			Ending->m_transform->mScale /= 2.0f;
		}
	}

	if (FlyCatched != nullptr)
	{
		FlyEaten = true;
	}
	else
		FlyEaten = false;

	if (Playerptr == nullptr)
	{
		Playerptr = objectManager.FindObjectByName("player");
		std::cout << "PlayerNULL on tongue.cpp" << std::endl;
	}



	Increase_DecreaseTongue();

	if (FlyEaten)
	{
		GotAFly();
	}
}

void TongueComponent::LateUpdate()
{
	if (DeleteTongue)
	{
		DestroyAllObjects();
		return;
	}

	if (m_alive == false)
	{
		Owner = nullptr;
		Begginning = nullptr;
		MidPart = nullptr;
		Ending = nullptr;
		Outline = nullptr;
	}

	if (FlyCatched != nullptr && FlyCatched->m_alive == false)
	{
		FlyCatched = nullptr;
	}
}

TongueComponent::~TongueComponent()
{
	using namespace Messaging;
	Messaging::EventHandler* temp = &Owner->GetEventHandler();

	////subscribe this handler to this type of event
	Messaging::EventDispatcher::get_instance().unsubscribe(*temp, type_of<OnCollisionStarted>());
	Messaging::EventDispatcher::get_instance().unsubscribe(*temp, type_of<OnCollisionPersisted>());
	Messaging::EventDispatcher::get_instance().unsubscribe(*temp, type_of<OnCollisionEnded>());
}
void TongueComponent::Initialize()
{
	Owner = this->GetOwner();	//Save the object

	Playerptr = objectManager.FindObjectByName("player");

	ThrowHead = objectManager.FindObjectByName("ThrowHead");

	ComputeVectorandAngle();	//Get the vector, the angle and rotate the tongue
	Owner->m_transform->RotateBy(Angle);

	CreateTongueParts();

	if (ThrowHead != nullptr)
	{
		Begginning->m_transform->mPosition = ThrowHead->m_transform->GetPos() + DirectionVector * 20.0f;
		Ending->m_transform->mPosition.x = Begginning->m_transform->mPosition.x + InitialLength;
		Ending->m_transform->mPosition.y = Begginning->m_transform->mPosition.y;
		MidPart->m_transform->mPosition = ComputeMidpoint(Begginning->m_transform->mPosition, Ending->m_transform->mPosition);

		StartingMidPartSize = MidPart->m_transform->mScale;
		MidPartMinSize = StartingMidPartSize * 0.2f;
		MidPartMaxSize = StartingMidPartSize * 1.2f;
		StartingMidPartPos = MidPart->m_transform->mPosition;
		Ending->m_transform->mScale /= 2.0f;
	}




	using namespace Messaging;
	Messaging::EventHandler* temp = &Owner->GetEventHandler();

	Owner->GetEventHandler().register_handler(this, &TongueComponent::collisionStarted);
	Owner->GetEventHandler().register_handler(this, &TongueComponent::collisionPersisted);
	Owner->GetEventHandler().register_handler(this, &TongueComponent::collisionEnded);

	////subscribe this handler to this type of event
	Messaging::EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionStarted>());
	Messaging::EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionPersisted>());
	Messaging::EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionEnded>());
}

void TongueComponent::CreateTongueParts()
{
	Begginning = ConfigureObjSettings(objectManager.AddGameObject("TongueBegginning"), "BEGGING.png", { 500.0f, 500.0f });
	Begginning->m_transform->mScale = { 40.0f, 40.0f };
	Begginning->m_transform->mZorder = 11.3f;

	MidPart = ConfigureObjSettings(objectManager.AddGameObject("TongueMidPart"), "MIDPART.png", { 500.0f, 500.0f });
	MidPart->m_transform->mZorder = 11.5f;

	Ending = ConfigureObjSettings(objectManager.AddGameObject("TongueEnding"), "ENDING.png", { 500.0f, 500.0f });
	Ending->m_transform->mZorder = 11.3f;

	///Outline = objectManager.AddGameObject("TongueOutline");
	///Outline->m_transform->mZorder = 1.2f;
	///
	///Outline->EditableByGizmos = false;
	///Outline->m_transform->SetScale(MidPart->m_transform->GetScale());
	///auto* rend = Outline->AddComponent<Graphics::Renderable>();
	///rend->pTransform->SetPos(MidPart->m_transform->GetPos());
	///rend->mColor = { 0,0,0,1 };

	//Outline->AddComponent<Collider>();
	//auto * collider = Outline->FindComponentByType<Collider>();
	//collider->addCollisionGroup(CollisionGroup::GetCollisionGroup("TongueCollisionGroup"));

	///Outline->AddComponent<RigidBody>();
	///auto * rigidbody = Outline->FindComponentByType<RigidBody>();
	///rigidbody->setAffectedByGravity(false);
	///rigidbody->isStatic = true;
	///rigidbody->mInvMass = 0.0f;
}

GameObject * TongueComponent::ConfigureObjSettings(GameObject * Object, const char * TextureName, glm::vec2 Position, glm::vec2 Size)
{
	Object->EditableByGizmos = false;
	Object->m_transform->SetScale(Size);
	auto* rend = Object->AddComponent<Graphics::Renderable>();
	rend->pTransform->SetPos(Position);
	rend->pTexture = ResourceManager::textures[TextureName].get();

	//Object->AddComponent<Collider>();
	//auto * collider = Object->FindComponentByType<Collider>();
	//collider->addCollisionGroup(CollisionGroup::GetCollisionGroup("TongueCollisionGroup"));

	Object->AddComponent<RigidBody>();
	auto * rigidbody = Object->FindComponentByType<RigidBody>();
	rigidbody->setAffectedByGravity(false);
	rigidbody->isStatic = true;
	rigidbody->mInvMass = 0.0f;

	return Object;
}

int counter = 0;
void TongueComponent::UpdateVectorAndAngle()
{
	//Compute the vector
	DirectionVector = glm::normalize(ClickPosition - ThrowHead->m_transform->GetPos());

	//Compute height and width
	float h = ClickPosition.y - ThrowHead->m_transform->GetPos().y;
	float w = ClickPosition.x - ThrowHead->m_transform->GetPos().x;

	Angle = glm::atan(h / w);	//Compute angle

	if (ClickPosition.x < ThrowHead->m_transform->GetPos().x && alreadycorrectedflip == false)	//In case the angle is in the 1st or 3rd quadrant
	{															//rotate the object by 180 degrees so that the texture matches
																//Angle += glm::radians(180.0f);
		ThrowHead->FindComponentByType<Graphics::Renderable>()->Flip();
		alreadycorrectedflip = true;

		Begginning->FindComponentByType<Graphics::Renderable>()->Flip();
		MidPart->FindComponentByType<Graphics::Renderable>()->Flip();
		Ending->FindComponentByType<Graphics::Renderable>()->Flip();
		//std::cout << "Flip counter = " << ++counter << std::endl;
	}
}

void TongueComponent::Increase_DecreaseTongue()
{
	if (ThrowHead == nullptr)
		return;

	Begginning->m_transform->mPosition = ThrowHead->m_transform->mPosition + DirectionVector * 20.0f; //The beginning of the tongue will always start from the  head

	UpdateVectorAndAngle();	//Compute again the angle 
	UpdateAngleOfObject(Begginning);
	UpdateAngleOfObject(MidPart);
	UpdateAngleOfObject(Ending);
	///UpdateAngleOfObject(Outline);

	if ((Length < MaxLength) && !Return)
	{
		if (PropulsionObjCollidedWith != nullptr)
		{
			Return = true;
			return;
		}

		Length += IncreaseSizeSpeed;

		Ending->m_transform->mPosition = ThrowHead->m_transform->GetPos() + DirectionVector * Length;

		//Update the position of the tongue												
		MidPart->m_transform->mPosition = ComputeMidpoint(Begginning->m_transform->GetPos(), Ending->m_transform->GetPos());

		MidPart->m_transform->mScale.x = (glm::distance(Begginning->m_transform->GetPos(), Ending->m_transform->GetPos()));

		MidPart->m_transform->mScale.y = StartingMidPartSize.y - (StartingMidPartSize.y * (Length / MaxLength));

		if (MidPart->m_transform->mScale.y < StartingMidPartSize.y * 0.25f)
		{
			MidPart->m_transform->mScale.y = MidPartMinSize.y;
		}

		Owner->m_transform->mAngle = MidPart->m_transform->mAngle;
		Owner->m_transform->mPosition = MidPart->m_transform->mPosition;
		Owner->m_transform->mScale.y = MidPart->m_transform->mScale.y * 0.5f;
		Owner->m_transform->mScale.x = MidPart->m_transform->mScale.x + (Begginning->m_transform->mScale.x / 2.0f) + (Ending->m_transform->mScale.x / 2.0f);
	}
	else
	{
		if (Return && !UpdatedMaxLength)
		{
			MaxLength = glm::distance(Begginning->m_transform->mPosition, Ending->m_transform->GetPos());
			UpdatedMaxLength = true;
		}

		if (PropulsionObjCollidedWith != nullptr && glm::distance(PropulsionObjCollidedWith->m_transform->mPosition, Playerptr->m_transform->mPosition) < 50.0f)
		{
			Playerptr->FindComponentByType<HeadComponent>()->ReadyToRotHead = false;
			DestroyAllObjects();  //Destroy tongue
			return;
		}

		Length += IncreaseSizeSpeed;

		Ending->m_transform->mPosition = Begginning->m_transform->mPosition + DirectionVector * (2.0f * MaxLength - Length);
		//Update the position of the tongue												
		MidPart->m_transform->mPosition = ComputeMidpoint(Begginning->m_transform->GetPos(), Ending->m_transform->GetPos());

		MidPart->m_transform->mScale.x = (glm::distance(Begginning->m_transform->GetPos(), Ending->m_transform->GetPos()));

		MidPart->m_transform->mScale.y = MidPartMinSize.y + (MidPartMinSize.y * ((Length - InitialLength) / MaxLength));

		if (MidPart->m_transform->mScale.y > StartingMidPartSize.y * 1.25f)
		{
			MidPart->m_transform->mScale.y = MidPartMaxSize.y;
		}

		///Outline->m_transform->mScale.y = MidPart->m_transform->mScale.y + 0.5f;
		///Outline->m_transform->mScale.x = MidPart->m_transform->mScale.x;
		///Outline->m_transform->mPosition = MidPart->m_transform->mPosition;


		Owner->m_transform->mAngle = MidPart->m_transform->mAngle;
		Owner->m_transform->mPosition = MidPart->m_transform->mPosition;
		Owner->m_transform->mScale.y = MidPart->m_transform->mScale.y * 0.5f;
		Owner->m_transform->mScale.x = MidPart->m_transform->mScale.x + (Begginning->m_transform->mScale.x / 2.0f) + (Ending->m_transform->mScale.x / 2.0f);

		if ((Length > MaxLength * 2.0f) || MidPart->m_transform->mScale.x < 30.0f)
		{
			if (Playerptr == nullptr)
				return;

			if (!Playerptr->FindComponentByType<HeadComponent>())
				return;

			Playerptr->FindComponentByType<HeadComponent>()->ReadyToRotHead = false;
			ThrowHead->FindComponentByType<AnimComp>()->mColor[3] = 0.0f;
			//DestroyAllObjects();  //Destroy when fully compacted

			DeleteTongue = true;
		}
	}
}

float TongueComponent::QuadraticEaseIn(float currenttime, float start, float cahngeinvalue, float duration)
{
	currenttime /= duration;
	return cahngeinvalue*currenttime*currenttime + start;
}

float TongueComponent::Lerp(float start, float end, float t)
{
	return (1.0f - t) * start + t * end;
}

void TongueComponent::UpdateAngleOfObject(GameObject * Object)
{
	Object->m_transform->mAngle = 0.0f;
	Object->m_transform->RotateBy(Angle);
}

glm::vec2 TongueComponent::ComputeMidpoint(glm::vec2 InitialPos, glm::vec2 EndPos)	//Compute midpoint betwwen two points
{
	return ((InitialPos + EndPos) / 2.0f);
}

void TongueComponent::ComputeVectorandAngle()
{
	if (ThrowHead == nullptr)return;

	glm::vec2 mouseWorldPosition = Input::Instance()->ScreenToWorld();	//Position of mouse

	ClickPosition = mouseWorldPosition;

	//Compute the vector
	DirectionVector = glm::normalize(mouseWorldPosition - ThrowHead->m_transform->GetPos());

	//Compute height and width
	float h = mouseWorldPosition.y - ThrowHead->m_transform->GetPos().y;
	float w = mouseWorldPosition.x - ThrowHead->m_transform->GetPos().x;

	Angle = glm::atan(h / w);	//Compute angle
}

void TongueComponent::collisionStarted(const OnCollisionStarted& mEvent)
{
	if (mEvent.mData.eitherObjectHasACollisionGroupOf(CollisionGroup::GetCollisionGroup("pipeCollisionGroup")))
	{
		Return = true;
		return;
	}

	if (mEvent.mData.obj1->getCollisionGroup() != CollisionGroup::GetCollisionGroup("PropulsionObj") && mEvent.mData.obj2->getCollisionGroup() != CollisionGroup::GetCollisionGroup("PropulsionObj"))
	{
		this->Return = true;
	}
	if (mEvent.mData.obj1->getCollisionGroup() == CollisionGroup::GetCollisionGroup("PropulsionObj") || mEvent.mData.obj1->getCollisionGroup() == CollisionGroup::GetCollisionGroup("ClimbableObj"))
	{
		PropulsionObjCollidedWith = mEvent.mData.obj1->GetOwner();

		StateMachine * PlayerSM = Playerptr->FindComponentByType<StateMachine>();

		if (PlayerSM->mCurrentState == PlayerSM->GetState<PlayerSM::HorizontalClimb_State>() && PropulsionObjCollidedWith == PlayerSM->GetState<PlayerSM::HorizontalClimb_State>()->WallClimbing)
		{
			Return = true;
			return;
		}
		else if (PlayerSM->mCurrentState == PlayerSM->GetState<PlayerSM::VerticalClimb_State>() && PropulsionObjCollidedWith == PlayerSM->GetState<PlayerSM::VerticalClimb_State>()->WallClimbing)
		{
			Return = true;
			return;
		}


		CollisionPoint = mEvent.mData.mContactData.mPi;
		Return = true;

		PlayerSM->ChangeState<PlayerSM::Propulsed_State>();

	}
	else if (mEvent.mData.obj2->getCollisionGroup() == CollisionGroup::GetCollisionGroup("PropulsionObj") || mEvent.mData.obj2->getCollisionGroup() == CollisionGroup::GetCollisionGroup("ClimbableObj"))
	{
		PropulsionObjCollidedWith = mEvent.mData.obj2->GetOwner();

		StateMachine * PlayerSM = Playerptr->FindComponentByType<StateMachine>();

		if (PlayerSM->mCurrentState == PlayerSM->GetState<PlayerSM::HorizontalClimb_State>() && PropulsionObjCollidedWith == PlayerSM->GetState<PlayerSM::HorizontalClimb_State>()->WallClimbing)
		{
			Return = true;
			return;
		}
		else if (PlayerSM->mCurrentState == PlayerSM->GetState<PlayerSM::VerticalClimb_State>() && PropulsionObjCollidedWith == PlayerSM->GetState<PlayerSM::VerticalClimb_State>()->WallClimbing)
		{
			Return = true;
			return;
		}

		CollisionPoint = mEvent.mData.mContactData.mPi;

		PlayerSM->ChangeState<PlayerSM::Propulsed_State>();
	}
	if (mEvent.mData.obj1->getCollisionGroup() == CollisionGroup::GetCollisionGroup("FlyCollisionGroup"))
	{
		mEvent.mData.obj1->GetOwner()->FindComponentByType<FlyComponent>()->Stop = true;
		FlyCatched = mEvent.mData.obj1->GetOwner();
	}
	if (mEvent.mData.obj2->getCollisionGroup() == CollisionGroup::GetCollisionGroup("FlyCollisionGroup"))
	{
		mEvent.mData.obj2->GetOwner()->FindComponentByType<FlyComponent>()->Stop = true;
		FlyCatched = mEvent.mData.obj2->GetOwner();
	}
	if (mEvent.mData.obj1->getCollisionGroup() == CollisionGroup::GetCollisionGroup("CamouflageFlyCollisionGroup"))
	{
		FlyCatched = mEvent.mData.obj1->GetOwner();
	}
	if (mEvent.mData.obj2->getCollisionGroup() == CollisionGroup::GetCollisionGroup("CamouflageFlyCollisionGroup"))
	{
		FlyCatched = mEvent.mData.obj2->GetOwner();
	}
}
void TongueComponent::collisionPersisted(const OnCollisionPersisted& mEvent)
{
	if (mEvent.mData.obj1->getCollisionGroup() == CollisionGroup::GetCollisionGroup("PropulsionObj") || mEvent.mData.obj1->getCollisionGroup() == CollisionGroup::GetCollisionGroup("ClimbableObj"))
	{
		PropulsionObjCollidedWith = mEvent.mData.obj1->GetOwner();

		StateMachine * PlayerSM = Playerptr->FindComponentByType<StateMachine>();

		if (PlayerSM->mCurrentState == PlayerSM->GetState<PlayerSM::HorizontalClimb_State>() && PropulsionObjCollidedWith == PlayerSM->GetState<PlayerSM::HorizontalClimb_State>()->WallClimbing)
		{
			Return = true;
			return;
		}
		else if (PlayerSM->mCurrentState == PlayerSM->GetState<PlayerSM::VerticalClimb_State>() && PropulsionObjCollidedWith == PlayerSM->GetState<PlayerSM::VerticalClimb_State>()->WallClimbing)
		{
			Return = true;
			return;
		}
	}
	else if (mEvent.mData.obj2->getCollisionGroup() == CollisionGroup::GetCollisionGroup("PropulsionObj") || mEvent.mData.obj2->getCollisionGroup() == CollisionGroup::GetCollisionGroup("ClimbableObj"))
	{
		PropulsionObjCollidedWith = mEvent.mData.obj2->GetOwner();

		StateMachine * PlayerSM = Playerptr->FindComponentByType<StateMachine>();

		if (PlayerSM->mCurrentState == PlayerSM->GetState<PlayerSM::HorizontalClimb_State>() && PropulsionObjCollidedWith == PlayerSM->GetState<PlayerSM::HorizontalClimb_State>()->WallClimbing)
		{
			Return = true;
			return;
		}
		else if (PlayerSM->mCurrentState == PlayerSM->GetState<PlayerSM::VerticalClimb_State>() && PropulsionObjCollidedWith == PlayerSM->GetState<PlayerSM::VerticalClimb_State>()->WallClimbing)
		{
			Return = true;
			return;
		}
	}
}
void TongueComponent::collisionEnded(const OnCollisionEnded&)
{}

glm::vec2 TongueComponent::GetDirVec()
{
	return DirectionVector;
}

void TongueComponent::DestroyAllObjects()
{
	Begginning->Destroy();
	MidPart->Destroy();
	Ending->Destroy();
	///Outline->Destroy();
	Owner->Destroy();
}

void TongueComponent::GotAFly()
{
	if (FlyCatched == nullptr)
	{
		return;
	}

	if (glm::distance(FlyCatched->m_transform->mPosition, Playerptr->m_transform->mPosition) < 190.0f)
	{
		//Flies get eaten and destroyed
		Playerptr->FindComponentByType<CamouflageComponent>()->AteFly();

		FlyCatched->Destroy();
	}

	FlyCatched->m_transform->mPosition = Ending->m_transform->mPosition;
}
