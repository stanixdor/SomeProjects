
#include "FontRenderable.h"

#include "Engine\Graphics\GraphicsSystem.h"
#include "../Mesh.h"
#include "Transform\Transform2D.h"
#include "Engine\EngineDebug\OpenGL_error.h"

#include "Platform\Resource Manager\ResourceManager.h"

namespace Graphics
{
	REFLECTION(FontRenderer, text, originalColor);
	FontRenderer::FontRenderer()
	{
		m_name = "Font Renderer";
		pShader = GraphicsSystem::Instance()->mPrograms[3];

		pMesh = new Mesh();
		pMesh->Clear_Vectors();
		pMesh->Init(true);

		font_ = new FontInfo();
		font_->mFontBitmap = ResourceManager::textures[mTextureName].get();
		text = "Enter Text";
	}

	FontRenderer::~FontRenderer()
	{
		delete font_;
	}

	void FontRenderer::Initialize()
	{
		Renderable::Initialize();

		const char * gTTFFilePath = "Data/FontData/data/sources/ProggyClean.ttf";
		const char * gFontTexFilePath = "Data/FontData/data/ProggyClean.png";
		
		LoadTTF(gTTFFilePath,
			space, // first character 32 = space
			lastChar, // last character 197
			width, // output texture width
			height,	 // output texture height
			sizeRasterizing,  // font size when rasterizing
			font_, // font info filled with necessary data to construct mesh. 
			true,	// save to texture dlft is texture with same name in same directory.
			gFontTexFilePath); // texture file path if saving (optional)

		pTexture = font_->mFontBitmap;

		CreateTextMesh(font_, text.c_str());
	}

	void FontRenderer::Shutdown()
	{
		Renderable::Shutdown();
	}

	void FontRenderer::Render(Camera * cameraToUse)
	{
		Renderable::Render(cameraToUse);
	}



	// ------------------------------------------------------------------------
	// \fn		CreateTextMesh
	// \brief	Given a FontInfo class and a text to display, this function 
	//			will generate a triangle mesh  such that:
	//			- The mesh is made up of quads (one for each character in 'text')
	//			- Each quad has proper texture coordinates into the font texture. 
	//			- Each quad has proper spacing applied to it based on FontInfo's 
	//			  data for spacing and kerning. 
	//
	//	\param	fontInfo	FontInfo class (correctly initialized).
	//	\param	text		Source string to generate the mesh from.
	void FontRenderer::CreateTextMesh(FontInfo * fontInfo, const char * Text)
	{
		if (fontInfo == nullptr || Text == nullptr)
			return;

		if(pMesh)
			delete pMesh;
		pMesh = new Graphics::Mesh();
		pMesh->Clear_Vectors();
		pMesh->Init(true);

		if (pMesh)
		{
			// start position - bottom left
			glm::vec2 startPos = { 0, 0 };

			// we will create a quad for each letter
			size_t len = strlen(Text);
			int loop_counter = 0;
			while (loop_counter < len)
			{
				// get the character
				int char_index = *(Text + loop_counter);

				// new line
				if (char_index == '\n')
				{
					startPos.y -= fontInfo->mLineGap;
					startPos.x = 0;
					loop_counter++;
					continue;
				}

				// get the glyph index in the font info
				int glyph_index = char_index - fontInfo->mFirstCharacterCode;

				// get the glyp
				Glyph & glyph = fontInfo->mGlyphContainer[glyph_index];

				// make a quad
				glm::vec2 positions[6];
				glm::vec2 texCoord[6];

				positions[0] = startPos + glm::vec2(glyph.mX0, glyph.mY1);
				positions[1] = startPos + glm::vec2(glyph.mX0, glyph.mY0);
				positions[2] = startPos + glm::vec2(glyph.mX1, glyph.mY0);

				positions[3] = startPos + glm::vec2(glyph.mX0, glyph.mY1);
				positions[4] = startPos + glm::vec2(glyph.mX1, glyph.mY0);
				positions[5] = startPos + glm::vec2(glyph.mX1, glyph.mY1);

				// Texture coordinates
				texCoord[0] = glm::vec2(glyph.mU0, glyph.mV1);
				texCoord[1] = glm::vec2(glyph.mU0, glyph.mV0);
				texCoord[2] = glm::vec2(glyph.mU1, glyph.mV0);

				texCoord[3] = glm::vec2(glyph.mU0, glyph.mV1);
				texCoord[4] = glm::vec2(glyph.mU1, glyph.mV0);
				texCoord[5] = glm::vec2(glyph.mU1, glyph.mV1);

				// add to model & set default color
				for (unsigned i = 0; i < 6; ++i) {
					pMesh->GetPosition().push_back(positions[i]);
					pMesh->GetTexCoord().push_back(texCoord[i]);
				}

				// advance 
				startPos.x += glyph.mAdvanceWidth;

				// get kerning
				if (Text + loop_counter + 1)
				{
					int next_char_index = *(Text + loop_counter + 1);
					int next_glyph_index = next_char_index - fontInfo->mFirstCharacterCode;

					float kern_advance = fontInfo->mKerningTable[glyph_index][next_glyph_index];

					startPos.x += kern_advance;
				}

				// next character
				loop_counter++;
			}
		}
		pMesh->Bind();

		{

			glBindBuffer(GL_ARRAY_BUFFER, pMesh->mBufferObjects[0]);
			//creates and initializes mtexcoords's data store.

			glBufferData(GL_ARRAY_BUFFER, GLsizeiptr(pMesh->mPositionList.size() * sizeof(GLfloat) * 2.0f), pMesh->mPositionList.data(), GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, pMesh->mBufferObjects[1]);
			//creates and initializes mtexcoords's data store.

			glBufferData(GL_ARRAY_BUFFER, GLsizeiptr(pMesh->mTexCoordList.size() * sizeof(GLfloat) * 2.0f), pMesh->mTexCoordList.data(), GL_DYNAMIC_DRAW);

			//pMesh->mPositionList.resize(pMesh->mPositionList.size());
			pMesh->mIndexes.resize(pMesh->mPositionList.size());
			for (GLint i = 0; i < pMesh->mIndexes.size(); ++i)
				pMesh->mIndexes[i] = i;

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pMesh->mBufferObjects[2]);
			//creates and initializes mtexcoords's data store.
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, GLsizeiptr(pMesh->mIndexes.size() * sizeof(GLuint)), pMesh->mIndexes.data(), GL_DYNAMIC_DRAW);
		}
		gl_error();
		//pMesh->Init();
	}
}
