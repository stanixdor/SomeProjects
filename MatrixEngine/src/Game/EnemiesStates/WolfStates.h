#pragma once
#include "Logic\StateMachine\AnimationStateMachine.h"
#include "Physics/CollisionEvents/OnCollisionEvent.h"
#include "Components\CamouflageComponent.h"
#include "Utils/CogPath.h"


class WolfStateMachine : public StateMachine
{
public:
	SERIALIZABLE;

	//member functions.
	void Initialize();
	void Update();

	bool WolfdetectPlayer(GameObject* owner);

	glm::vec2 Jump(GLfloat time, glm::vec2 start, glm::vec2 end, glm::vec2 mid);
	
	//member variables.
	GLfloat mTimeForIdle = 2.5f;
	GLfloat mTimeForPatrol = 0.5f;
	GLfloat mTimeDisconcerted = 2.0f;
	GLfloat mDetectionDis = 150.f;
	GLfloat mJumpHeight = 250.f;
	GLfloat	mAnimTimer = 0.f;
	GLint mDirection = 1;
	GLfloat mMaxMovingRange = 400.f;
	GLfloat mVelocity = 3.5f;
	GLfloat mDistance;
	glm::vec2 mInitGen;
	glm::vec2 mInitialPos;

	glm::vec2 mJumpP0;
	glm::vec2 mJumpP1;
	glm::vec2 mJumpP2;

	CogPath cg1;
	GameObject *mPlayer;
	CamouflageComponent *mCamuflaje;
};

namespace EnemiesStates
{
	//									WOLF

	class Wolf_Idle_State : public AnimState
	{
	public:
		void Enter();
		void Exit();
		void Update();

		virtual void initialize();

		//event System functions
		void collisionStarted(const OnCollisionStarted& mEvent);
	private:
		WolfStateMachine * mWolfMachine;
	};

	class Wolf_Patrol_State : public AnimState
	{
	public:
		void Enter();
		void Exit();
		void Update();

		virtual void initialize();

		//event System functions
		void collisionStarted(const OnCollisionStarted& mEvent);
	private:
		WolfStateMachine * mWolfMachine;
		Graphics::AnimComp *mAnim;
	};

	class Wolf_JumpUp_State : public AnimState
	{
	public:
		void Enter();
		void Exit();
		void Update();

		virtual void initialize();

		//event System functions
		void collisionStarted(const OnCollisionStarted& mEvent);
		void collisionEnded(const OnCollisionPersisted& mEvent);
	private:
		WolfStateMachine * mWolfMachine;
		bool onAir = false;
	};

	class Wolf_Disconcerted_State : public AnimState
	{
	public:
		void Enter();
		void Exit();
		void Update();

		virtual void initialize();

		//event System functions
		void collisionStarted(const OnCollisionStarted& mEvent);
	private:
		WolfStateMachine * mWolfMachine;
	};

	class Wolf_JumpBack_State : public AnimState
	{
	public:
		void Enter();
		void Exit();
		void Update();

		virtual void initialize();

		//event System functions
		void collisionStarted(const OnCollisionStarted& mEvent);
	private:
		WolfStateMachine * mWolfMachine;
		Graphics::AnimComp *mAnim;
		bool direction;
	};
}