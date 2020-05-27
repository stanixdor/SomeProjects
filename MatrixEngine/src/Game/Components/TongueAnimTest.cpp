#include "Components\TongueAnimTest.h"
#include "Physics\RigidBody.h"
#include "Engine\Physics\CollisionEvents\OnCollisionEvent.h"
#include "Engine/Physics/CollisionSystem.h"
#include "Engine/Graphics/GraphicsSystem.h"
#include "Physics\CollisionTable.h"
#include "Platform\Resource Manager\ResourceManager.h"
#include "Levels\LevelTest.h"
#include "GLM/glm.hpp"
#include "imgui\imgui.h"
#include "Editor\ImGuiUpdate.h"

GameObject * ConfigureObjSettings(GameObject * Object, const char * TextureName, glm::vec2 Position, glm::vec2 Size);

TestTongue::TestTongue() : Component("TongueANIM")
{
	m_name = std::string("TongueANIM");
}

void TestTongue::Initialize()
{
	CreateTongueParts();
	StartingMidPartSize = MidPart->m_transform->mScale;
	MidPartMinSize = StartingMidPartSize * 0.2f;
	StartingMidPartPos = MidPart->m_transform->mPosition;
	Ending->m_transform->mScale /= 2.0f;
}
void TestTongue::CreateTongueParts()
{
	Begginning = ConfigureObjSettings(objectManager.AddGameObject("TongueBegginning"), "BEGGING.png", { 0.0f, 100.0f }, { 40.0f, 20.0f });
	MidPart = ConfigureObjSettings(objectManager.AddGameObject("TongueMidPart"), "MIDPART.png", { 30.0f, 100.0f }, { 60.0f, 10.0f });
	Ending = ConfigureObjSettings(objectManager.AddGameObject("TongueEnding"), "ENDING.png", { 60.0f, 100.0f }, { 40.0f, 20.0f });

	Ending->m_transform->mZorder = 0.5f;

	Outline = objectManager.AddGameObject("TongueOutline");

	Outline->EditableByGizmos = false;
	Outline->m_transform->SetScale(MidPart->m_transform->GetScale());
	auto* rend = Outline->AddComponent<Graphics::Renderable>();
	rend->pTransform->SetPos(MidPart->m_transform->GetPos());
	rend->mColor = { 0,0,0,1 };

	Outline->AddComponent<RigidBody>();
	auto * rigidbody = Outline->FindComponentByType<RigidBody>();
	rigidbody->setAffectedByGravity(false);

	rigidbody->addCollisionGroup(CollisionGroup::GetCollisionGroup("TongueCollisionGroup"));
	rigidbody->isStatic = true;
	rigidbody->mInvMass = 0.0f;

	Outline->m_transform->mZorder = 0.3f;
}

GameObject * ConfigureObjSettings(GameObject * Object, const char * TextureName, glm::vec2 Position, glm::vec2 Size)
{
	Object->EditableByGizmos = false;
	Object->m_transform->SetScale(Size);
	auto* rend = Object->AddComponent<Graphics::Renderable>();
	rend->pTransform->SetPos(Position);
	rend->pTexture = ResourceManager::textures[TextureName].get();

	Object->AddComponent<RigidBody>();
	auto * rigidbody = Object->FindComponentByType<RigidBody>();
	rigidbody->setAffectedByGravity(false);

	rigidbody->addCollisionGroup(CollisionGroup::GetCollisionGroup("TongueCollisionGroup"));
	rigidbody->isStatic = true;
	rigidbody->mInvMass = 0.0f;

	return Object;
}

void TestTongue::Update()
{
	if (Length != EditorHacks::Length)
	{
		Ending->m_transform->mPosition.x = Begginning->m_transform->mPosition.x + EditorHacks::Length;
		Length = EditorHacks::Length;
	}

	MidPart->m_transform->mPosition = ComputeMidpoint(Begginning->m_transform->GetPos(), Ending->m_transform->GetPos());
	
	//ScaleOffset = 1.2f; //11.5f
	MidPart->m_transform->mScale.x = (glm::distance(Begginning->m_transform->GetPos(), Ending->m_transform->GetPos())) /*+ ScaleOffset*/;


	MidPart->m_transform->mScale.y = StartingMidPartSize.y - (StartingMidPartSize.y * (Length / MaxLength) * 0.5f);
	
	if (MidPart->m_transform->mScale.y < StartingMidPartSize.y * 0.25f)
	{
		MidPart->m_transform->mScale.y = MidPartMinSize.y;
	}

	Outline->m_transform->mScale.y = MidPart->m_transform->mScale.y + 1.0f;
	Outline->m_transform->mScale.x = MidPart->m_transform->mScale.x;
	Outline->m_transform->mPosition = MidPart->m_transform->mPosition;
}

glm::vec2 TestTongue::ComputeMidpoint(glm::vec2 InitialPos, glm::vec2 EndPos)	//Compute midpoint betwwen two points
{
	return ((InitialPos + EndPos) / 2.0f);
}