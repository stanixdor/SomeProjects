#pragma once
#include "GameObject\Component.h"
#include "GLM\glm.hpp"
#include "GameObject\GameObject.h"
#include "Physics\CollisionEvents\OnCollisionEvent.h"
#include "Components\CamouflageComponent.h"

class EnergyBarComponent : public Component
{
public:
	SERIALIZABLE_NO_PROPERTIES;
	EnergyBarComponent();
	virtual ~EnergyBarComponent() = default;

	void Update() override;
	void Initialize();


private:
	GameObject * Owner; //The Bar
	GameObject * Player;
	CamouflageComponent * CamouflageComp;

	bool GotTheComponent = false;
	float SizePercentage = 0.0f;

	glm::vec2 OriginalSize;
};