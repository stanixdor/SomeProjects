#include "Levels\LevelTest.h"
#include "Levels\GenericLevel.h"
#include "Engine/Platform/Resource Manager/ResourceManager.h"
#include "Components\HeadComponent.h"
#include "Physics/Collider.h"
#include "Engine\Graphics\GraphicsSystem.h"
#include "Game\EnemiesStates\SheepStates.h"
#include "Game\EnemiesStates\WolfStates.h"


// GAME OBJECTS
//std::unique_ptr<GameObject> player = std::make_unique<GameObject>();
Graphics::AnimComp *playerRend = nullptr;
GameObject* box = nullptr;
GameObject* box2 = nullptr;
GameObject * sheep = nullptr;
GameObject * sloth = nullptr;
GameObject * detector = nullptr;
GameObject* PropulsionObj = nullptr;
GameObject* ClimbingObj = nullptr;
GameObject* ZClimbingObj = nullptr;
GameObject* CheckpointObj = nullptr;
GameObject* LeverObj = nullptr;
GameObject* LaserObj = nullptr;

//---------------------------------------------------------------------------/
// ADD ENEMY 																 /
//---------------------------------------------------------------------------/
GameObject* AddSloth(glm::vec2 pos)
{
	auto* object = objectManager.AddGameObject("Sloth");
	object->m_transform->SetPos(pos);
	object->m_transform->SetScale(glm::vec2(40, 40));


	//auto * slothRend = object->AddComponent<Graphics::AnimComp>();

	//StateMachine * slothMachine = object->AddComponent<SlothStateMachine>();

	auto* slothRigidBody = object->AddComponent<RigidBody>();
	//auto* slothCollider = object->AddComponent<Collider>();

	slothRigidBody->setAffectedByGravity(true);
	slothRigidBody->isStatic = true;
	//slothRigidbody->mInvMass = 0.0f;

	return object;

}


//---------------------------------------------------------------------------/
// ADD DETECTOR FOR THE ENEMY											     /
//---------------------------------------------------------------------------/
GameObject* AddDetector(glm::vec2 pos)
{
	auto* object = objectManager.AddGameObject("Detector");

	object->m_transform->SetScale(glm::vec2(150, 50));
	auto* rend = object->AddComponent<Graphics::Renderable>();
	rend->pMesh = GraphicsSystem::Instance()->pMesh;
	rend->pTexture = ResourceManager::textures["Select.png"].get();
	rend->pTransform->SetPos(pos);
	rend->pShader = GraphicsSystem::Instance()->mPrograms[0];
	rend->pTransform->mZorder = 0.5f;

	GraphicsSystem::Instance()->mSpriteLayer[0]->AddRenderable(rend);

	object->AddComponent<RigidBody>();

	auto * rigidbody = object->FindComponentByType<RigidBody>();
	rigidbody->setAffectedByGravity(false);

	rigidbody->mRestitution = -1.0f;

	//auto* detectorCollider = object->AddComponent<Collider>();


	//ADD DETECTOR COMPONENT:

	object->AddComponent<Detector>();



	return object;
}

//---------------------------------------------------------------------------/
// ADD BUTTON : EXAMPLE LEVELS, LASER ETC									 /
//---------------------------------------------------------------------------/

GameObject * AddActivable(glm::vec2 pos)
{
	auto * object = objectManager.AddGameObject("button");
	object->m_transform->SetScale(glm::vec2(20, 20));
	auto* rend = object->AddComponent<Graphics::Renderable>();
	rend->pMesh = GraphicsSystem::Instance()->pMesh;
	rend->pTexture = ResourceManager::textures["Palanca off.png"].get();

	rend->pTransform->SetPos(pos);
	rend->pShader = GraphicsSystem::Instance()->mPrograms[0];

	GraphicsSystem::Instance()->mSpriteLayer[0]->AddRenderable(rend);
	object->AddComponent<RigidBody>();
	auto * rigidbody = object->FindComponentByType<RigidBody>();

	//auto* activableCollider = object->AddComponent<Collider>();


	//add activable component
	object->AddComponent<Activable>();

	rigidbody->setAffectedByGravity(false);
	rigidbody->mRestitution = -1.0f;

	return object;

}

