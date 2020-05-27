#include "ParallaxComponent.h"
#include "Engine\GameObject\GameObjectManager.h"

GameObject* ParallaxComponent::player = nullptr;

REFLECTION(ParallaxComponent, effectModifier, minDistanceToActivate)

ParallaxComponent::ParallaxComponent()
{
	m_name = "Parallax";
	effectModifier = 1.f;
	minDistanceToActivate = 400.f;
	
}

ParallaxComponent::~ParallaxComponent()
{
	player = nullptr;
}

void ParallaxComponent::Initialize()
{
	
	if (!hasAPointerToPlayer())
		findPlayer();

}

void ParallaxComponent::Update()
{
	if (!hasAPointerToPlayer())
		findPlayer();

	player = objectManager.FindObjectByName("player");
	if (player != nullptr)
		m_GameObject->m_transform->mPosition = player->m_transform->GetPos();

	//if the player is in range to activate the parallax
	//if (isPlayerInRange())
	//{	
	//	computeMovementVector();
	//	m_GameObject->m_transform->translate(movementVector);
	//}
	//
	////refresh player pos
	//lastPlayerPos = currentPlayerPos;
}

bool ParallaxComponent::isPlayerInRange()
{
	currentPlayerPos = player->m_transform->GetPos();
	glm::vec2 myPos = this->m_GameObject->m_transform->GetPos();

	movementVector = (currentPlayerPos - lastPlayerPos);
	return (glm::length(movementVector) < minDistanceToActivate);
}

bool ParallaxComponent::hasAPointerToPlayer() const
{
	return !(player == nullptr);
}

void ParallaxComponent::findPlayer()
{
	player = objectManager.FindObjectByName("player");
	if(player != nullptr)
	lastPlayerPos = player->m_transform->GetPos();
}

void ParallaxComponent::computeMovementVector()
{
	glm::normalize(movementVector);
	movementVector *= effectModifier;
}

void ParallaxComponent::LateUpdate()
{
	if (player->m_alive == false)
		player = nullptr;
}
