#pragma once

#include "Structure/spk_vector2.h"
#include "Structure/spk_window.h"
#include "Structure/spk_shader_atlas.h"

#ifndef STB_TRUETYPE_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#define STBTT_STATIC
#include "ExternalLibraries/stb_truetype.h"
#endif

namespace spk
{
	class Font
	{
	public:
		struct GlyphData
		{
			Vector2 positions[4];
			Vector2 uvs[4];
			Vector2 size = Vector2(0, 0);
			Vector2 step = Vector2(0, 0);
			Vector2 offset = Vector2(0, 0);
		};
		struct FontData
		{
			size_t textSize;
			size_t outlineSize;

			FontData(size_t p_textSize, size_t p_outlineSize)
			{
				textSize = p_textSize;
				outlineSize = p_outlineSize;
			}
			bool operator < (const FontData& p_other) const
			{
				if (textSize < p_other.textSize)
					return (true);
				if (textSize == p_other.textSize && outlineSize < p_other.outlineSize)
					return (true);
				return (false);
			}
		};
		struct FontGlyphData
		{
			Vector2Int size;
			std::vector<GlyphData> atlas;
			GLuint id;
		};

	private:
		static inline Shader* _shader = nullptr;

		static inline Buffer* _modelSpaceBuffer = nullptr;
		static inline Buffer* _modelDepthBuffer = nullptr;
		static inline Buffer* _modelUvBuffer = nullptr;
		static inline Buffer* _indexesBuffer = nullptr;
		static inline std::vector<Vector2> _modelSpaceData = std::vector<Vector2>();
		static inline std::vector<float> _modelDepthData = std::vector<float>();
		static inline std::vector<Vector2> _modelUvData = std::vector<Vector2>();
		static inline std::vector<unsigned int> _indexesData = std::vector<unsigned int>();
		static inline Uniform* _textureUniform = nullptr;
		static inline Uniform* _textColorUniform = nullptr;
		static inline Uniform* _outlineColorUniform = nullptr;
		static inline Uniform* _outlineSizeUniform = nullptr;

		std::recursive_mutex _mutex;
		unsigned char* _fontBuffer;
		std::map<FontData, FontGlyphData> _fontGlyphDatas;

		void _loadFontTexture(Font::FontData p_fontData)
		{
			const size_t nb_char = 256;

			unsigned char* atlasData = nullptr;

			size_t width = 32 + p_fontData.outlineSize * 2;
			size_t height = 32 + p_fontData.outlineSize * 2;

			stbtt_packedchar* char_info = new stbtt_packedchar[nb_char];

			while (1) {
				atlasData = new unsigned char[width * height];

				stbtt_pack_context context;

				if (!stbtt_PackBegin(&context,
									atlasData,
									static_cast<int>(width),
									static_cast<int>(height),
									0,
									static_cast<int>(p_fontData.outlineSize * 2),
									nullptr))
					throw std::runtime_error("Failed to initialize font");

				stbtt_PackSetOversampling(&context, 1, 1);
				if (!stbtt_PackFontRange(&context, _fontBuffer, 0, p_fontData.textSize, 0, nb_char, char_info)) {
					// too small
					delete[] atlasData;
					stbtt_PackEnd(&context);
					width *= 2;
					height *= 2;
				}
				else {
					stbtt_PackEnd(&context);
					break;
				}
			}

			FontGlyphData fontGlyphData;

			fontGlyphData.size = spk::Vector2Int(static_cast<int>(width), static_cast<int>(height));
			float width_delta = 0.5f / width;
			float height_delta = 0.5f / height;
			float offsetUvsX = (1.0f / width) * p_fontData.outlineSize;
			float offsetUvsY = (1.0f / height) * p_fontData.outlineSize;

			fontGlyphData.atlas.resize(nb_char + 1);
			for (size_t i = 32; i < nb_char; i++)
			{
				unsigned char c = static_cast<unsigned char>(i);
				GlyphData data;

				stbtt_aligned_quad quad;

				stbtt_GetPackedQuad(char_info, static_cast<int>(width), static_cast<int>(height), c, &data.step.x, &data.step.y, &quad, 1);

				data.step.x += p_fontData.outlineSize;

				const float xmin = quad.x0;
				const float xmax = quad.x1 + p_fontData.outlineSize * 2;
				const float ymin = quad.y0;
				const float ymax = quad.y1 + p_fontData.outlineSize * 2;

				data.positions[0] = Vector2(xmin, ymin);
				data.positions[1] = Vector2(xmax, ymin);
				data.positions[2] = Vector2(xmin, ymax);
				data.positions[3] = Vector2(xmax, ymax);
				data.size = Vector2Int(xmax - xmin, ymax - ymin);
				data.offset = Vector2(-xmin, -ymax);

				data.uvs[0] = { quad.s0 - offsetUvsX, quad.t0 - offsetUvsY };
				data.uvs[1] = { quad.s1 + offsetUvsX, quad.t0 - offsetUvsY };
				data.uvs[2] = { quad.s0 - offsetUvsX, quad.t1 + offsetUvsY };
				data.uvs[3] = { quad.s1 + offsetUvsX, quad.t1 + offsetUvsY };

				fontGlyphData.atlas[c] = data;
			}

			for (size_t y = 0; y < height; y++)
			{
				for (size_t x = 0; x < width; x++)
				{
					size_t index = x + y * width;
					if (atlasData[index] != 0x00)
					{
						atlasData[index] = 0xFF;
					}
				}
			}

			glGenTextures(1, &fontGlyphData.id);
			glBindTexture(GL_TEXTURE_2D, fontGlyphData.id);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, static_cast<GLsizei>(width), static_cast<GLsizei>(height), 0, GL_RED, GL_UNSIGNED_BYTE, atlasData);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			_fontGlyphDatas[p_fontData] = fontGlyphData;