GameObject* AddLaser(glm::vec2 pos)
{
	auto* object = objectManager.AddGameObject("laser");
	object->m_transform->SetScale(glm::vec2(20, 20));
	auto* rend = object->AddComponent<Graphics::Renderable>();
	rend->pMesh = GraphicsSystem::Instance()->pMesh;
	rend->pTexture = ResourceManager::textures["Plataforma pinchos.png"].get();

	rend->pTransform->SetPos(pos);
	rend->pShader = GraphicsSystem::Instance()->mPrograms[0];

	GraphicsSystem::Instance()->mSpriteLayer[0]->AddRenderable(rend);
	object->AddComponent<RigidBody>();
	auto * rigidbody = object->FindComponentByType<RigidBody>();
	//add activable component

	//auto* LaserCollider = object->AddComponent<Collider>();

	object->AddComponent<Laser>();

	rigidbody->setAffectedByGravity(false);

	rigidbody->mRestitution = -1.0f;

	return object;
}

//---------------------------------------------------------------------------/
// ADD CHECKPOINT															 /
//---------------------------------------------------------------------------/

GameObject* AddCheckpoint(glm::vec2 pos)
{
	auto* object = objectManager.AddGameObject("Checkpoint");
	object->m_transform->SetScale(glm::vec2(50, 50));
	auto* rend = object->AddComponent<Graphics::Renderable>();
	rend->pMesh = GraphicsSystem::Instance()->pMesh;
	rend->pTexture = ResourceManager::textures["Bidon.png"].get();
	rend->pTransform->SetPos(pos);
	rend->pShader = GraphicsSystem::Instance()->mPrograms[0];
	rend->pTransform->mZorder = 0.5f;

	GraphicsSystem::Instance()->mSpriteLayer[0]->AddRenderable(rend);

	object->AddComponent<RigidBody>();

	auto * rigidbody = object->FindComponentByType<RigidBody>();
	rigidbody->setAffectedByGravity(false);


	//CollisionSystem::Instance()->AddRigidBody(*rigidbody, false);

	rigidbody->mRestitution = -1.0f;

	//auto * collider = object->AddComponent<Collider>();

	// ADD THE CHECKPOINT COMPONENT
	object->AddComponent<Checkpoint>();

	return object;
}






