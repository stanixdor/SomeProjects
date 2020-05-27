//Mikel Fernandez
#pragma once

#include "../GL/glew.h"
#include <string>

namespace Graphics
{
	class Texture
	{
	public:
		//Constructor
		Texture(const std::string &filename);

		Texture(const Texture &) = delete;
		Texture & operator=(const Texture &) = delete;

		//Destructor.
		virtual ~Texture();

		// Load from file
		void LoadFromFile(const std::string &filename);
		void imageInvertVertically(const unsigned width, const unsigned height, const unsigned channels, unsigned char* image);

		void Bind();
		void Unbind();

		void Unload();

		//settors
		/*void SetWidth(GLfloat width) { mWidth = width; }
		void SetHeight(GLfloat height) { mHeight = height; }*/

		// gettors
		GLint GetWidth() { return mWidth; }
		GLint GetHeight() { return mHeight; }
		unsigned char * GetPixels(unsigned int x = 0, unsigned int y = 0) { return mPixels + ((mWidth * y + x) * 4); }
		GLint GetGLHandle() { return mGLHandle; }

	private:
		GLint mWidth;
		GLint mHeight;
		GLuint mGLHandle;
		unsigned char * mPixels;
	};
}