#pragma once
//Mikel Fernandez
#include "Logic\StateMachine\AnimationStateMachine.h"
#include "Physics\CollisionEvents\OnCollisionEvent.h"
#include "Components\CamouflageComponent.h"
#include "Utils/CogPath.h"

class SheepStateMachine : public StateMachine
{
public:
	SERIALIZABLE;
	//member function
	void Initialize();

	void Update();

	bool SheepdetectPlayer(GameObject* owner);
	GameObject* CreateObject(std::string texName);

	//member variables
	GLfloat mDetectionDis = 450.f;
	GLfloat	mAnimTimer = 0.f;
	GLfloat mTimeForIdle = 3.5f;
	GLfloat mTimeForPatrol = 1.0f;
	GLfloat mTimeForDetect = 2.5f;
	GLint mDirection = 1;
	GLfloat mMaxMovingRange = 850.f;
	GLfloat mVelocity = 3.5f;
	glm::vec2 mInitialPos;
	glm::vec2 mInitGen;

	CogPath cg1;
	GameObject *mPlayer;
	CamouflageComponent *mCamuflaje;

	GameObject * detectionAnim = nullptr;
};

namespace EnemiesStates
{
	//									SHEEP

	class Sheep_Idle_State : public AnimState
	{
	public:
		void Enter();
		void Exit();
		void Update();

		virtual void initialize();

		//event System functions
		void collisionStarted(const OnCollisionStarted& mEvent);
	private:
		SheepStateMachine * mSheepMachine;
		Graphics::AnimComp *mAnim;
		bool fliped = false;
	};

	class Sheep_Patrol_State : public AnimState
	{
	public:
		void Enter();
		void Exit();
		void Update();

		virtual void initialize();

		//event System functions
		void collisionStarted(const OnCollisionStarted& mEvent);
	private:
		SheepStateMachine * mSheepMachine;
		Graphics::AnimComp *mAnim;
		bool fliped = false;
	};

	class Sheep_Detection_State : public AnimState
	{
	public:
		void Enter();
		void Exit();
		void Update();

		virtual void initialize();

		//event System functions
		void collisionStarted(const OnCollisionStarted& mEvent);
	private:
		SheepStateMachine * mSheepMachine;
		Graphics::AnimComp *mAnim;
		GameObject* question;
		Graphics::AnimComp *questionAnim;
		CamouflageComponent *mCamuflaje;
		bool fliped = false;
		float forceBounceTimer = 0.0f;
	};

	class Sheep_Detected_State : public AnimState
	{
	public:
		void Enter();
		void Exit();
		void Update();

		virtual void initialize();
	private:
		SheepStateMachine * mSheepMachine;
		Graphics::AnimComp *mAnim;
		GameObject* exclamation;
		Graphics::AnimComp *exclamationAnim;
		bool fliped = false;
		float forceBounceTimer = 0.0f;
	};
}