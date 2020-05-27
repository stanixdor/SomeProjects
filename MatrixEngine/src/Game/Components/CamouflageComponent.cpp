#include "CamouflageComponent.h"
#include "Platform\Input\Input.h"
#include "Graphics\GraphicsComponents\Renderable.h"

CamouflageComponent::CamouflageComponent() : Component("CamouflageComponent")
{
	m_name = "CamouflageComponent";
}

void CamouflageComponent::Update()
{
	if (PlayerHead == nullptr)
	{
		PlayerHead = objectManager.FindObjectByName("PlayerHead");
	}
	if (FlyEaten)
	{
		Camouflaged = true;
		ChangePlayerAlpha(Camouflaged);
	}
	else
	{
		ResetAlpha();
		Camouflaged = false;
	}
}

void CamouflageComponent::Initialize()
{
	Owner = this->GetOwner();	//Save the object

	PlayerHead = objectManager.FindObjectByName("PlayerHead");

	CamouflageAlpha = 0.5f;
}

bool CamouflageComponent::IsVisible()
{
	return !Camouflaged;
}

void CamouflageComponent::AteFly()
{
	FlyEaten = true;
}

void CamouflageComponent::ReleaseFly()
{
	FlyEaten = false;
}

void CamouflageComponent::ChangePlayerAlpha(bool IsCamouflaged)
{
	if (IsCamouflaged)
	{
		Owner->FindComponentByType<Graphics::Renderable>()->SetAlpha(CamouflageAlpha);
		PlayerHead->FindComponentByType<Graphics::Renderable>()->SetAlpha(CamouflageAlpha);
		return;
	}

	ResetAlpha();
}

void CamouflageComponent::ResetAlpha()
{
	Owner->FindComponentByType<Graphics::Renderable>()->SetAlpha(1.0f);

	if (PlayerHead)
	{
		PlayerHead->FindComponentByType<Graphics::Renderable>()->SetAlpha(1.0f);
	}
}