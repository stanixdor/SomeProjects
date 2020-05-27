// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior 
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:		LevelTest.h
//	Purpose:		use state machine for the demo implementation:
//	Author:			Javier Tafur, javier.tafur
// ---------------------------------------------------------------------------

#pragma once
#include "../../Engine/Logic/Level.h"
#include "../../Engine/GameObject/GameObject.h"
#include "../Components/Test.h"
#include "../../src/Engine/Graphics/GraphicsSystem.h"
#include "../../src/Engine/Graphics/Camera.h"
#include "../../src/Engine/Physics/RigidBody.h"
#include "../../Engine/GameObject/GameObjectManager.h"
#include "../../Engine/Physics/CollisionSystem.h"
#include <iostream>
#include "../Components/Player.h"
#include "../../Engine/Graphics/GraphicsComponents/AnimationComponent.h"
#include "../../src/Engine/Logic/StateMachine/StateMachine.h"
#include "../../src/Engine/Logic/StateMachine/PlayerStateMachine.h"

class LevelTestSM : public Level
{
public:
	void Initialize()
	{
		CollisionSystem::Instance()->Init();

	}
	virtual void LoadResources()
	{

	}
	virtual void Update() 
	{
		objectManager.Update();
		CollisionSystem::Instance()->Update();
		

	}
	virtual void UnloadResources()
	{
		objectManager.Destroy();
	}
	virtual void ShutDown()
	{

	}
};
