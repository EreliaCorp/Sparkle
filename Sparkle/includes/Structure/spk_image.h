#pragma once

#include "spk_opengl.h"

#include "Structure/spk_vector2.h"
#include "Structure/spk_shader.h"
#include "Structure/spk_shader_atlas.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "ExternalLibraries/stb_image.h"

namespace spk
{
	class Image
	{
	protected:
		GLuint _id;
		unsigned char* _data;

		int _width = 0;
		int _height = 0;
		int _nbChannels = 0;

		static inline Shader* _shader = nullptr;
		static inline Buffer* _modelSpaceBuffer = nullptr;
		static inline Buffer* _modelDepthBuffer = nullptr;
		static inline Buffer* _modelUvBuffer = nullptr;
		static inline Buffer* _indexesBuffer = nullptr;
		static inline Uniform* _textureUniform = nullptr;

		void _init_shader_data()
		{
			if (_shader == nullptr)
				_shader = ShaderAtlas::instance()->get("Texture2D");

			if (_modelSpaceBuffer == nullptr)
				_modelSpaceBuffer = _shader->buffer("model_space");
			if (_modelDepthBuffer == nullptr)
				_modelDepthBuffer = _shader->buffer("model_depth");
			if (_modelUvBuffer == nullptr)
				_modelUvBuffer = _shader->buffer("model_uvs");
			if (_indexesBuffer == nullptr)
				_indexesBuffer = _shader->elementBuffer();
			if (_textureUniform == nullptr)
				_textureUniform = _shader->uniform("textureID");
		}

	public:
		Image(GLuint p_id) : _data(nullptr)
		{
			_id = p_id;
		}
		Image(std::string path) : _data(nullptr)
		{
			_data = stbi_load(path.c_str(), &_width, &_height, &_nbChannels, 0);

			if (_data == nullptr)
			{
				std::string errorMessage = "Can't load file " + path;
				throw std::runtime_error(errorMessage.c_str());
			}

			glGenTextures(1, &_id);
			glBindTexture(GL_TEXTURE_2D, _id);
			if (_nbChannels == 3)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, _data);
			else if (_nbChannels == 4)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _data);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}

		~Image()
		{
			if (_id != -1)
				glDeleteTextures(1, &_id);
		}

		const unsigned char* data() { return reinterpret_cast<const unsigned char*>(_data); }
		const int id() { return (_id); }
		const int width() { return (_width); }
		const int height() { return (_height); }

		unsigned int pixel(unsigned int p_x, unsigned int p_y)
		{
			if (p_x > static_cast<unsigned int>(_width) || p_y > static_cast<unsigned int>(_height))
			{
				throw std::runtime_error("Pixel out of range");
			}
			unsigned char datas[4] = { 0, 0, 0, 0 };

			int index = (p_x * _nbChannels) + (_width * _nbChannels) * p_y;
			for (int comp = 0; comp < _nbChannels; comp++)
				datas[comp] = _data[index + (_nbChannels - 1 - comp)];

			if (_nbChannels == 3)
			{
				datas[3] = 0xFF;
			}

			return (*(reinterpret_cast<unsigned int*>(datas)));
		}
		unsigned int pixel(Vector2UInt p_pos) { return (pixel(p_pos.x, p_pos.y)); }

		void activate(GLenum texture = GL_TEXTURE0)
		{
			glActiveTexture(texture);
			glBindTexture(GL_TEXTURE_2D, _id);
		}
		void desactivate(GLenum texture = GL_TEXTURE0)
		{
			glActiveTexture(texture);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void draw(Vector2Int pos, Vector2Int size, Vector2 uv_pos, Vector2 uv_size, float p_depth)
		{
			static unsigned int elementIndex[6] = { 0, 3, 1, 2, 3, 0 };
			static Vector2Int deltaPos[4] = {
				Vector2Int(0, 0),
				Vector2Int(1, 0),
				Vector2Int(0, 1),
				Vector2Int(1, 1)
			};

			_init_shader_data();

			Vector2 vertexContent[4];
			float depthContent[4];
			Vector2 uvContent[4];

			for (size_t i = 0; i < 4; i++)
			{
				vertexContent[i] = spk::Window::instance()->convertScreenToOpenGL(pos + size * deltaPos[i]);
				depthContent[i] = spk::Window::instance()->convertDepthToOpenGL(p_depth);
				uvContent[i] = (uv_pos + uv_size * deltaPos[i]);
			}
			activate();

			_modelSpaceBuffer->send(vertexContent, 4);
			_modelDepthBuffer->send(depthContent, 4);
			_modelUvBuffer->send(uvContent, 4);
			_indexesBuffer->send(elementIndex, 6);
			_textureUniform->send(0);

			_shader->launch(spk::Shader::Mode::Triangle);
		}
	};
}