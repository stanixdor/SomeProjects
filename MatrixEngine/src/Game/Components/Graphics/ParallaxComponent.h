#pragma once


#include "Engine/GameObject/Component.h"

class ParallaxComponent : public Component
{
public:

	//SERIALIZABLE;

	SERIALIZABLE;
	ParallaxComponent();
	~ParallaxComponent();

	virtual void Initialize();
	virtual void Update();
	void LateUpdate() override;
	bool isPlayerInRange();
	bool hasAPointerToPlayer() const;
	void findPlayer();
	void computeMovementVector();
	static GameObject* player;

	glm::vec2 movementVector;
	float minDistanceToActivate;
	float effectModifier;
	glm::vec2 lastPlayerPos;
	glm::vec2 currentPlayerPos;
};
#pragma once
