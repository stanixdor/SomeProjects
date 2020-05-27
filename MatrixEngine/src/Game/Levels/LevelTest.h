#pragma once
#include "Engine/Logic/Level.h"
#include "Engine/GameObject/GameObject.h"
#include "Components/Test.h"
#include "Engine/Graphics/GraphicsSystem.h"
#include "Engine/Graphics/Camera.h"
#include "Engine/Physics/RigidBody.h"
#include "Engine/GameObject/GameObjectManager.h"
#include "Engine/Physics/CollisionSystem.h"
#include "Components/Player.h"
#include "Engine/Graphics/GraphicsComponents/AnimationComponent.h"
#include "Engine/EngineDebug/OpenGL_error.h"
#include "Platform\MessageWindow\MessageWindow.h"
#include "Physics\CollisionTable.h"
#include "Game/Logic/StateMachines/PlayerStateMachine.h"
#include "Engine/Logic/StateMachine/StateMachine.h"
#include <iostream>
#include "Components\PropulsionObject.h"
#include "Components\MovingPlatform.h"
#include "Components/ClimbableObject.h"
#include "Components/ZClimbable.h"
#include "Components\DetectorComponent.h"
#include "Components\ActivableComponent.h"
#include "Components\Laser.h"
#include "Components\Checkpoint.h"
#include "Game\Logic\StateMachines\SlothStateMachine.h"

//GameObject *cameraObj;
//GameObject *cameraObjIMGUI;
//Graphics::Camera *cam1;
//Graphics::Camera *imguiCam;
//GameObject* ground;

extern Graphics::AnimComp *playerRend;
extern GameObject* box;



