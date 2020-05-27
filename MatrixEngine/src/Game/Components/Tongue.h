#pragma once
#include "GameObject\Component.h"
#include "GLM\glm.hpp"
#include "GameObject\GameObject.h"
#include "Physics\CollisionEvents\OnCollisionEvent.h"

class TongueComponent : public Component
{
public:
	SERIALIZABLE_NO_PROPERTIES;
	TongueComponent();
	~TongueComponent();
	// update
	virtual void Update() override;
	virtual void LateUpdate() override;
	void Initialize();

	//event System functions
	void collisionStarted(const OnCollisionStarted& mEvent);
	void collisionPersisted(const OnCollisionPersisted& mEvent);
	void collisionEnded(const OnCollisionEnded& mEvent);

	float getangle() { return Angle; };

	glm::vec2 GetDirVec();

	bool Return;
	GameObject * PropulsionObjCollidedWith = nullptr;
	GameObject * ThrowHead;
	glm::vec2 CollisionPoint;
	glm::vec2 ClickPosition;
	static bool paused;
	float Length = 0.0f;
	float MaxLength;

private:
	glm::vec2 Tongue_size;
	GameObject * Owner;
	GameObject * Playerptr;

	float IncreaseSizeSpeed;
	float HalfIncreaseSizeSpeed;
	float DistanceCounter;
	float MaxDistance;
	float Angle;
	glm::vec2 DirectionVector;
	glm::vec2 PositionOffset;

	bool alreadycorrectedflip = false;

	void Increase_DecreaseTongue();
	void ComputeVectorandAngle();
	void UpdateVectorAndAngle();
	void CreateTongueParts();
	GameObject * ConfigureObjSettings(GameObject * Object, const char * TextureName, glm::vec2 Position = { 0, 0 }, glm::vec2 Size = { 100.0f, 100.0f });
	glm::vec2 ComputeMidpoint(glm::vec2 InitialPos, glm::vec2 EndPos);
	void DestroyAllObjects();
	void UpdateAngleOfObject(GameObject * Object);
	float Lerp(float start, float end, float t);

	float QuadraticEaseIn(float currenttime, float start, float cahngeinvalue, float duration);
	void GotAFly();

	GameObject * FlyCatched = nullptr;

	bool FlyEaten = false;

	/****/
	GameObject * Begginning = nullptr;
	GameObject * MidPart = nullptr;
	GameObject * Ending = nullptr;
	GameObject * Outline = nullptr;

	float TimeCounter = 0.0f;
	float MaxTime;

	bool UpdatedMaxLength = false;

	float InitialLength = 20.0f;
	float ScaleOffset = 0.0f;
	glm::vec2 StartingMidPartSize;
	glm::vec2 MidPartMinSize;
	glm::vec2 MidPartMaxSize;
	glm::vec2 StartingMidPartPos;

	bool DeleteTongue = false;
};