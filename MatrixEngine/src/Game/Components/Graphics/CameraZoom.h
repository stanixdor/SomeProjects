#pragma once

//Mikel Fernandez

#include "GameObject\Component.h"
#include "Platform\Time\FrameRateController.h"
#include "Physics\CollisionEvents\OnCollisionEvent.h"
#include "Graphics\Camera.h"

class CamZoom : public Component
{
public:
	SERIALIZABLE;
	CamZoom();

	void interpolate();
	void stayStand();
	void goBack();

	virtual void Initialize();
	virtual void Update();

	void collisionStarted(const OnCollisionStarted& mEvent);
	void collisionEnded(const OnCollisionEnded& mEvent);
	glm::vec2 end_pos; //given in the editor.
	glm::vec2 scale;
private:
	bool activated = false;
	float timer = 0;
	//float greater_axis;
	//bool XaxisGreater = false;
	glm::vec2 start_pos; //this will be the player position.
	glm::vec2 original_scale;
	glm::vec2 new_scale;

	Transform2D* camTransform; //the camera transform to make all the computations on that.
	Graphics::Camera *camera;
	GameObject* player;
};
