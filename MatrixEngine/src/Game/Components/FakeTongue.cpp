#include "Platform\Time\FrameRateController.h"
#include "Platform\Input\Input.h"
#include "Physics\CollisionTable.h"
#include "Engine/Physics/CollisionSystem.h"
#include "Engine\Platform\EventSystem\event_dispatcher.h"
#include "Logic\StateMachine\StateMachine.h"
#include "Tongue.h"
#include "Pipe.h"
//#include "Logic\StateMachine\PlayerStateMachine.h"
#include "Levels\LevelTest.h"
#include "Components\HeadComponent.h"
#include "Platform\Resource Manager\ResourceManager.h"
#include "Physics\Collider.h"
#include "Components/FlyComponent.h"
#include "Components/CamouflageComponent.h"
#include "FakeTongue.h"

#include "../GLM/gtx/perpendicular.hpp"
#include "../GLM/gtx/matrix_transform_2d.hpp"
#include "../GLM/gtx/string_cast.hpp"

FakeTongue::FakeTongue() : Component("FakeTongue")
{
	IncreaseSizeSpeed = 70.0f;//50.0f	//Initalize the variables
	HalfIncreaseSizeSpeed = IncreaseSizeSpeed / 2.0f;
	DistanceCounter = 0.0f;
	MaxDistance = 0.2f;
	Return = false;
	MaxLength = 500.0f;
	MaxTime = 2.0f;

	m_name = std::string("FakeTongue");
}

FakeTongue::~FakeTongue()
{
	TongueComponent::paused = false;
	DestroyAllObjects();
}

void FakeTongue::Update()
{
	Increase_DecreaseTongue();
	
	if (eatenFly)
	{
		GotAFly();
	}
}

void FakeTongue::Initialize()
{
	
	TongueComponent::paused = true;
	Owner = m_GameObject;	//Save the object
	//
	//Playerptr = objectManager.FindObjectByName("player");
	//
	//PlayerHead = objectManager.FindObjectByName("PlayerHead");
	//
	ComputeVectorandAngle();	//Get the vector, the angle and rotate the tongue
	m_GameObject->m_transform->RotateBy(Angle);
	//
	//CreateTongueParts();
	//
	//Begginning->m_transform->mPosition = pipeTransform.mPosition;
	//Ending->m_transform->mPosition.x = Begginning->m_transform->mPosition.x + InitialLength;
	//Ending->m_transform->mPosition.y = Begginning->m_transform->mPosition.y;
	//MidPart->m_transform->mPosition = ComputeMidpoint(Begginning->m_transform->mPosition, Ending->m_transform->mPosition);
	//
	//StartingMidPartSize = MidPart->m_transform->mScale;
	//MidPartMinSize = StartingMidPartSize * 0.2f;
	//MidPartMaxSize = StartingMidPartSize * 1.2f;
	//StartingMidPartPos = MidPart->m_transform->mPosition;
	//Ending->m_transform->mScale /= 2.0f;
	
	using namespace Messaging;
	Messaging::EventHandler* temp = &m_GameObject->GetEventHandler();
	
	temp->register_handler(this, &FakeTongue::collisionStarted);
	//Owner->GetEventHandler().register_handler(this, &FakeTongue::collisionPersisted);
	//Owner->GetEventHandler().register_handler(this, &FakeTongue::collisionEnded);
	//
	//subscribe this handler to this type of event
	Messaging::EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionStarted>());
	//Messaging::EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionPersisted>());
	//Messaging::EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionEnded>());
}

void FakeTongue::CreateTongueParts()
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

GameObject * FakeTongue::ConfigureObjSettings(GameObject * Object, const char * TextureName, glm::vec2 Position, glm::vec2 Size)
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


