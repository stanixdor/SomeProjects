#pragma once

#include "GameObject\GameObject.h"

class TestTongue : public Component
{
public:
	SERIALIZABLE_NO_PROPERTIES;
	TestTongue();
	virtual ~TestTongue() {}

	virtual void Update() override;

	void Initialize();

	void CreateTongueParts();
	glm::vec2 ComputeMidpoint(glm::vec2 InitialPos, glm::vec2 EndPos);

private:
	GameObject * Begginning;
	GameObject * MidPart;
	GameObject * Ending;
	GameObject * Outline;

	float Length = 0.0f;
	float MaxLength = 100.0f;
	float ScaleOffset = 0.0f;
	glm::vec2 StartingMidPartSize;
	glm::vec2 MidPartMinSize;
	glm::vec2 StartingMidPartPos;
};
