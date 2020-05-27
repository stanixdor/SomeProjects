#pragma once
#include "GameObject\Component.h"
#include "GLM\glm.hpp"
#include "GameObject\GameObject.h"
#include "Physics\CollisionEvents\OnCollisionEvent.h"

class FlyComponent : public Component
{
public:
	SERIALIZABLE;

	FlyComponent();
	virtual ~FlyComponent() {}
	// update
	virtual void Update() override;

	void Initialize();

	bool Stop = false;
	glm::vec2 InitialPosition;
	bool teleported = false;
	void onCollisionStarted(const OnCollisionStarted& mEvent);
private:
	GameObject * Owner;	//The owner is the fly
	float MovementCounterUpDown = 0.0f;
	float MovementCounterRightLeft = 0.0f;
	float MaxMovementX = 10.0f;
	float MaxMovementY = 5.0f;
	float IncrementSpeed = 0.2f;
	float MovementSpeedUpDown = 0.3f;
	bool GoUp = true;
	bool GoRight = true;

	void Idle();
	void MoveUpDown();
	void MoveRightLeft();
};