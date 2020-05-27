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
#include "Engine/Graphics/GraphicsComponents/LineRenderer.h"
#include "Platform\Resource Manager\ResourceManager.h"
#include "EngineDebug\ConsolePrinting.h"
#include <iostream>

extern std::unique_ptr<GameObject> cameraObj;
extern Graphics::Camera *cam1;



class GenericLevel : public Level
{
public:
	virtual void Initialize();

	virtual void LoadResources(const std::string& name, const std::string& folder);

	virtual void Update();

	virtual void UnloadResources();

	virtual void ShutDown();

};
