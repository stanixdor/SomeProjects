#pragma once
#include "GameObject\GameObject.h"
#include "GLM\glm.hpp"
#include <vector>



class Rabbit : public Component
{
public:
	SERIALIZABLE;
	virtual void Initialize() override;
	virtual void Update() override;
	void AddNode(glm::vec2 && pos);
	void AddNode(const glm::vec2& pos);
	void UpdateJumpNodes();
	void Jump();

private:
	std::vector<glm::vec2> jumpNodes;
	unsigned nodeIndexA{ 0 };
	unsigned nodeIndexB{ 1 };
	glm::vec2 heightPos{0.f,0.f};
	float t = 0.f;
	float dt = 0.01f;

};