void LevelTest::Initialize()
{

	// COLLSIONS GROUPS //

	CollisionGroup::CreateCollisionGroup("default");
	CollisionGroup floorCollision = CollisionGroup::CreateCollisionGroup("Floor");
	CollisionGroup playerCollision = CollisionGroup::CreateCollisionGroup("Player");
	CollisionGroup checkpointCollision = CollisionGroup::CreateCollisionGroup("Checkpoint");
	CollisionGroup DetectCollision = CollisionGroup::CreateCollisionGroup("Detector");
	CollisionGroup ClimbableGroup = CollisionGroup::CreateCollisionGroup("ClimbableObj");
	CollisionGroup ZClimbableGroup = CollisionGroup::CreateCollisionGroup("ZClimbableObj");
	CollisionGroup PropulsionObjCollision = CollisionGroup::CreateCollisionGroup("PropulsionObj");
	CollisionGroup TongueCollisionGroup = CollisionGroup::CreateCollisionGroup("TongueCollisionGroup");
	CollisionGroup SheepCollisionGroup = CollisionGroup::CreateCollisionGroup("SheepCollisionGroup");
	CollisionGroup SlothCollisionGroup = CollisionGroup::CreateCollisionGroup("SlothCollisionGroup");
	CollisionGroup ActivableCollision = CollisionGroup::CreateCollisionGroup("activable");
	CollisionGroup HazardCollision = CollisionGroup::CreateCollisionGroup("hazard");
	CollisionGroup FlyCollisionGroup = CollisionGroup::CreateCollisionGroup("FlyCollisionGroup");
	CollisionGroup LaserCollisionGroup = CollisionGroup::CreateCollisionGroup("LaserCollisionGroup");
	CollisionGroup SteamCollisionGroup = CollisionGroup::CreateCollisionGroup("SteamCollisionGroup");
	CollisionGroup DoorCollisionGroup = CollisionGroup::CreateCollisionGroup("DoorCollisionGroup");
	CollisionGroup CamouflageFlyCollisionGroup = CollisionGroup::CreateCollisionGroup("CamouflageFlyCollisionGroup");
	CollisionGroup CollectibleCollisionGroup = CollisionGroup::CreateCollisionGroup("CollectibleCollisionGroup");
	CollisionGroup pipeCollisionGroup = CollisionGroup::CreateCollisionGroup("PipeCollisionGroup");
	CollisionGroup fakeTongueCollisionGroup = CollisionGroup::CreateCollisionGroup("FakeTongueCollisionGroup");



	CollisionTable::Instance()->AddPolicy(CollisionGroup::GetCollisionGroup("Player").getId(), CollisionGroup::GetCollisionGroup("CamouflageFlyCollisionGroup").getId(), CollisionPolicy::SkipDetection);

	CollisionTable::Instance()->AddPolicy(CollisionGroup::GetCollisionGroup("Player").getId(), CollisionGroup::GetCollisionGroup("PropulsionObj").getId(), CollisionPolicy::SkipDetection);
	CollisionTable::Instance()->AddPolicy(CollisionGroup::GetCollisionGroup("Player").getId(), CollisionGroup::GetCollisionGroup("Player").getId(), CollisionPolicy::SkipDetection);
	CollisionTable::Instance()->AddPolicy(CollisionGroup::GetCollisionGroup("Player").getId(), CollisionGroup::GetCollisionGroup("TongueCollisionGroup").getId(), CollisionPolicy::SkipDetection);

	CollisionTable::Instance()->AddPolicy(CollisionGroup::GetCollisionGroup("ZClimbableObj").getId(), CollisionGroup::GetCollisionGroup("Player").getId(), CollisionPolicy::SkipResolution);
	CollisionTable::Instance()->AddPolicy(CollisionGroup::GetCollisionGroup("Player").getId(), CollisionGroup::GetCollisionGroup("LaserCollisionGroup").getId(), CollisionPolicy::SkipResolution);
	//Tongue collision group
	CollisionTable::Instance()->AddPolicy(CollisionGroup::GetCollisionGroup("TongueCollisionGroup").getId(), CollisionGroup::GetCollisionGroup("TongueCollisionGroup").getId(), CollisionPolicy::SkipDetection);
	CollisionTable::Instance()->AddPolicy(CollisionGroup::GetCollisionGroup("TongueCollisionGroup").getId(), CollisionGroup::GetCollisionGroup("PipeCollisionGroup").getId(), CollisionPolicy::SkipResolution);
	
	CollisionTable::Instance()->AddPolicy(CollisionGroup::GetCollisionGroup("FakeTongueCollisionGroup").getId(), CollisionGroup::GetCollisionGroup("PipeCollisionGroup").getId(), CollisionPolicy::SkipDetection);
	CollisionTable::Instance()->AddPolicy(CollisionGroup::GetCollisionGroup("FakeTongueCollisionGroup").getId(), CollisionGroup::GetCollisionGroup("CamouflageFlyCollisionGroup").getId(), CollisionPolicy::SkipResolution);
	CollisionTable::Instance()->AddPolicy(CollisionGroup::GetCollisionGroup("FakeTongueCollisionGroup").getId(), CollisionGroup::GetCollisionGroup("activable").getId(), CollisionPolicy::SkipResolution);

	CollisionTable::Instance()->AddPolicy(CollisionGroup::GetCollisionGroup("CamouflageFlyCollisionGroup").getId(), CollisionGroup::GetCollisionGroup("pipeCollisionGroup").getId(), CollisionPolicy::SkipResolution);
	CollisionTable::Instance()->AddPolicy(CollisionGroup::GetCollisionGroup("CamouflageFlyCollisionGroup").getId(), CollisionGroup::GetCollisionGroup("Player").getId(), CollisionPolicy::SkipDetection);


	CollisionTable::Instance()->AddPolicy(CollisionGroup::GetCollisionGroup("Detector").getId(), CollisionGroup::GetCollisionGroup("TongueCollisionGroup").getId(), CollisionPolicy::SkipDetection);


	CollisionTable::Instance()->AddPolicy(CollisionGroup::GetCollisionGroup("TongueCollisionGroup").getId(), CollisionGroup::GetCollisionGroup("CollectibleCollisionGroup").getId(), CollisionPolicy::Resolve);
	//player enemies collision group
	CollisionTable::Instance()->AddPolicy(CollisionGroup::GetCollisionGroup("Player").getId(), CollisionGroup::GetCollisionGroup("SheepCollisionGroup").getId(), CollisionPolicy::SkipResolution);
	CollisionTable::Instance()->AddPolicy(CollisionGroup::GetCollisionGroup("Player").getId(), CollisionGroup::GetCollisionGroup("SlothCollisionGroup").getId(), CollisionPolicy::Resolve);

	//Enemy-Detector collision group
	CollisionTable::Instance()->AddPolicy(CollisionGroup::GetCollisionGroup("Player").getId(), CollisionGroup::GetCollisionGroup("Detector").getId(), CollisionPolicy::SkipResolution);
	CollisionTable::Instance()->AddPolicy(CollisionGroup::GetCollisionGroup("SlothCollisionGroup").getId(), CollisionGroup::GetCollisionGroup("Detector").getId(), CollisionPolicy::SkipDetection);
	CollisionTable::Instance()->AddPolicy(CollisionGroup::GetCollisionGroup("floor").getId(), CollisionGroup::GetCollisionGroup("Detector").getId(), CollisionPolicy::SkipDetection);

	//player checkpoint collision group
	CollisionTable::Instance()->AddPolicy(CollisionGroup::GetCollisionGroup("Checkpoint").getId(), CollisionGroup::GetCollisionGroup("Player").getId(), CollisionPolicy::SkipResolution);

	// player - activable objects:
	CollisionTable::Instance()->AddPolicy(CollisionGroup::GetCollisionGroup("activable").getId(), CollisionGroup::GetCollisionGroup("Player").getId(), CollisionPolicy::SkipResolution);
	CollisionTable::Instance()->AddPolicy(CollisionGroup::GetCollisionGroup("DoorCollisionGroup").getId(), CollisionGroup::GetCollisionGroup("Player").getId(), CollisionPolicy::Resolve);

	// player - hazards object collision groups : lasers, lava etc

	CollisionTable::Instance()->AddPolicy(CollisionGroup::GetCollisionGroup("hazard").getId(), CollisionGroup::GetCollisionGroup("Player").getId(), CollisionPolicy::SkipResolution);
	CollisionTable::Instance()->AddPolicy(CollisionGroup::GetCollisionGroup("SteamCollisionGroup").getId(), CollisionGroup::GetCollisionGroup("Player").getId(), CollisionPolicy::SkipResolution);

	CollisionSystem::Instance()->Init();


	//CAMERA
	//objectManager.m_gameObjects.push_back(std::make_unique<GameObject>("camera", true));
	//objectManager.FindObjectByName("camera")->EditableByGizmos = false;
	//cam1 = objectManager.FindObjectByName("camera")->AddComponent<Graphics::Camera>();
	//GraphicsSystem::Instance()->mSpriteLayer[0]->setCamera(cam1);

	return;
	//PLAYER
	//player = std::make_unique<GameObject>("player",true);
	
	/*player = objectManager.AddGameObject("player");
	CollisionSystem::Instance()->mCollisionIterations = 2;
	player->m_transform->SetScale(glm::vec2(20, 20));
	playerRend = player->AddComponent<Graphics::AnimComp>();
	playerRend->pTransform->SetPos(glm::vec2(0, 50));
	cam1->setPlayerTrans(player->m_transform);

	StateMachine * machine = player->AddComponent<PlayerStateMachine>();


	player->AddComponent<RigidBody>();
	auto * playerCollider = player->AddComponent<Collider>();

	auto * rigidbody = player->FindComponentByType<RigidBody>();

	playerCollider->addCollisionGroup(playerCollision);
	rigidbody->isStatic = false;
	rigidbody->setAffectedByGravity(true);

	rigidbody->mRestitution = 0.f;



	////////////////Create Floor
	box = objectManager.AddGameObject("floor");
	box->m_transform->SetScale(glm::vec2(3000, 50));
	auto* rend = box->AddComponent<Graphics::Renderable>();
	rend->pTransform->SetPos({ 0,0 });
	//rend->pTexture = GraphicsSystem::Instance()->mTextures[4];

	box->AddComponent<RigidBody>();
	auto * boxCollider = box->AddComponent<Collider>();
	auto * rigidbody2 = box->FindComponentByType<RigidBody>();
	rigidbody2->setAffectedByGravity(false);
	rigidbody2->isStatic = true;
	rigidbody2->mInvMass = 0.0f;
	boxCollider->addCollisionGroup(floorCollision);


	//floor for enemy
	box2 = objectManager.AddGameObject("floor");
	box2->m_transform->SetScale(glm::vec2(300, 50));
	auto* rend2 = box2->AddComponent<Graphics::Renderable>();
	rend2->pTransform->SetPos({ 50,100 });
	//rend->pTexture = GraphicsSystem::Instance()->mTextures[4];

	box2->AddComponent<RigidBody>();
	auto * rigidbody3 = box2->FindComponentByType<RigidBody>();
	auto* collider2 = box2->AddComponent<Collider>();

	rigidbody3->setAffectedByGravity(false);
	rigidbody3->isStatic = true;
	rigidbody3->mInvMass = 0.0f;
	collider2->addCollisionGroup(floorCollision);

	player->AddComponent<HeadComponent>();*/

	////////////////Create Floor

	////////////////Create PropulsionObj
	/*PropulsionObj = objectManager.AddGameObject("PropulsionObj");
	PropulsionObj->m_transform->SetScale(glm::vec2(50, 20));
	auto* rend2 = PropulsionObj->AddComponent<Graphics::Renderable>();
	rend2->pTransform->SetPos({ 0, 100 });

	PropulsionObj->AddComponent<PropulsionObject>();

	PropulsionObj->AddComponent<RigidBody>();
	auto * rigidbody3 = PropulsionObj->FindComponentByType<RigidBody>();
	rigidbody3->setAffectedByGravity(false);
	rigidbody3->isStatic = true;
	rigidbody3->mInvMass = 0.0f;
	rigidbody3->addCollisionGroup(PropulsionObjCollision);

	////////////////Create PropulsionObj

	////////////////Create ClimbingObj
	ClimbingObj = objectManager.AddGameObject("ClimbingObj");
	ClimbingObj->m_transform->SetScale(glm::vec2(50, 150));
	auto* rend3 = ClimbingObj->AddComponent<Graphics::Renderable>();
	rend3->pTransform->SetPos({ 200, 100 });

	ClimbingObj->AddComponent<PropulsionObject>();
	ClimbingObj->AddComponent<ClimbableObject>();

	ClimbingObj->AddComponent<RigidBody>();
	auto * rigidbody4 = ClimbingObj->FindComponentByType<RigidBody>();
	rigidbody4->setAffectedByGravity(false);
	rigidbody4->isStatic = true;
	rigidbody4->mInvMass = 0.0f;
	rigidbody4->addCollisionGroup(ClimbableGroup);

	////////////////Create ClimbingObj

	////////////////Create ZClimbingObj
	ZClimbingObj = objectManager.AddGameObject("ZClimbingObj");
	ZClimbingObj->m_transform->SetScale(glm::vec2(50, 150));
	auto* rend4 = ZClimbingObj->AddComponent<Graphics::Renderable>();
	rend4->pTransform->SetPos({ -200, 100 });
	ZClimbingObj->AddComponent<ZClimbableObject>();

	ZClimbingObj->AddComponent<RigidBody>();
	auto * rigidbody5 = ZClimbingObj->FindComponentByType<RigidBody>();
	rigidbody5->setAffectedByGravity(false);
	rigidbody5->isStatic = true;
	rigidbody5->mInvMass = 0.0f;
	rigidbody5->addCollisionGroup(ZClimbableGroup);

	ZClimbingObj->m_transform->mZorder = 0.5f;
	////////////////Create ZClimbingObj*/

	//SHEEP ENEMY
/*
	sheep = objectManager.AddGameObject("Sheep");
	sheep->m_transform->SetScale(glm::vec2(50.f, 50.f));
	sheep->m_transform->SetPos(glm::vec2(50, 150));

	auto * sheepRend = sheep->AddComponent<Graphics::AnimComp>();

	StateMachine * sheepMachine = sheep->AddComponent<SheepStateMachine>();

	sheep->AddComponent<RigidBody>();
	auto* sheepCollider = sheep->AddComponent<Collider>();
	auto * sheepRigidbody = sheep->FindComponentByType<RigidBody>();
	sheepRigidbody->setAffectedByGravity(true);
	sheepRigidbody->isStatic = false;
	//sheepRigidbody->mInvMass = 0.0f;
	sheepCollider->addCollisionGroup(SheepCollisionGroup);

	// SLOTH ENEMY
	sloth = AddSloth({ 20, 40 });
	sloth->FindComponentByType<Collider>()->addCollisionGroup(SlothCollisionGroup);

	// DETECTOR OBJECT
	detector = AddDetector({ 20, 40 });
	detector->FindComponentByType<Collider>()->addCollisionGroup(ActivableCollision);
	detector->FindComponentByType<Detector>()->mDetectorOwner = sloth;

	// ACTIVABLE OBJECT
	LeverObj = AddActivable({ -100, 40 });

	LaserObj = AddLaser({ -200, 200 });
	LaserObj->FindComponentByType<Collider>()->addCollisionGroup(HazardCollision);

	//CHECKPOINT

	CheckpointObj = AddCheckpoint({ -400, 20 });
	CheckpointObj->FindComponentByType<Collider>()->addCollisionGroup(checkpointCollision);
	//CheckpointObj->FindComponentByType<Checkpoint>()->mDetectorOwner = player.get();



	*/
}

