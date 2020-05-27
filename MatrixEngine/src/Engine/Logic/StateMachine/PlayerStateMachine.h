// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior 
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:		PlayerStateMachine.h
//	Purpose:		Create the logic for the player using state machines.
//	Author:			Jorge Alvarez, Javier Tafur
// ---------------------------------------------------------------------------

#pragma once
#include "Engine/Logic/StateMachine/StateMachine.h"
#include "Engine/Platform/Input/Input.h"
#include "Physics\CollisionEvents\OnCollisionEvent.h"
#include "Engine\Logic\StateMachine\AnimationStateMachine.h"
#include <stdio.h>


// STATES
class PlayerStateMachine : public StateMachine
{
public:
	SERIALIZABLE;
	PlayerStateMachine();
	~PlayerStateMachine();
	void Initialize();

	float JumpForce = 100000.0f;
	float max_velocity = 250.0f;
	float drag_acceleration = 20.0f;
	GameObject * Player = nullptr;
	float ChangeDirHelp = 15.0f;
	StateMachine * playerStateMachine = nullptr;
	GameObject * Tongue = nullptr;
	bool PropulsionActive = false;
	bool OnAir = false;
	bool IsClimbing_H = false;
	bool IsClimbing_V = false;

	bool InitializedflipMovement = false;

	static bool teleportMode;
	//enum PlayerOrientation {Right, Left};
	//PlayerOrientation Orientation = PlayerOrientation::Right;

	//Graphics::AnimComp * AnimationComp;

	float PreviousPos = 0;
	float CurrentPos = 0;

	glm::vec2 InitialPosition;

	void PlayerStateMachine::UpdatePlayerOrientation(Graphics::AnimComp * AnimationComp);
};

namespace PlayerSM
{
	class PlayerState : public AnimState
	{
	public:
		PlayerStateMachine * getPlayerStateMachine();
		static PlayerStateMachine * PSM;

	};

	class Idle_State : public PlayerState
	{
	public:
		void Enter();
		void Exit();
		void Update();

		void initialize() override;

		//event System functions
		void collisionStarted(const OnCollisionStarted& mEvent);
		void collisionPersisted(const OnCollisionPersisted& mEvent);
		void collisionEnded(const OnCollisionEnded& mEvent);

	private:
		Graphics::AnimComp * AnimationComp;
		RigidBody * PlayerRigidBody;

		bool InitializedFlip = false;
	};

	class Elevating_Jump_State : public PlayerState
	{
	public:
		void Enter();
		void Exit();
		void Update();

		virtual void initialize();

		//event System functions
		void collisionStarted(const OnCollisionStarted& mEvent);
		void collisionPersisted(const OnCollisionPersisted& mEvent);
		void collisionEnded(const OnCollisionEnded& mEvent);

	private:
		RigidBody * PlayerRigidBody;
		Graphics::AnimComp * AnimationComp;
	};

	class Falling_Jump_State : public PlayerState
	{
	public:
		void Enter();
		void Exit();
		void Update();

		virtual void initialize();

		//event System functions
		void collisionStarted(const OnCollisionStarted& mEvent);
		void collisionPersisted(const OnCollisionPersisted& mEvent);
		void collisionEnded(const OnCollisionEnded& mEvent);

	private:
		RigidBody * PlayerRigidBody;
		Graphics::AnimComp * AnimationComp;
	};

	class Landing_Jump_State : public PlayerState
	{
	public:
		void Enter();
		void Exit();
		void Update();

		virtual void initialize();

		//event System functions
		void collisionStarted(const OnCollisionStarted& mEvent);
		void collisionPersisted(const OnCollisionPersisted& mEvent);
		void collisionEnded(const OnCollisionEnded& mEvent);

	private:
		Graphics::AnimComp * AnimationComp;
		RigidBody * PlayerRigidBody;
	};

	class MoveLeft_State : public PlayerState
	{
	public:
		void Enter();
		void Exit();
		void Update();

		virtual void initialize();

		//event System functions
		void collisionStarted(const OnCollisionStarted& mEvent);
		void collisionPersisted(const OnCollisionPersisted& mEvent);
		void collisionEnded(const OnCollisionEnded& mEvent);

	private:
		RigidBody * PlayerRigidBody;
		Graphics::AnimComp * AnimationComp;
	};

	class MoveRight_State : public PlayerState
	{
	public:
		void Enter();
		void Exit();
		void Update();

		virtual void initialize();

		//event System functions
		void collisionStarted(const OnCollisionStarted& mEvent);
		void collisionPersisted(const OnCollisionPersisted& mEvent);
		void collisionEnded(const OnCollisionEnded& mEvent);

	private:
		RigidBody * PlayerRigidBody;
		Graphics::AnimComp * AnimationComp;
	};

	class ThrowTongue_State : public PlayerState
	{
	public:
		void Enter();
		void Exit();
		void Update();

		virtual void initialize();

		//event System functions
		void collisionStarted(const OnCollisionStarted& mEvent);
		void collisionPersisted(const OnCollisionPersisted& mEvent);
		void collisionEnded(const OnCollisionEnded& mEvent);

		State * PreviousState;
	};

	class Propulsed_State : public PlayerState
	{
	public:
		void Enter();
		void Exit();
		void Update();

		virtual void initialize();

		//event System functions
		void collisionStarted(const OnCollisionStarted& mEvent);
		void collisionPersisted(const OnCollisionPersisted& mEvent);
		void collisionEnded(const OnCollisionEnded& mEvent);

	private:
		glm::vec2 PropulsionVector;
		RigidBody * PlayerRigidBody;
		Graphics::AnimComp * AnimationComp;
	};

	class HorizontalClimb_State : public PlayerState
	{
	public:
		void Enter();
		void Exit();
		void Update();

		virtual void initialize();

		//event System functions
		void collisionStarted(const OnCollisionStarted& mEvent);
		void collisionPersisted(const OnCollisionPersisted& mEvent);
		void collisionEnded(const OnCollisionEnded& mEvent);


		GameObject * WallClimbing;	//The wall that the player is climbing

	private:
		PlayerStateMachine * PlayerSM;
		Graphics::AnimComp * AnimationComp;
		RigidBody * PlayerRigidBody;

		bool DontMoveRight = false;
		bool DontMoveLeft = false;
	};

	class VerticalClimb_State : public PlayerState
	{
	public:
		void Enter();
		void Exit();
		void Update();

		virtual void initialize();

		//event System functions
		void collisionStarted(const OnCollisionStarted& mEvent);
		void collisionPersisted(const OnCollisionPersisted& mEvent);
		void collisionEnded(const OnCollisionEnded& mEvent);


		GameObject * WallClimbing;	//The wall that the player is climbing

	private:
		PlayerStateMachine * PlayerSM;
		Graphics::AnimComp * AnimationComp;
		RigidBody * PlayerRigidBody;

		bool DontMoveUp = false;
		bool DontMoveDown = false;
	};

	class ZClimb_State : public PlayerState
	{
	public:
		void Enter();
		void Exit();
		void Update();

		virtual void initialize();

		//event System functions
		void collisionStarted(const OnCollisionStarted& mEvent);
		void collisionPersisted(const OnCollisionPersisted& mEvent);
		void collisionEnded(const OnCollisionEnded& mEvent);

		GameObject * WallClimbing;

	private:
		Graphics::AnimComp * AnimationComp;
		RigidBody * PlayerRigidBody;
	};
}