			delete[] atlasData;
			delete[] char_info;
		}

		void _initCharRender()
		{
			_modelSpaceData.clear();
			_modelUvData.clear();
			_modelDepthData.clear();
			_indexesData.clear();
		}
		Vector2Int _prepareCharRender(FontGlyphData& p_fontGlyphData, unsigned char p_char, Vector2Int p_pos, unsigned int p_size, size_t p_outlineSize, float p_depth)
		{
			if (p_char < 32)
				return (Vector2Int(0, 0));

			static unsigned int element_index[6] = { 0, 3, 1, 2, 3, 0 };
			static Vector2Int delta_pos[4] = {
				Vector2Int(0, 0),
				Vector2Int(1, 0),
				Vector2Int(0, 1),
				Vector2Int(1, 1)
			};

			Font::GlyphData& glyphData = getData(p_fontGlyphData, static_cast<unsigned char>(p_char));
			Vector2 BaseGlyphSize = _calcCharSize(p_fontGlyphData, p_char);
			Vector2Int glyphSize = glyphData.size;

			size_t elementSize = static_cast<size_t>(_modelSpaceData.size());
			for (size_t i = 0; i < 4; i++)
			{
				Vector2Int tmp_pos = p_pos + glyphSize * delta_pos[i];
				_modelSpaceData.push_back(spk::Window::instance()->convertScreenToOpenGL(tmp_pos));
				_modelDepthData.push_back(spk::Window::instance()->convertDepthToOpenGL(p_depth));
				_modelUvData.push_back(glyphData.uvs[i]);
			}

			for (size_t i = 0; i < 6; i++)
			{
				_indexesData.push_back(static_cast<unsigned int>(elementSize + element_index[i]));
			}

			return (glyphData.step);
		}
		void _castCharRender(GLuint p_id, Color p_textColor, unsigned int p_outlineSize, Color p_outlineColor)
		{
			exportShaderData(_modelSpaceBuffer, _modelDepthBuffer, _modelUvBuffer, _indexesBuffer);

			_shader->activate();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, p_id);

			_textureUniform->send(0);
			_textColorUniform->send(p_textColor);
			_outlineColorUniform->send(p_outlineColor);
			_outlineSizeUniform->send(p_outlineSize);

