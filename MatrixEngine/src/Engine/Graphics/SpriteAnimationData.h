#pragma once

#include "glew.h"
#include <vector>
#include "Texture.h"
#include "GameObject\Component.h"

namespace Graphics
{
	class SpriteAnimationData : public Component
	{
	public:
		SERIALIZABLE;
		SpriteAnimationData();
		SpriteAnimationData(const char *name, Texture * tex, GLint columns, GLint rows, GLfloat dur, GLuint FrameCount = 0);
		~SpriteAnimationData();

		void setDuration(GLfloat time) { mDuration = time; }
		void setNumSprites(GLint sprites) { mNumSprites = sprites; }

		std::string	mName;			           // name of the animation
		Texture * mAtlas;                     // frame list
		GLint mNumSprites;                   // number of sprites in the texture
		GLint mRows;
		GLint mColumns;
		GLfloat	mDuration;		            // animation duration
		GLuint	mFrameCount;	           // framecount
		std::vector<GLfloat> mDelays;	  // Delays
	};
}