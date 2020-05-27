#include "CameraZoom.h"
#include "Engine\Platform\EventSystem\event_dispatcher.h"
#include "Engine\GameObject\GameObjectManager.h"
#include "Utils\Interpolation.h"
#include "Graphics\GraphicsSystem.h"

REFLECTION(CamZoom, end_pos);

CamZoom::CamZoom()
{
	m_name = "Camera Zoom";
	start_pos = glm::vec2(0,0);
}

void CamZoom::Initialize()
{
	//register object and function
	using namespace Messaging;

	// register to the event:
	EventHandler* temp = &GetEventHandler();
	temp->register_handler(this, &CamZoom::collisionStarted);
	temp->register_handler(this, &CamZoom::collisionEnded);

	// subscribe this to this type of event
	EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionStarted>());
	EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionEnded>());
}

void CamZoom::interpolate()
{
	GraphicsSystem::Instance()->mSpriteLayer[0]->mCameraLayer->big_room = false;
	player->mode = GameObjectMode::Inactive;
	timer += (GLfloat)FRC::FrameRateController::Instance()->GetFrameTime();

	//move camTransform from startPos to endPos.
	camTransform->SetPos({ Interpolation::lerp(start_pos.x,end_pos.x,timer / 3), Interpolation::lerp(start_pos.y,end_pos.y,timer / 3) });

	//change the scale of the camera.
	//if(XaxisGreater)
	//	camera->CamSetScale({ Interpolation::lerp(original_scale.x,greater_axis,timer / 3), Interpolation::lerp(original_scale.y,greater_axis*(1/camera->aspectRatio),timer / 3) });
	//else
	//	camera->CamSetScale({ Interpolation::lerp(original_scale.x,greater_axis*camera->aspectRatio,timer / 3), Interpolation::lerp(original_scale.y,greater_axis,timer / 3) });
	camera->zoom(1.005f);
}

void CamZoom::stayStand()
{
	timer += (GLfloat)FRC::FrameRateController::Instance()->GetFrameTime();
}

void CamZoom::goBack()
{
	timer += (GLfloat)FRC::FrameRateController::Instance()->GetFrameTime();
	float timerChapuza = timer-4;

	//move camTransform from startPos to endPos.
	camTransform->SetPos({ Interpolation::lerp(end_pos.x, start_pos.x, timerChapuza / 2), Interpolation::lerp(end_pos.y, start_pos.y, timerChapuza / 2) });

	//change the scale of the camera.
	//if (XaxisGreater)
	//	camera->CamSetScale({ Interpolation::lerp(greater_axis, original_scale.x, timerChapuza / 2), Interpolation::lerp(greater_axis*(1 / camera->aspectRatio), original_scale.y, timerChapuza / 2) });
	//else
	//	camera->CamSetScale({ Interpolation::lerp(greater_axis*camera->aspectRatio, original_scale.x, timerChapuza / 2), Interpolation::lerp(greater_axis, original_scale.y, timerChapuza / 2) });
	camera->zoom(GLfloat(1-(0.005*3/2)));
}

void CamZoom::Update()
{
	if (activated)
	{
		if (timer <= 3)
			interpolate();
		if (timer > 3 && timer <= 4)
			stayStand();
		if (timer > 4 && timer <= 6)
			goBack();
		if (timer > 6)
		{
			timer = 0;
			activated = false;
			player->mode = GameObjectMode::Default;
			GraphicsSystem::Instance()->mSpriteLayer[0]->mCameraLayer->big_room = true;
		}
	}
}

void CamZoom::collisionStarted(const OnCollisionStarted& mEvent)
{
	ENGINE_UNUSED(mEvent);
	player = objectManager.FindObjectByName("player");
	start_pos = player->m_transform->GetPos();
	camTransform = objectManager.FindObjectByName("camera")->m_transform;
	camera = objectManager.FindObjectByName("camera")->FindComponentByType<Graphics::Camera>();
	new_scale = 2.0f * (end_pos - start_pos);
	//greater_axis = (new_scale.x > new_scale.y) ? new_scale.x : new_scale.y;
	//greater_axis = new_scale.y;
	/*if (new_scale.x > new_scale.y)
	{
		greater_axis = new_scale.x;
		XaxisGreater = true;
	}
	else
	{
		greater_axis = new_scale.y;
		XaxisGreater = false;
	}*/
	original_scale = camera->GetScale();
}

void CamZoom::collisionEnded(const OnCollisionEnded& mEvent)
{
	ENGINE_UNUSED(mEvent);
	activated = true;
}
