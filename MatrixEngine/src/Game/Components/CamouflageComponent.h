#pragma once
#include "GameObject\Component.h"
#include "GLM\glm.hpp"
#include "GameObject\GameObject.h"
#include "Physics\CollisionEvents\OnCollisionEvent.h"

class CamouflageComponent : public Component
{
public:
	SERIALIZABLE_NO_PROPERTIES;
	CamouflageComponent();
	virtual ~CamouflageComponent() = default;

	void Update() override;
	void Initialize();

	bool IsVisible(); //Getor
	void AteFly();
	void ReleaseFly();

private:
	GameObject * Owner;
	GameObject * PlayerHead;
	Component * RenderComp;
	bool Camouflaged = false;
	bool FlyEaten = false;
	float CamouflageAlpha;

	void ChangePlayerAlpha(bool IsCamouflaged);
	void ResetAlpha();
};
