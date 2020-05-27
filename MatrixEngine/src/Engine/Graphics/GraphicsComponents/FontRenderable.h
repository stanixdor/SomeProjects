//Mikel Fernandez

#pragma once

#include "Renderable.h"
#include "../FontRenderingInfo.h"

namespace Graphics
{
	class FontRenderer : public Renderable
	{
	public:
		SERIALIZABLE;
		FontRenderer();
		virtual ~FontRenderer();

		virtual void Initialize() override;
		virtual void Shutdown() override;
		virtual void Render(Camera * cameraToUse) override;
 
		void CreateTextMesh(FontInfo * fontInfo, const char * Text);


		//settors
		void setText(std::string txt)
		{
			text = txt;
			CreateTextMesh(font_, text.c_str());
		}

		void setFont(FontInfo * fontinfo)
		{
			font_ = fontinfo;
		}

		void setSpace(int val) { space = val; }
		void setLastChar(int val) { lastChar = val; }
		void setWidth(int val) { width = val; }
		void setHeight(int val) { height = val; }
		void setRasSize(float val) { sizeRasterizing = val; }

		//gettors
		std::string getText() { return text; }
		FontInfo * getFontInfo() { return font_; }
		int getSpace() { return space; }
		int getLastChar() { return lastChar; }
		int getWidth() { return width; }
		int getHeight() { return height; }
		float getRasSize() { return sizeRasterizing; }


		std::string text;
		int space = 32;
		int lastChar = 197;
		int width = 256;
		int height = 64;
		float sizeRasterizing = 13;

	private:
		FontInfo * font_;
		//glm::vec4 textColor_ = glm::vec4(1.f, 1.f, 1.f, 1.f);
		//std::string fontName;
		//glm::vec2 scale;

	};
}