#include "SpriteAnimationData.h"
#include <iostream>

namespace Graphics
{
	REFLECTION(SpriteAnimationData, mDelays, mDuration, mRows, mColumns);
	SpriteAnimationData::SpriteAnimationData() : mName("Animation")
												, mAtlas(nullptr)
												, mNumSprites(0)
												, mDuration(0.0f)
												, mFrameCount(0)
												, mRows(0)
												, mColumns(0)
	{
		for (int i = 0; i < mNumSprites; i++)
		{
			mDelays.push_back(0.2f);
		}
		mFrameCount = static_cast<GLuint>(mDelays.size());
	}

	SpriteAnimationData::SpriteAnimationData(const char *name, Texture * tex, GLint columns, GLint rows, GLfloat dur, GLuint FrameCount) : mName(name)
		, mAtlas(tex)
		, mDuration(dur)
		, mFrameCount(FrameCount)
		, mRows(rows)
		, mColumns(columns)
	{
		mNumSprites = columns * rows;
		for (int i = 0; i < mNumSprites; i++)
		{
			mDelays.push_back(0.2f);
		}
		mFrameCount = static_cast<GLuint>(mDelays.size());
	}

	SpriteAnimationData::~SpriteAnimationData()
	{

	}
}