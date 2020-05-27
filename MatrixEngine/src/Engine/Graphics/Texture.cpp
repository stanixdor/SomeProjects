#include "Texture.h"

#include "EngineDebug/OpenGL_error.h"

#include <cstring>

#include "../../extern/stb_image/stb_image.h"

namespace Graphics
{
	Texture::Texture(const std::string &filename) : mWidth(0), mHeight(0), mPixels(nullptr), mGLHandle(0)
	{
		LoadFromFile(filename);
	}

	Texture::~Texture()
	{
		this->Unload();
	}

	void Texture::LoadFromFile(const std::string &filename)
	{
		Unload();

		int width, height, channels = 4;
		unsigned char * image = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);

		mWidth = width;
		mHeight = height;

		GLenum format = GL_RGBA;
		if (channels == 1)
			format = GL_RED;
		else if (channels == 3)
			format = GL_RGB;
		else if(channels == 4)
			format = GL_RGBA;

		if (!image)
			return;

		//std_load put the texture turned around
		imageInvertVertically(width, height, STBI_rgb_alpha, image);

		glGenTextures(1, &mGLHandle);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mGLHandle);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		//the texture object is that bound to target
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//free the data used by image
		stbi_image_free(image);
	}

	void Texture::imageInvertVertically(const unsigned width, const unsigned height, const unsigned channels, unsigned char* image)
	{
		//function for fliping the texture.
		unsigned i, j;
		for (j = 0; j * 2 < height; ++j)
		{
			unsigned index1 = j* width * channels;
			unsigned index2 = (height - 1 - j) * width * channels;
			for (i = width*channels; i > 0; --i)
			{
				unsigned char temp = image[index1];
				image[index1] = image[index2];
				image[index2] = temp;
				++index1;
				++index2;
			}
		}
	}

	void Texture::Bind()
	{
		if (mGLHandle)
			glBindTexture(GL_TEXTURE_2D, mGLHandle);
	}

	void Texture::Unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::Unload()
	{
		if (mPixels)
			delete[] mPixels;

		if (mGLHandle)
		{
			glDeleteTextures(1, (GLuint*)(&mGLHandle)); // delete
			mGLHandle = 0;
		}
	}
}