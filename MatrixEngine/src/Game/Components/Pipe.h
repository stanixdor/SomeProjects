#pragma once
#include "Engine\GameObject\Component.h"
#include "Engine\Utils\CogPath.h"

#include "Engine\Physics\CollisionEvents\OnCollisionEvent.h"

class Pipe: public Component
{
public:	
	Pipe();

	SERIALIZABLE;

	void Initialize();
	void Update() override;

	CogPath exitCogPath;
	GameObject* exitPipe;

	GameObject* createdTongue = nullptr;
	void onCollisionStarted(const OnCollisionStarted& mEvent);
	//in the future this should work with cogpaths

	static bool fakeTongueHasFullLength;

};