#pragma once

#include "Renderable.h"
#include "../SpriteAnimationData.h"

#define Right false
#define Left true

namespace Graphics
{
	class AnimComp : public Renderable
	{
	public:
		SERIALIZABLE;
		AnimComp();
		~AnimComp();

		virtual void Initialize() override;
		virtual void Shutdown() override;
		virtual void Render(Camera * cameraToUse) override;

		void updateValues();

		// Animation Methods
		void PlayAnimation();
		void PauseAnimation();
		void ResetAnimation();
		void SetLoop(bool enable);
		void FlipAnimation();
		void SetAnimationFrame(GLuint frameIndex);

		//Specific update for animations
		void Update();

		//SpriteAnimationData * mAnimData;
		std::string mAtlasName;

		GLuint					mAnimCurrentFrame;
		GLfloat					mAnimTimer;
		bool					mbAnimLoop;
		bool					mbAnimPlaying;
		bool					flipX = Right;

		GLint CompRows;
		GLint CompColumns;
		GLint CompSprites;
		GLfloat CompDuration;
		GLfloat	mAnimSpeed;
		std::vector<GLfloat> CompDelays;
	};
}