void FakeTongue::UpdateVectorAndAngle()
{
	//Compute the vector
	//DirectionVector = glm::normalize(ClickPosition - pipeTransform.GetPos());
	//
	////Compute height and width
	//float h = ClickPosition.y - pipeTransform.GetPos().y;
	//float w = ClickPosition.x - pipeTransform.GetPos().x;
	//
	//Angle = glm::atan(h / w);	//Compute angle
	Angle = pipeTransform.mAngle;
								//if (ClickPosition.x < pipeTransform.GetPos().x)	//In case the angle is in the 1st or 3rd quadrant
								//{															//rotate the object by 180 degrees so that the texture matches
								//	Angle += glm::radians(180.0f);
								//}
}

void FakeTongue::placeTongue(const Transform2D & transform)
{
	Owner = transform.m_GameObject;
	
	CreateTongueParts();
	pipeTransform = transform;
	auto angle = transform.GetRotation();
	DirectionVector.x = cos(angle);
	DirectionVector.y = sin(angle);
	Begginning->m_transform->mPosition = transform.mPosition + DirectionVector * Owner->m_transform->mScale / 2.f;
	Ending->m_transform->mPosition.x = Begginning->m_transform->mPosition.x + InitialLength;
	Ending->m_transform->mPosition.y = Begginning->m_transform->mPosition.y;
	MidPart->m_transform->mPosition = ComputeMidpoint(Begginning->m_transform->mPosition, Ending->m_transform->mPosition);
	
	StartingMidPartSize = MidPart->m_transform->mScale;
	MidPartMinSize = StartingMidPartSize * 0.2f;
	MidPartMaxSize = StartingMidPartSize * 1.2f;
	StartingMidPartPos = MidPart->m_transform->mPosition;
	Ending->m_transform->mScale /= 2.0f;
	
	GetOwner()->m_transform->RotateBy(transform.GetRotation());
	glm::mat3x3 rotA = glm::rotate(glm::mat3x3(), transform.GetRotation());
	//glm::vec2 axisBx(rotB * glm::vec3(glm::vec2(abs(OBB2.GetScale().x) / 2, 0), 0));
	//DirectionVector = glm::vec2( rotA * glm::vec3(glm::vec2(1, 0), 0) );
	
	//std::cout << "angle is " << angle << std::endl;
	
}

void FakeTongue::Increase_DecreaseTongue()
{
	//Begginning->m_transform->mPosition = pipeTransform.GetPos(); //The beginning of the tongue will always start from the  head
	
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

		Ending->m_transform->mPosition = pipeTransform.GetPos() + DirectionVector * Length;

		//Update the position of the tongue												
		MidPart->m_transform->mPosition = ComputeMidpoint(Begginning->m_transform->GetPos(), Ending->m_transform->GetPos());

		MidPart->m_transform->mScale.x = (glm::distance(Begginning->m_transform->GetPos(), Ending->m_transform->GetPos()));

		MidPart->m_transform->mScale.y = StartingMidPartSize.y - (StartingMidPartSize.y * (Length / MaxLength));

		if (MidPart->m_transform->mScale.y < StartingMidPartSize.y * 0.25f)
		{
			MidPart->m_transform->mScale.y = MidPartMinSize.y;
		}

		m_GameObject->m_transform->mAngle = MidPart->m_transform->mAngle;
		m_GameObject->m_transform->mPosition = MidPart->m_transform->mPosition;
		m_GameObject->m_transform->mScale.y = MidPart->m_transform->mScale.y;
		m_GameObject->m_transform->mScale.x = MidPart->m_transform->mScale.x + (Begginning->m_transform->mScale.x / 2.0f) + (Ending->m_transform->mScale.x / 2.0f);
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


		m_GameObject->m_transform->mAngle = MidPart->m_transform->mAngle;
		m_GameObject->m_transform->mPosition = MidPart->m_transform->mPosition;
		m_GameObject->m_transform->mScale.y = MidPart->m_transform->mScale.y;
		m_GameObject->m_transform->mScale.x = MidPart->m_transform->mScale.x + (Begginning->m_transform->mScale.x / 2.0f) + (Ending->m_transform->mScale.x / 2.0f);

		if ((Length > MaxLength * 2.0f) || MidPart->m_transform->mScale.x < 30.0f)
		{
			DestroyAllObjects();  //Destroy when fully compacted
		}
	}
}

