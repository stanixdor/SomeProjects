#include "EnergyBar.h"
#include "Platform\Input\Input.h"
#include "Graphics\GraphicsComponents\Renderable.h"
#include "GameObject\GameObject.h"

EnergyBarComponent::EnergyBarComponent() : Component("EnergyBarComponent")
{
	m_name = "EnergyBarComponent";
}

void EnergyBarComponent::Update()
{
	if (Player == nullptr)
	{
		Player = objectManager.FindObjectByName("player");
		if (Player == nullptr)
			return;
	}
	if (!GotTheComponent)
	{
		CamouflageComp = Player->FindComponentByType<CamouflageComponent>();
		GotTheComponent = true;
	}

	if (SizePercentage != (CamouflageComp->CurrentEnergy / CamouflageComp->MaxEnergy))
	{
		SizePercentage = (CamouflageComp->CurrentEnergy / CamouflageComp->MaxEnergy);
		Owner->m_transform->mScale.x = OriginalSize.x * SizePercentage;
		Owner->m_transform->mPosition.x = Owner->m_transform->mPosition.x - ((OriginalSize.x - (OriginalSize.x * SizePercentage)) / 2.0f);
	}
}

void EnergyBarComponent::Initialize()
{
	Owner = this->GetOwner();	//Save the object
	
	Player = objectManager.FindObjectByName("player");

	OriginalSize = Owner->m_transform->mScale;
}