void LevelTest::LoadResources(const std::string & name, const std::string & folder)
{
	objectManager.LoadFromFile(name, folder);
	if (!objectManager.FindObjectByName("camera"))
	{
		objectManager.m_gameObjects.push_back(std::make_unique<GameObject>("camera", true));
		objectManager.FindObjectByName("camera")->EditableByGizmos = false;
		cam1 = objectManager.FindObjectByName("camera")->AddComponent<Graphics::Camera>();
		GraphicsSystem::Instance()->mSpriteLayer[0]->setCamera(cam1);
	}
	else
	{
		objectManager.FindObjectByName("camera")->EditableByGizmos = false;
		cam1 = objectManager.FindObjectByName("camera")->FindComponentByType<Graphics::Camera>();
		GraphicsSystem::Instance()->mSpriteLayer[0]->setCamera(cam1);
	}
}

void LevelTest::Update()
{

	//	for (auto& comp : player->m_components)
	//		comp->LateUpdate();
	//

	//	//for (auto& comp : cameraObj->m_components)
	//	//	comp->Update();
	//	//for (auto& comp : player->m_components)
	//	//	comp->Update();
	//CollisionSystem::Instance()->Update();

	if (playMode)
	{
		if (objectManager.FindObjectByName("player") != nullptr)
			cam1->setPlayerTrans(objectManager.FindObjectByName("player")->m_transform);
	}
	else
		cam1->mPlayer = nullptr;


	if (objectManager.FindObjectByName("player") != nullptr)
	{
		// example for checkpoints: in this case by key and collision with the check, trivila to pass to die.
		if (Input::Instance()->KeyTriggered('N'))
			objectManager.FindObjectByName("player")->m_transform->SetPos(Checkpoint::respawnPosition);

		if (Input::Instance()->KeyTriggered('M'))
			Checkpoint::respawnPosition = objectManager.FindObjectByName("player")->m_transform->GetPos();


	}

}

void LevelTest::UnloadResources()
{
	cameraObj.reset();
	objectManager.Destroy();
	cam1 = nullptr;
	CollisionGroup::collisionGroups.clear();
	CollisionGroup::idGenerator = 1;
	//player->Destroy();
	objectManager.DestroyDeathObjects();
	CollisionSystem::Instance()->ClearBodies();
	CollisionGroup::idGenerator = 1;
	GraphicsSystem::Instance()->clearLayers();
}

void LevelTest::ShutDown()
{

}