float FakeTongue::QuadraticEaseIn(float currenttime, float start, float cahngeinvalue, float duration)
{
	currenttime /= duration;
	return cahngeinvalue*currenttime*currenttime + start;
}

float FakeTongue::Lerp(float start, float end, float t)
{
	return (1.0f - t) * start + t * end;
}

void FakeTongue::UpdateAngleOfObject(GameObject * Object)
{
	Object->m_transform->mAngle = 0.0f;
	Object->m_transform->RotateBy(Angle);
}

glm::vec2 FakeTongue::ComputeMidpoint(glm::vec2 InitialPos, glm::vec2 EndPos)	//Compute midpoint betwwen two points
{
	return ((InitialPos + EndPos) / 2.0f);
}

void FakeTongue::ComputeVectorandAngle()
{
	if (PlayerHead == nullptr)
		return;

	glm::vec2 mouseWorldPosition = Input::Instance()->ScreenToWorld();	//Position of mouse

	ClickPosition = mouseWorldPosition;

	//Compute the vector
	DirectionVector = glm::normalize(mouseWorldPosition - pipeTransform.GetPos());

	//Compute height and width
	float h = mouseWorldPosition.y - pipeTransform.GetPos().y;
	float w = mouseWorldPosition.x - pipeTransform.GetPos().x;

	Angle = glm::atan(h / w);	//Compute angle
}

void FakeTongue::collisionStarted(const OnCollisionStarted& mEvent)
{
	if (mEvent.mData.obj1->getCollisionGroup() != CollisionGroup::GetCollisionGroup("PropulsionObj") && mEvent.mData.obj2->getCollisionGroup() != CollisionGroup::GetCollisionGroup("PropulsionObj"))
	{
		this->Return = true;
	}
	if (mEvent.mData.obj1->getCollisionGroup() == CollisionGroup::GetCollisionGroup("FlyCollisionGroup"))
	{
		mEvent.mData.obj1->GetOwner()->FindComponentByType<FlyComponent>()->Stop = true;
		eatenFly = (mEvent.mData.obj1->GetOwner());
	}
	if (mEvent.mData.obj2->getCollisionGroup() == CollisionGroup::GetCollisionGroup("FlyCollisionGroup"))
	{
		mEvent.mData.obj2->GetOwner()->FindComponentByType<FlyComponent>()->Stop = true;
		eatenFly = (mEvent.mData.obj2->GetOwner());
	}
	if (mEvent.mData.obj1->getCollisionGroup() == CollisionGroup::GetCollisionGroup("CamouflageFlyCollisionGroup"))
	{
		eatenFly = (mEvent.mData.obj1->GetOwner());
	}
	if (mEvent.mData.obj2->getCollisionGroup() == CollisionGroup::GetCollisionGroup("CamouflageFlyCollisionGroup"))
	{
		eatenFly = (mEvent.mData.obj2->GetOwner());
	}
}
void FakeTongue::collisionPersisted(const OnCollisionPersisted&)
{}
void FakeTongue::collisionEnded(const OnCollisionEnded&)
{}

void FakeTongue::LateUpdate()
{
	if (eatenFly == nullptr)
		return;

	if (eatenFly->m_alive == false)
		eatenFly = nullptr;
}

glm::vec2 FakeTongue::GetDirVec()
{
	return DirectionVector;
}

void FakeTongue::DestroyAllObjects()
{
	if (!destroyedAllObjects)
	{
		Begginning->Destroy();
		MidPart->Destroy();
		Ending->Destroy();
		m_GameObject->Destroy();

		destroyedAllObjects = true;
	}
}


void FakeTongue::GotAFly()
{
	if (eatenFly)
	{
		FlyComponent* flycomp = eatenFly->FindComponentByType<FlyComponent>();

		if (flycomp)
		{
			if (flycomp->teleported)
			{
				eatenFly = nullptr;
			}
			else
			{
				eatenFly->m_transform->mPosition = Ending->m_transform->mPosition;
			}
		}
	}
}