			_shader->launch(Shader::Mode::Triangle);
		}

		void _initShaderData()
		{
			if (_shader == nullptr)
				_shader = spk::ShaderAtlas::instance()->get("TextTexture2D");

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
			if (_textColorUniform == nullptr)
				_textColorUniform = _shader->uniform("textColor");
			if (_outlineColorUniform == nullptr)
				_outlineColorUniform = _shader->uniform("outlineColor");
			if (_outlineSizeUniform == nullptr)
				_outlineSizeUniform = _shader->uniform("outlineSize");
		}

		Vector2 _calcCharSize(FontGlyphData& p_fontGlyphData, unsigned char p_char)
		{
			Vector2Int minValue = Vector2Int(0, 0);
			Vector2Int maxValue = Vector2Int(0, 0);

			Font::GlyphData& glyphData = getData(p_fontGlyphData, static_cast<unsigned char>(p_char));

			maxValue.x += glyphData.size.x;

			if (minValue.y > glyphData.positions[0].y)
				minValue.y = glyphData.positions[0].y;

			if (maxValue.y < glyphData.positions[3].y)
				maxValue.y = glyphData.positions[3].y;

			return (maxValue - minValue);
		}
		Vector2Int _calcLineOffset(FontGlyphData& p_fontGlyphData, std::string p_string)
		{
			spk::Vector2Int result = spk::Vector2Int(0, 0);

			for (size_t i = 0; i < p_string.size(); i++)
			{
				Font::GlyphData& glyphData = getData(p_fontGlyphData, static_cast<unsigned char>(p_string[i]));

				if (result.y > glyphData.positions[0].y)
				{
					result.y = glyphData.positions[0].y;
				}
			}

			return (result);
		}

	public:
		Font(std::string p_path)
		{
			long size;

			FILE* fontFile = NULL;

			_initShaderData();

			errno_t err = fopen_s(
				&fontFile,
				p_path.c_str(), "rb"
			);

			if (fontFile == NULL)
			{
				std::string error = std::string("Can't open file ") + p_path.c_str();
				throw std::runtime_error("Can't open file ");
			}

			fseek(fontFile, 0, SEEK_END);
			size = ftell(fontFile); /* how long is the file ? */
			fseek(fontFile, 0, SEEK_SET); /* reset */

			_fontBuffer = (unsigned char*)malloc(size * sizeof(unsigned char));

			fread(_fontBuffer, size, 1, fontFile);
			fclose(fontFile);
		}
		~Font()
		{
			//glDeleteTextures(1, &_id);
		}

		FontGlyphData& getFontGlyphData(size_t p_textSize, size_t p_outlineSize)
		{
			Font::FontData tmp(p_textSize, p_outlineSize);

			if (_fontGlyphDatas.count(tmp) == 0)
			{
				_loadFontTexture(tmp);
			}
			auto& result = _fontGlyphDatas[tmp];
			return (result);
		}
		GlyphData& getData(size_t p_textSize, size_t p_outlineSize, unsigned char p_char)
		{
			FontGlyphData fontGlyphData = getFontGlyphData(p_textSize, p_outlineSize);

			if (fontGlyphData.atlas.size() < p_char)
			{
				std::string errorMessage = std::string("Trying to print an unexpected char [") + static_cast<char>(p_char) + "](" + std::to_string(static_cast<long>(p_char)) + ")";
				throw std::runtime_error(errorMessage.c_str());
			}
			return (fontGlyphData.atlas[p_char]);
		}
		GlyphData& getData(FontGlyphData& p_fontGlyphData, unsigned char p_char)
		{
			if (p_fontGlyphData.atlas.size() < p_char)
			{
				std::string errorMessage = std::string("Trying to print an unexpected char [") + static_cast<char>(p_char) + "](" + std::to_string(static_cast<long>(p_char)) + ")";
				throw std::runtime_error(errorMessage.c_str());
			}
			return (p_fontGlyphData.atlas[p_char]);
		}

		size_t computeTextSize(std::string p_string, size_t p_outlineSize, Vector2Int p_textArea) {
			std::vector<int> deltas = { 100, 50, 20, 10, 1 };
			size_t result = 2;

			if (p_string == "")
				return (p_textArea.y);

			for (size_t i = 0; i < deltas.size(); i++)
			{
				bool enough = false;
				while (enough == false)
				{
					Vector2Int tmp_size = calcStringSize(p_string, static_cast<unsigned int>(result + deltas[i]), p_outlineSize);
					if (tmp_size.x >= p_textArea.x || tmp_size.y >= p_textArea.y)
						enough = true;
					else
						result += deltas[i];
				}
			}
			return (result);
		}

		GLuint textureID(size_t p_textSize, size_t p_outlineSize) {
			auto& tmp = getFontGlyphData(p_textSize, p_outlineSize);
			return (tmp.id);
		}

		void exportShaderData(Buffer* p_modelSpaceBuffer, Buffer* p_modelDepthBuffer, Buffer* p_modelUvBuffer, Buffer* p_indexesBuffer)
		{
			if (p_modelSpaceBuffer != nullptr)
				p_modelSpaceBuffer->send(_modelSpaceData.data(), static_cast<size_t>(_modelSpaceData.size()));
			if (p_modelDepthBuffer != nullptr)
				p_modelDepthBuffer->send(_modelDepthData.data(), static_cast<size_t>(_modelDepthData.size()));
			if (p_modelUvBuffer != nullptr)
				p_modelUvBuffer->send(_modelUvData.data(), static_cast<size_t>(_modelUvData.size()));
			if (p_indexesBuffer != nullptr)
				p_indexesBuffer->send(_indexesData.data(), static_cast<size_t>(_indexesData.size()));
		}

		Vector2Int calcCharSize(unsigned char p_char, unsigned int p_size, size_t p_outlineSize)
		{
			FontGlyphData& fontGlyphData = getFontGlyphData(p_size, 0);

			return (_calcCharSize(fontGlyphData, p_char));
		}
		Vector2Int calcStringSize(std::string p_string, unsigned int p_size, size_t p_outlineSize)
		{
			Vector2Int minValue = Vector2Int(0, 0);
			Vector2Int maxValue = Vector2Int(0, 0);

			FontGlyphData& fontGlyphData = getFontGlyphData(p_size, p_outlineSize);

			for (size_t i = 0; i < p_string.size(); i++)
			{
				Font::GlyphData& glyphData = getData(fontGlyphData, static_cast<unsigned char>(p_string[i]));

				maxValue.x += glyphData.step.x;

				if (minValue.y > glyphData.positions[0].y)
					minValue.y = glyphData.positions[0].y;

				if (maxValue.y < glyphData.positions[3].y)
					maxValue.y = glyphData.positions[3].y;
			}

			return (maxValue - minValue);
		}

		Vector2Int draw(std::string p_string, Vector2Int p_pos, unsigned int p_size, Color p_color, float p_depth)
		{
			return (draw(p_string, p_pos, p_size, p_color, 0, p_color, p_depth));
		}
		Vector2Int draw(std::string p_string, Vector2Int p_pos, unsigned int p_size, Color p_color, size_t p_outlineSize, Color p_outlineColor, float p_depth)
		{
			FontGlyphData& fontGlyphData = getFontGlyphData(p_size, p_outlineSize);

			Vector2Int result = prepareDraw(p_string, p_pos, p_size, p_outlineSize, p_depth);

			_castCharRender(fontGlyphData.id, p_color, static_cast<unsigned int>(p_outlineSize), p_outlineColor);

			return (result);
		}

		Vector2Int prepareDraw(std::string p_string, Vector2Int p_pos, unsigned int p_size, float p_depth)
		{
			return (prepareDraw(p_string, p_pos, p_size, 0, p_depth));
		}
		Vector2Int prepareDraw(std::string p_string, Vector2Int p_pos, unsigned int p_size, size_t p_outlineSize, float p_depth)
		{
			_initCharRender();
			FontGlyphData& fontGlyphData = getFontGlyphData(p_size, p_outlineSize);

			Vector2Int result = Vector2Int(0, 0);
			Vector2Int lineOffset = _calcLineOffset(fontGlyphData, p_string);

			for (size_t i = 0; i < p_string.size(); i++)
			{
				Font::GlyphData& glyphData = getData(fontGlyphData, static_cast<unsigned char>(p_string[i]));

				spk::Vector2Int charOffset = spk::Vector2Int(0, -lineOffset.y - glyphData.size.y);

				spk::Vector2Int charPosition = p_pos + result + charOffset - glyphData.offset;

				Vector2Int tmp = _prepareCharRender(
					fontGlyphData,
					p_string[i],
					charPosition,
					p_size,
					p_outlineSize,
					p_depth
				);

				result.x += tmp.x;
			}

			return (result);
		}
	};
}