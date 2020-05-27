#include "Rabbit.h"

REFLECTION(Rabbit, jumpNodes);

#define EPSILON 0.01f

void Rabbit::Initialize()
{
	AddNode({ 0.f,0.f });
	AddNode({ 60.f,0.f });
	AddNode({ 200.f,-100.f });
	if (jumpNodes[nodeIndexA].y > jumpNodes[nodeIndexB].y)
	{
		heightPos.y = jumpNodes[nodeIndexA].y + (abs(jumpNodes[nodeIndexA].x - jumpNodes[nodeIndexB].x) / 2.f);
	}
	else
	{
		heightPos.y = jumpNodes[nodeIndexB].y + (abs(jumpNodes[nodeIndexA].x - jumpNodes[nodeIndexB].x) / 2.f);
	}
	m_name = "Rabbit";
}

void Rabbit::Update()
{
	if (jumpNodes.size() < 2)
		return;
	if (glm::distance(GetOwner()->m_transform->GetPos(), jumpNodes[nodeIndexB]) < EPSILON)
	{
		UpdateJumpNodes();
		t = 0.f;
	}
	Jump();
	t += dt;
}

void Rabbit::AddNode(const glm::vec2 & pos)
{
	jumpNodes.push_back(pos);
}
void Rabbit::AddNode(glm::vec2 && pos)
{
	jumpNodes.push_back(std::move(pos));
}

void Rabbit::UpdateJumpNodes()
{
	++nodeIndexA;
	++nodeIndexB;
	if (nodeIndexB >= jumpNodes.size())
		nodeIndexB = 0;
	if (nodeIndexA >= jumpNodes.size())
		nodeIndexA = 0;
	heightPos.x = (jumpNodes[nodeIndexA].x + jumpNodes[nodeIndexB].x) / 2.f;
	
	if (jumpNodes[nodeIndexA].y > jumpNodes[nodeIndexB].y)
	{
		heightPos.y = jumpNodes[nodeIndexA].y + (abs(jumpNodes[nodeIndexA].x - jumpNodes[nodeIndexB].x) / 2.f);
	}
	else
	{
		heightPos.y = jumpNodes[nodeIndexB].y + (abs(jumpNodes[nodeIndexA].x - jumpNodes[nodeIndexB].x) / 2.f);
	}
}

void Rabbit::Jump()
{
	GetOwner()->m_transform->mPosition.x = ((1.f - t)*(1.f - t)) * jumpNodes[nodeIndexA].x + 2.f * t * (1.f - t) * heightPos.x + (t*t) * jumpNodes[nodeIndexB].x;
	GetOwner()->m_transform->mPosition.y = ((1.f - t)*(1.f - t)) * jumpNodes[nodeIndexA].y + 2.f * t * (1.f - t) * heightPos.y + (t*t) * jumpNodes[nodeIndexB].y;
}
