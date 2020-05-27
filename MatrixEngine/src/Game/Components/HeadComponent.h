#pragma once
#include "GameObject\Component.h"
#include "GLM\glm.hpp"
#include "GameObject\GameObject.h"
#include "Physics\CollisionEvents\OnCollisionEvent.h"

class HeadComponent : public Component
{
public:
	SERIALIZABLE_NO_PROPERTIES;

	HeadComponent();
	virtual ~HeadComponent() {}
	// update
	virtual void Update() override;
	virtual void LateUpdate() override;

	void Initialize();

	//event System functions
	void collisionStarted(const OnCollisionStarted& mEvent);
	void collisionPersisted(const OnCollisionPersisted& mEvent);
	void collisionEnded(const OnCollisionEnded& mEvent);

	bool ReadyToRotHead = false;
	bool UpdateMouse = false;

	enum Animations { IDLE, WALK, ELEVATIONJUMP, FALLINGJUMP, LANDINGJUMP, CLIMBH, CLIMBV, CLIMBZ };

	Animations Anims = IDLE;
	glm::vec2 HeadWorldPos;
private:
	GameObject * Owner;	//The owner is the player
	GameObject * Head;	//The Head
	GameObject * ThrowHead = nullptr;	//The Head when the tongue is thrown
	glm::vec2 DirectionVector;
	glm::vec2 PosOffset;
	glm::vec2 OriginalPosOffset;

	glm::vec2 ClickPos;
	float Angle;

	void ComputeVectorandAngle();
	void ComputeAngleWithClickPos();

	void ChangeAnimation(Animations Anim);

	bool AlreadyChangedAnim = true;
	std::string CurrentStateName;

	void GetThrowTongueHead();
	void DisableThrowTongueHead();
	void EnableThrowTongueHead();

	bool ThrowTongueHeadEnabled = false;
	float invisiblewaitcounter = 0.0f;
};