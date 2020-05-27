// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior 
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:		AnimationStateMachine.h
//	Purpose:		use state machine for the animation of the game:
//	Author:			Javier Tafur, javier.tafur
// ---------------------------------------------------------------------------

#pragma once

//state machine component
#include "Engine/Logic/StateMachine/StateMachine.h"
#include "Engine/Graphics/SpriteAnimationData.h"
#include "Graphics/GraphicsComponents/AnimationComponent.h"
#include "Engine/GameObject/GameObjectManager.h"
#include "Graphics\Texture.h"

class AnimState : public State
{
	public:

		AnimState();
		// data for the animation
		//Graphics::SpriteAnimationData * animation = nullptr;
		GLint CompRows;
		GLint CompColumns;
		GLint CompSprites;
		GLfloat CompDuration;
		GLfloat	mAnimSpeed = 1.0f;
		Graphics::Texture*	StateTexture;
		std::vector<GLfloat> CompDelays;
		int fist_frame = 0;
		bool loop = true;

		GameObject* mOwner;
		
		// internal 
		virtual void InternalEnter();
		virtual void InternalExit();
		virtual void InternalUpdate();
};

