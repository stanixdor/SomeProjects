#pragma once
#include "../../MatrixEngine.h"
#include <iostream>
#include "../../Engine/Graphics/GraphicsComponents/AnimationComponent.h"

class Player : public Component
{
public:
	SERIALIZABLE_NO_PROPERTIES;
	//RTTI_DECLARATION;
	virtual void Initialize();

	virtual void Update();

	Graphics::Shader * vertex1;
	Graphics::Shader * fragment1;
	Graphics::Camera *cam1;


};