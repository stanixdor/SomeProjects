// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior 
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:		AnimationStateMachine.h
//	Purpose:		use state machine for the animation of the game:
//	Author:			Javier Tafur, javier.tafur
// ---------------------------------------------------------------------------

#include "Engine/Logic/StateMachine/AnimationStateMachine.h"
#include "Graphics\GraphicsComponents\AnimationComponent.h"
#include <assert.h>


// overide Internal function of the state machine for the Animations:


AnimState::AnimState()
{

}

void AnimState::InternalEnter()
{
	State::InternalEnter();  // base behaviour

	if (mOwnerStateMachine->GetOwner()->FindComponentByType<Graphics::AnimComp>() == nullptr)
		return;
	assert(mOwnerStateMachine->GetOwner()->FindComponentByType<Graphics::AnimComp>() != nullptr && "The state machine owner contains no Animation component"); // break if not true
	mOwnerStateMachine->GetOwner()->FindComponentByType<Graphics::AnimComp>()->CompRows = CompRows;
	mOwnerStateMachine->GetOwner()->FindComponentByType<Graphics::AnimComp>()->CompColumns = CompColumns;
	mOwnerStateMachine->GetOwner()->FindComponentByType<Graphics::AnimComp>()->CompSprites = CompSprites;
	mOwnerStateMachine->GetOwner()->FindComponentByType<Graphics::AnimComp>()->CompDuration = CompDuration;
	mOwnerStateMachine->GetOwner()->FindComponentByType<Graphics::AnimComp>()->CompDelays = CompDelays;
	mOwnerStateMachine->GetOwner()->FindComponentByType<Graphics::AnimComp>()->pTexture = StateTexture;
	mOwnerStateMachine->GetOwner()->FindComponentByType<Graphics::AnimComp>()->SetLoop(loop);
	mOwnerStateMachine->GetOwner()->FindComponentByType<Graphics::AnimComp>()->mAnimSpeed = mAnimSpeed;
	mOwnerStateMachine->GetOwner()->FindComponentByType<Graphics::AnimComp>()->ResetAnimation();
}


void AnimState::InternalUpdate()
{

	State::InternalUpdate();  // base behaviour
}


void AnimState::InternalExit()
{
	State::InternalExit();  // base behaviour
}
