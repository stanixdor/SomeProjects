#pragma once
//Mikel Fernandez

#include "Logic\StateMachine\AnimationStateMachine.h"
#include "Physics\CollisionEvents\OnCollisionEvent.h"

class SheepStateMachine : public StateMachine
{
public:
	void Initialize();
};

namespace EnemiesStates
{
	class SheepInfoComp : public Component
	{
	public:
		SERIALIZABLE;
		SheepInfoComp();

		GLfloat mDetectionDis = 100.f;
		GLfloat	mAnimTimer = 0.f;
		GLint mDirection = 1;
		GLfloat mMaxMovingRange = 50.f;
		GLfloat mVelocity = 1.5f;
		glm::vec2 mInitialPos;
	};

	class WolfInfoComp : public Component
	{
	public:
		SERIALIZABLE;
		WolfInfoComp();

		GLfloat mDetectionDis = 100.f;
		GLfloat	mAnimTimer = 0.f;
		GLint mDirection = 1;
		GLfloat mMaxMovingRange = 50.f;
		GLfloat mVelocity = 1.5f;
		GLfloat mJumpVelocity = 5.0f;
		GLfloat mAngle = 0.0f;
		GLfloat mGrav;
		GLfloat mDistance;
		glm::vec2 mInitialPos;
	};

	//									SHEEP

	class Sheep_Idle_State : public AnimState
	{
	public:
		void Enter();
		void Exit();
		void Update();

		virtual void initialize();

		//event System functions
		//void collisionStarted(const OnCollisionStarted& mEvent);
		//void collisionPersisted(const OnCollisionPersisted& mEvent);
		//void collisionEnded(const OnCollisionEnded& mEvent);
	};

	class Sheep_Patrol_State : public AnimState
	{
	public:
		void Enter();
		void Exit();
		void Update();

		virtual void initialize();

		//event System functions
		//void collisionStarted(const OnCollisionStarted& mEvent);
		//void collisionPersisted(const OnCollisionPersisted& mEvent);
		//void collisionEnded(const OnCollisionEnded& mEvent);
	};

	class Sheep_Detection_State : public AnimState
	{
	public:
		void Enter();
		void Exit();
		void Update();

		virtual void initialize();

		//event System functions
		//void collisionStarted(const OnCollisionStarted& mEvent);
		//void collisionPersisted(const OnCollisionPersisted& mEvent);
		//void collisionEnded(const OnCollisionEnded& mEvent);
	};
	
	//									WOLF

	class Wolf_Idle_State: public AnimState
	{
	public:
		void Enter();
		void Exit();
		void Update();

		virtual void initialize();

		//event System functions
		//void collisionStarted(const OnCollisionStarted& mEvent);
		//void collisionPersisted(const OnCollisionPersisted& mEvent);
		//void collisionEnded(const OnCollisionEnded& mEvent);
	};

	class Wolf_Patrol_State : public AnimState
	{
	public:
		void Enter();
		void Exit();
		void Update();

		virtual void initialize();

		//event System functions
		//void collisionStarted(const OnCollisionStarted& mEvent);
		//void collisionPersisted(const OnCollisionPersisted& mEvent);
		//void collisionEnded(const OnCollisionEnded& mEvent);
	};

	//este sera para la animacion de cuando el lobo empieza a saltar

	/*class Wolf_Detection_State : public AnimState
	{
	public:
		void Enter();
		void Exit();
		void Update();

		virtual void initialize();

		//event System functions
		//void collisionStarted(const OnCollisionStarted& mEvent);
		//void collisionPersisted(const OnCollisionPersisted& mEvent);
		//void collisionEnded(const OnCollisionEnded& mEvent);
	};*/

	class Wolf_JumpUp_State : public AnimState
	{
	public:
		void Enter();
		void Exit();
		void Update();

		virtual void initialize();

		GLfloat calculateAngle(glm::vec2 targetPos);

		//event System functions
		//void collisionStarted(const OnCollisionStarted& mEvent);
		//void collisionPersisted(const OnCollisionPersisted& mEvent);
		//void collisionEnded(const OnCollisionEnded& mEvent);
		WolfInfoComp * info;
	};

	class Wolf_JumpDown_State : public AnimState
	{
	public:
		void Enter();
		void Exit();
		void Update();

		virtual void initialize();

		//event System functions
		//void collisionStarted(const OnCollisionStarted& mEvent);
		//void collisionPersisted(const OnCollisionPersisted& mEvent);
		//void collisionEnded(const OnCollisionEnded& mEvent);
	};


}