//inline GameObject* AddBox(glm::vec2 pos)
//{
//	auto* object = objectManager.AddGameObject("floor");
//	object->m_transform->SetScale(glm::vec2(1500, 50));
//	auto* rend = object->AddComponent<Graphics::Renderable>();
//	rend->pMesh = GraphicsSystem::Instance()->pMesh;
//	rend->pTransform->SetPos(pos);
//	rend->pShader = GraphicsSystem::Instance()->mPrograms[0];
//	//rend->pTexture = GraphicsSystem::Instance()->mTextures[4];
//
//	 object->AddComponent<RigidBody>();
//	 auto * rigidbody = object->FindComponentByType<RigidBody>();
//	 CollisionSystem::Instance()->AddRigidBody(*rigidbody, true);
//
//    GraphicsSystem::Instance()->mSpriteLayer[0]->AddRenderable(rend);
//
//    return object;
//}
//
//inline GameObject* AddPlayer(glm::vec2 pos)
//{
//    CollisionSystem::Instance()->mCollisionIterations = 2;
//    SpriteAnimationData * IdleAnim_data = new SpriteAnimationData("Idle", GraphicsSystem::Instance()->mTextures[1], 7, 2, 2.0f);
//    SpriteAnimationData * MoveAnim_data = new SpriteAnimationData("Move", GraphicsSystem::Instance()->mTextures[3], 5, 1, 2.0f);
//
//    auto* object = objectManager.AddGameObject("player");
//    object->m_transform->SetScale(glm::vec2(20, 20));
//    auto* rend = object->AddComponent<Graphics::AnimComp>();
//    //rend->pMesh = GraphicsSystem::Instance()->pMesh;
//    rend->mAnimData = IdleAnim_data;
//    rend->pTransform->SetPos(pos);
//    //rend->pShader = GraphicsSystem::Instance()->mPrograms[1];
//
//    //EXAMPLE ADD ADD AN STATE MACHINE TO DIBO:
//    StateMachine * machine = object->AddComponent<StateMachine>();
//
//    //add states to dibo:
//    machine->AddState<Idle_State>();		//States
//    machine->AddState<Jump_State>();
//    machine->AddState<MoveRight_State>();
//	machine->AddState<MoveLeft_State>();
//	machine->AddState<ThrowTongue_State>();
//	machine->AddState<Propulsed_State>();
//	machine->AddState<Climb_State>();
//	machine->AddState<ZClimb_State>();
//    machine->SetInitState<Idle_State>();	
//
//    machine->GetState<MoveLeft_State>()->animation = MoveAnim_data;
//    machine->GetState<Idle_State>()->animation = IdleAnim_data;
//    machine->GetState<MoveRight_State>()->animation = MoveAnim_data;
//
//    object->AddComponent<RigidBody>();
//    auto * rigidbody = object->FindComponentByType<RigidBody>();
//    CollisionSystem::Instance()->AddRigidBody(*rigidbody, false);
//
//    rigidbody->mRestitution = 0.f;
//
//    machine->SetInitState<Idle_State>();
//
//    GraphicsSystem::Instance()->mSpriteLayer[0]->AddRenderable(rend);
//
//    return object;
//}
//
//class LevelTest : public Level
//{
//public:
//    void Initialize()
//    {
//        //FAKED RESOURCE MANAGER
//        CollisionGroup floorCollision = CollisionGroup::CreateCollisionGroup("Floor");
//		CollisionGroup playerCollision = CollisionGroup::CreateCollisionGroup("Player");
//		CollisionGroup ClimbableGroup = CollisionGroup::CreateCollisionGroup("ClimbableObj");
//		CollisionGroup ZClimbableGroup = CollisionGroup::CreateCollisionGroup("ZClimbableObj");
//		CollisionGroup PropulsionObjCollision = CollisionGroup::CreateCollisionGroup("PropulsionObj");
//		CollisionGroup TongueCollisionGroup = CollisionGroup::CreateCollisionGroup("TongueCollisionGroup");
//
//
//		CollisionTable::Instance()->AddPolicy(CollisionGroup::GetCollisionGroup("Floor")               .getId(), CollisionGroup::GetCollisionGroup("Player")              .getId(), CollisionPolicy::Resolve);
//		CollisionTable::Instance()->AddPolicy(CollisionGroup::GetCollisionGroup("Player")              .getId(), CollisionGroup::GetCollisionGroup("ClimbableObj")        .getId(), CollisionPolicy::Resolve);
//		CollisionTable::Instance()->AddPolicy(CollisionGroup::GetCollisionGroup("Player")              .getId(), CollisionGroup::GetCollisionGroup("PropulsionObj")       .getId(), CollisionPolicy::SkipDetection);
//		CollisionTable::Instance()->AddPolicy(CollisionGroup::GetCollisionGroup("Player")              .getId(), CollisionGroup::GetCollisionGroup("TongueCollisionGroup").getId(), CollisionPolicy::SkipDetection);
//		CollisionTable::Instance()->AddPolicy(CollisionGroup::GetCollisionGroup("ZClimbableObj")       .getId(), CollisionGroup::GetCollisionGroup("Player")              .getId(), CollisionPolicy::SkipResolution);
//		CollisionTable::Instance()->AddPolicy(CollisionGroup::GetCollisionGroup("TongueCollisionGroup").getId(), CollisionGroup::GetCollisionGroup("PropulsionObj")       .getId(), CollisionPolicy::Resolve);
//
//        CollisionSystem::Instance()->Init();
//
//        cameraObj = objectManager.AddGameObject("camera");
//        cam1 = cameraObj->AddComponent<Graphics::Camera>();
//
//        GameObject * player = AddPlayer(glm::vec2(0, 50));
//        cam1->setPlayerTrans(player->m_transform);
//
//        player->FindComponentByType<RigidBody>()->addCollisionGroup(playerCollision);
//
//        GraphicsSystem::Instance()->mSpriteLayer[0]->setCamera(cam1);
//        
//		ground = AddBox({ 0,-5 });
//		auto * rigidbody = ground->FindComponentByType<RigidBody>();
//        rigidbody->addCollisionGroup(floorCollision);
//
//		rigidbody->setAffectedByGravity(false);
//		rigidbody->isStatic = true;
//		rigidbody->mInvMass = 0.0f;
//
//		GameObject * PropulseObj = objectManager.AddGameObject("PropulseObj");
//
//		PropulseObj->m_transform->SetScale(glm::vec2(100, 50));
//		auto* rend = PropulseObj->AddComponent<Graphics::Renderable>();
//		rend->pMesh = GraphicsSystem::Instance()->pMesh;
//		rend->pTransform->SetPos(glm::vec2(0, 150));
//		rend->pShader = GraphicsSystem::Instance()->mPrograms[0];
//
//		PropulseObj->AddComponent<PropulsionObject>();
//
//		PropulseObj->AddComponent<RigidBody>();
//		auto * rigidbody_Propulse = PropulseObj->FindComponentByType<RigidBody>();
//
//		CollisionSystem::Instance()->AddRigidBody(*rigidbody_Propulse, true);
//
//		rigidbody_Propulse->addCollisionGroup(PropulsionObjCollision);
//
//		rigidbody_Propulse->setAffectedByGravity(false);
//		rigidbody_Propulse->isStatic = true;
//		rigidbody_Propulse->mInvMass = 0.0f;
//
//		GraphicsSystem::Instance()->mSpriteLayer[0]->AddRenderable(rend);
//
//
//
//		/////////
//		GameObject * ClimbableObj = objectManager.AddGameObject("ClimbableObj");
//
//		ClimbableObj->m_transform->SetScale(glm::vec2(50, 150));
//		auto* rendclimb = ClimbableObj->AddComponent<Graphics::Renderable>();
//		rendclimb->pMesh = GraphicsSystem::Instance()->pMesh;
//		rendclimb->pTransform->SetPos(glm::vec2(250, 110));
//		rendclimb->pShader = GraphicsSystem::Instance()->mPrograms[0];
//
//		ClimbableObj->AddComponent<PropulsionObject>();
//		ClimbableObj->AddComponent<ClimbableObject>();
//		
//
//		ClimbableObj->AddComponent<RigidBody>();
//		auto * rigidbody_Climb = ClimbableObj->FindComponentByType<RigidBody>();
//
//		CollisionSystem::Instance()->AddRigidBody(*rigidbody_Climb, true);
//
//		rigidbody_Climb->addCollisionGroup(ClimbableGroup);
//
//		rigidbody_Climb->setAffectedByGravity(false);
//		rigidbody_Climb->isStatic = true;
//		rigidbody_Climb->mInvMass = 0.0f;
//
//		GraphicsSystem::Instance()->mSpriteLayer[0]->AddRenderable(rendclimb);
//		/////////
//
//		/////////
//		GameObject * ClimbableObj2 = objectManager.AddGameObject("ClimbableObj");
//
//		ClimbableObj2->m_transform->SetScale(glm::vec2(50, 150));
//		auto* rendclimb2 = ClimbableObj2->AddComponent<Graphics::Renderable>();
//		rendclimb2->pMesh = GraphicsSystem::Instance()->pMesh;
//		rendclimb2->pTransform->SetPos(glm::vec2(100, 150));
//		rendclimb2->pShader = GraphicsSystem::Instance()->mPrograms[0];
//
//		ClimbableObj2->AddComponent<PropulsionObject>();
//		ClimbableObj2->AddComponent<ClimbableObject>();
//		
//
//		ClimbableObj2->AddComponent<RigidBody>();
//		auto * rigidbody_Climb2 = ClimbableObj2->FindComponentByType<RigidBody>();
//
//		CollisionSystem::Instance()->AddRigidBody(*rigidbody_Climb2, true);
//
//		rigidbody_Climb2->addCollisionGroup(ClimbableGroup);
//
//		rigidbody_Climb2->setAffectedByGravity(false);
//		rigidbody_Climb2->isStatic = true;
//		rigidbody_Climb2->mInvMass = 0.0f;
//
//		GraphicsSystem::Instance()->mSpriteLayer[0]->AddRenderable(rendclimb2);
//		///////
//
//		/////////
//		GameObject * ZClimbableObj = objectManager.AddGameObject("ZClimbableObj");
//
//		ZClimbableObj->m_transform->SetScale(glm::vec2(50, 150));
//		auto* Zrendclimb = ZClimbableObj->AddComponent<Graphics::Renderable>();
//		Zrendclimb->pMesh = GraphicsSystem::Instance()->pMesh;
//		Zrendclimb->pTransform->SetPos(glm::vec2(-150, 100));
//		Zrendclimb->pShader = GraphicsSystem::Instance()->mPrograms[0];
//
//		ZClimbableObj->AddComponent<ZClimbableObject>();
//
//		//ZClimbableObj->AddComponent<MovingPlatform>();
//
//		ZClimbableObj->AddComponent<RigidBody>();
//		auto * Zrigidbody_Climb = ZClimbableObj->FindComponentByType<RigidBody>();
//
//		CollisionSystem::Instance()->AddRigidBody(*Zrigidbody_Climb, true);
//
//		Zrigidbody_Climb->addCollisionGroup(ZClimbableGroup);
//
//		Zrigidbody_Climb->setAffectedByGravity(false);
//		Zrigidbody_Climb->isStatic = true;
//		Zrigidbody_Climb->mInvMass = 0.0f;
//
//		GraphicsSystem::Instance()->mSpriteLayer[0]->AddRenderable(Zrendclimb);
//		///////
//	}
//    virtual void LoadResources()
//    {
//
//    }
//    virtual void Update()
//    {
//        CollisionSystem::Instance()->Update();
//        if (Input::Instance()->KeyTriggered('I'))
//        {
//            ground = AddBox({ 0, -5 });
//            auto * rigidbody = ground->FindComponentByType<RigidBody>();
//            rigidbody->setAffectedByGravity(false);
//            ground->m_transform->SetScale({ 20, 1 });
//            CollisionSystem::Instance()->AddRigidBody(*rigidbody, false);
//            rigidbody->isStatic = true;
//            rigidbody->mInvMass = 0.0f;
//        }
//        if (Input::Instance()->KeyTriggered('P'))
//        {
//        }
//
//        if (Input::Instance()->KeyTriggered('S'))
//        {
//            objectManager.SaveToFile("LevelTest.json");
//        }
//        if (Input::Instance()->MouseTriggered(Input::eMouseLeft))
//        {
//            //glm::vec2 pos =  cam1->convertScreenSToWorldS();
//            //GameObject* test = AddBox(pos);
//            //test->m_transform->SetScale({ 50,50 });
//        }
//        if (Input::Instance()->KeyTriggered('L'))
//        {
//
//        }
//        objectManager.Update();
//        //objectManager.LateUpdate();
//    }
//    virtual void UnloadResources()
//    {
//        objectManager.Destroy();
//    }
//    virtual void ShutDown()
//    {
//
//    }
//};

class LevelTest : public Level
{
public:
	virtual void Initialize();

	virtual void LoadResources(const std::string& name, const std::string& folder);

	virtual void Update();

	virtual void UnloadResources();

	virtual void ShutDown();

};
