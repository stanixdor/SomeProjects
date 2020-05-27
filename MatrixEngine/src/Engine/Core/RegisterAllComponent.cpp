#include "RegisterAllComponent.h"
#include "..\Base\IBase.h"
#include "Components\Test.h"
#include "../Graphics/GraphicsComponents/Renderable.h"
#include "../Physics/RigidBody.h"
#include "../Graphics/GraphicsComponents/AnimationComponent.h"
#include "Components/Player.h"
#include "Logic\StateMachine\StateMachine.h"
#include "Components\PropulsionObject.h"
#include "Components\ClimbableObject.h"
#include "Components\ZClimbable.h"
#include "Components\Tongue.h"
#include "Components\HeadComponent.h"
#include "Physics\Collider.h"
#include "Graphics\GraphicsComponents\LineRenderer.h"
#include "Game\Logic\StateMachines\SlothStateMachine.h"
#include "Game\Components\DetectorComponent.h"
#include "Game\Components\Checkpoint.h"
#include "Game\Components\ActivableComponent.h"
#include "Components\ActivObject.h"
#include "Game\Components\Laser.h"
#include "Game\Components\Laser2.h"
#include "Game\Logic\StateMachines\PlayerStateMachine.h"
#include "Game\EnemiesStates\WolfStates.h"
#include "Game\EnemiesStates\SheepStates.h"
#include "Components/FlyComponent.h"
#include "Components\SteamComponent.h"
#include "Components\TestPatherino.h"
#include "Components/CamouflageComponent.h"
#include "Components/Graphics/ParallaxComponent.h"
#include "Platform\Input\Button.h"
#include "Components\CollectibleComponent.h"
#include "Graphics\GraphicsComponents\FontRenderable.h"
#include "Components\Pipe.h"
#include "Game\Components\Graphics\CameraZoom.h"
#include "Components\Door.h"

void RegisterAllComponents()
{
	REGISTER_TYPE(Test);
	REGISTER_TYPE(Transform2D);
	REGISTER_TYPE(Graphics::Renderable);
	REGISTER_TYPE(RigidBody);
	REGISTER_TYPE(Pipe);
	REGISTER_TYPE(Graphics::Camera);
	REGISTER_TYPE(Graphics::AnimComp);
	REGISTER_TYPE(Player);
	REGISTER_TYPE(StateMachine);
	REGISTER_TYPE(PropulsionObject);
	REGISTER_TYPE(ClimbableObject);
	REGISTER_TYPE(ZClimbableObject);
	REGISTER_TYPE(TongueComponent);
	REGISTER_TYPE(Collider);
	REGISTER_TYPE(SheepStateMachine);
	REGISTER_TYPE(HeadComponent);
	REGISTER_TYPE(PlayerStateMachine);
	REGISTER_TYPE(Graphics::LineRenderer);
	REGISTER_TYPE(SlothStateMachine);
	REGISTER_TYPE(TestPatherino);
	REGISTER_TYPE(Detector);
	REGISTER_TYPE(Checkpoint);
	REGISTER_TYPE(Activable);
	REGISTER_TYPE(Door);
	REGISTER_TYPE(Laser);
	REGISTER_TYPE(Laser2);
	REGISTER_TYPE(WolfStateMachine);
	REGISTER_TYPE(CamouflageComponent);
	REGISTER_TYPE(FlyComponent);
	REGISTER_TYPE(ParallaxComponent);
	REGISTER_TYPE(Steam);
	REGISTER_TYPE(Button);
	REGISTER_TYPE(CollectibleComponent);
	REGISTER_TYPE(Graphics::FontRenderer);
	REGISTER_TYPE(CamZoom);
}
