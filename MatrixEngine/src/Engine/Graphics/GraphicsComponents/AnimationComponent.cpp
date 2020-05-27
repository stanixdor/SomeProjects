#include "AnimationComponent.h"

#include "../../Platform/Time/FrameRateController.h"
#include "Engine\Graphics\GraphicsSystem.h"
#include "Platform\Resource Manager\ResourceManager.h"
#include "Engine\Graphics\GraphicsSystem.h"

#include "../../Transform/Transform2D.h"
#include "../Mesh.h"
#include "../Shader.h"
#include "glew.h"
#include <cmath>
#include "GameObject\GameObject.h"

namespace Graphics
{
	REFLECTION(AnimComp, mAtlasName, CompRows, CompColumns, CompSprites, CompDuration, CompDelays, mLayer, originalColor);
    AnimComp::AnimComp() : Renderable(), mAnimCurrentFrame(0)
        , mAnimTimer(0.0f), mAnimSpeed(1.0f), mbAnimLoop(true), mbAnimPlaying(true), mAtlasName("Default.png")
    {
        m_name = "Animation Component";
        //pMesh->Clear_Vectors();

		pMesh = (GraphicsSystem::Instance()->pMesh);
		pShader = GraphicsSystem::Instance()->mPrograms[1];
		CompRows = 1;
		CompColumns = 1;
		CompSprites = CompRows * CompColumns;
		CompDuration = 2.f;
		mAnimSpeed = 1.f;
		for (auto& delay : CompDelays)
			delay = 0.2f;
		//mAnimData = GraphicsSystem::Instance()->IdleAnim_data3;
		pTexture = ResourceManager::textures[mAtlasName].get();
    }

    AnimComp::~AnimComp()
    {

    }

	void AnimComp::updateValues()
	{
		/*CompRows = mAnimData->mRows;
		CompColumns = mAnimData->mColumns;
		CompSprites = CompRows * CompColumns;
		CompDuration = mAnimData->mDuration;
		CompDelays.resize(mAnimData->mDelays.size());
		mAnimData->mAtlas = ResourceManager::textures[mAtlasName].get();*/
		/*for (unsigned int i = 0; i < CompDelays.size(); i++)
		{
			CompDelays[i] = mAnimData->mDelays[i];
		}*/

	}

    void AnimComp::Update()
    {
		updateValues();
        //if (mAnimData == nullptr)
        //    return;
        //if (mbAnimPlaying == false)
        //    return;

        if (pTexture)
        {
            mAnimTimer += (GLfloat)FRC::FrameRateController::Instance()->GetFrameTime() * mAnimSpeed;

            if (mbAnimPlaying)
            {
				if (mAnimCurrentFrame < CompDelays.size())
				{
					if (mAnimTimer > CompDelays[mAnimCurrentFrame]) //if we have passed the specific time for a frame
					{
						//mAnimTimer -= CompDelays[mAnimCurrentFrame]; //????
						mAnimCurrentFrame++;								//We go to th next frame
						mAnimTimer = 0.0f;									//set the timer to zero again.
						if (mAnimCurrentFrame >= static_cast<GLuint>(CompSprites))	//if we reach the end of the atlas
						{
							if (mbAnimLoop)
								mAnimCurrentFrame = 0;						//we start the animation again.
							else
								mbAnimPlaying = false;
						}
					}
				}
            }
        }
    }

    void AnimComp::Initialize()
    {
        Renderable::Initialize();
		pTexture = ResourceManager::textures[mAtlasName].get();
    }

    void AnimComp::Shutdown()
    {
        Renderable::Shutdown();
    }

    void AnimComp::Render(Camera * cameraToUse)
    {
        //specific stuff
        if (mIsVisible == true)
        {
            if (pShader == nullptr)
                return;

            //bind the shader we want to use
            pShader->Bind();

            //needed variables
            glm::vec2 start;
            glm::vec2 end;
            GLfloat u_end;
            GLfloat u_start;
            GLfloat v_end;
            GLfloat v_start;

            //variables to know the column and the row.
            GLint current_column = mAnimCurrentFrame % CompColumns;
            GLint current_row = mAnimCurrentFrame / CompColumns;

            //Calculation of the u,v.
            u_end = (1.0f / CompColumns) * (current_column + 1);
            u_start = (1.0f / CompColumns) * current_column;
            v_start = 1.0f - (current_row + 1)*(1.0f / CompRows);
            v_end = 1.0f - (current_row)*(1.0f / CompRows);

            //pass the u-v to the shader for the lerp
            start = glm::vec2(u_start, v_start);
            end = glm::vec2(u_end, v_end);
            pShader->setUniform("StartPos", start);
            pShader->setUniform("EndPos", end);
        }

       /* if (mAnimData)
        {*/
            //pTexture = mAnimData->mAtlas;
            Renderable::Render(cameraToUse);
            //return;
        //}

        //pMesh->draw();
    }


    void AnimComp::PlayAnimation()
    {
        mbAnimPlaying = true;
    }

    void AnimComp::PauseAnimation()
    {
        mbAnimPlaying = false;
    }

    void AnimComp::ResetAnimation()
    {
        //set the animation to 0
        SetAnimationFrame(0);
    }

    void AnimComp::SetLoop(bool enable)
    {
        mbAnimLoop = enable;
    }

    void AnimComp::FlipAnimation()
    {
        pTransform->SetScale(glm::vec2(-pTransform->GetScale().x, pTransform->GetScale().y));
		flipX = !flipX;
    }

    void AnimComp::SetAnimationFrame(GLuint frameIndex)
    {
        if (static_cast<GLint>(frameIndex) > CompSprites)
            return;
		
        //Given a frame index, set the mAnimCurrentFrame to this value
        mAnimCurrentFrame = frameIndex;

        //set the texture
        //////////////////mpTex = mAnimData->mFrames[mAnimCurrentFrame];
        //timer = 0

        mAnimTimer = 0.0f;
    }

}