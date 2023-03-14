#pragma once

#include "Structure/spk_vector2.h"
#include "spk_opengl.h"

#include "spk_image.h"

#include "Structure/spk_window.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_STATIC
#include "ExternalLibraries/stb_image_write.h"

namespace spk
{
	class ImageOutput
	{
	private:
		Vector2Int _size;
		GLuint _frameBufferObject;
		GLuint _outputTexture;
		GLuint _depthBuffer;

	public:
		ImageOutput(Vector2Int p_size)
		{
			_size = p_size;

			glGenTextures(1, &_outputTexture);
			glBindTexture(GL_TEXTURE_2D, _outputTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _size.x, _size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glGenRenderbuffers(1, &_depthBuffer);
			glBindRenderbuffer(GL_RENDERBUFFER, _depthBuffer);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, _size.x, _size.y);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);

			glGenFramebuffers(1, &_frameBufferObject);
			glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferObject);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _outputTexture, 0);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthBuffer);

			desassociate();
		}

		ImageOutput(unsigned int p_width, unsigned int p_heigth) : ImageOutput(Vector2UInt(p_width, p_heigth))
		{

		}

		~ImageOutput()
		{
			glDeleteFramebuffers(1, &_frameBufferObject);
			glDeleteTextures(1, &_outputTexture);
			glDeleteRenderbuffers(1, &_depthBuffer);
		}

		void associate()
		{
			spk::Window::instance()->setViewport(Vector2Int(_size.x, _size.y));
			glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferObject);
			clear();
		}

		void clear()
		{
			glViewport(0, 0, _size.x, _size.y);
			glScissor(0, 0, _size.x, _size.y);
			glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void desassociate()
		{
			spk::Window::instance()->reset();
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		Image* save()
		{
			return (new Image(saveToOpenGLTexture()));
		}
		GLuint saveToOpenGLTexture()
		{
			GLuint result;

			glGenTextures(1, &result);
			glBindTexture(GL_TEXTURE_2D, result);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _size.x, _size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glCopyImageSubData(_outputTexture, GL_TEXTURE_2D, 0, 0, 0, 0,
				result, GL_TEXTURE_2D, 0, 0, 0, 0,
				_size.x, _size.y, 1);

			return (result);
		}

		void saveToFile(std::string p_path)
		{
			throw std::runtime_error("Saving to file not implemented");
		}

		const Vector2Int& size() const { return (_size); }
	};
}