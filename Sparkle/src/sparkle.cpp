#include "sparkle.h"

namespace spk
{
	thread_local IOStream cout = IOStream(std::cout);
	thread_local IOStream cerr = IOStream(std::cerr);

	const Color Color::transparent = Color(0.0f, 0.0f, 0.0f, 0.0f);
	const Color Color::white = Color(1.0f, 1.0f, 1.0f, 1.0f);
	const Color Color::black = Color(0.0f, 0.0f, 0.0f, 1.0f);
	const Color Color::red = Color(1.0f, 0.0f, 0.0f, 1.0f);
	const Color Color::green = Color(0.0f, 1.0f, 0.0f, 1.0f);
	const Color Color::blue = Color(0.0f, 0.0f, 1.0f, 1.0f);

	std::string glGetTypeString(GLenum type)
	{
		switch (type)
		{
		case GL_NO_ERROR: return ("No error");
		case GL_INVALID_ENUM: return ("Invalid enum");
		case GL_INVALID_VALUE: return ("Invalid value");
		case GL_INVALID_OPERATION: return ("Invalid operation");
		case GL_INVALID_FRAMEBUFFER_OPERATION: return ("Invalid framebuffer operation");
		case GL_OUT_OF_MEMORY: return ("Out of memory");
		case GL_STACK_UNDERFLOW: return ("Stack underflow");
		case GL_STACK_OVERFLOW: return ("Stack overflow");

		case GL_ELEMENT_ARRAY_BUFFER: return ("GL_ELEMENT_ARRAY_BUFFER");
		case GL_ARRAY_BUFFER: return ("GL_ARRAY_BUFFER");
		case GL_BYTE: return ("GLbyte");
		case GL_UNSIGNED_BYTE: return ("GLubyte");
		case GL_UNSIGNED_SHORT: return ("GLushort");
		case GL_2_BYTES: return ("GL_2_BYTES");
		case GL_3_BYTES: return ("GL_3_BYTES");
		case GL_4_BYTES: return ("GL_4_BYTES");

		case 	GL_FLOAT: return ("float");
		case 	GL_FLOAT_VEC2: return ("vec2");
		case 	GL_FLOAT_VEC3: return ("vec3");
		case 	GL_FLOAT_VEC4: return ("vec4");
		case 	GL_DOUBLE: return ("double");
		case 	GL_DOUBLE_VEC2: return ("dvec2");
		case 	GL_DOUBLE_VEC3: return ("dvec3");
		case 	GL_DOUBLE_VEC4: return ("dvec4");
		case 	GL_INT: return ("int");
		case 	GL_INT_VEC2: return ("ivec2");
		case 	GL_INT_VEC3: return ("ivec3");
		case 	GL_INT_VEC4: return ("ivec4");
		case 	GL_UNSIGNED_INT: return ("unsigned int");
		case 	GL_UNSIGNED_INT_VEC2: return ("uvec2");
		case 	GL_UNSIGNED_INT_VEC3: return ("uvec3");
		case 	GL_UNSIGNED_INT_VEC4: return ("uvec4");
		case 	GL_BOOL: return ("bool");
		case 	GL_BOOL_VEC2: return ("bvec2");
		case 	GL_BOOL_VEC3: return ("bvec3");
		case 	GL_BOOL_VEC4: return ("bvec4");
		case 	GL_FLOAT_MAT2: return ("mat2");
		case 	GL_FLOAT_MAT3: return ("mat3");
		case 	GL_FLOAT_MAT4: return ("mat4");
		case 	GL_FLOAT_MAT2x3: return ("mat2x3");
		case 	GL_FLOAT_MAT2x4: return ("mat2x4");
		case 	GL_FLOAT_MAT3x2: return ("mat3x2");
		case 	GL_FLOAT_MAT3x4: return ("mat3x4");
		case 	GL_FLOAT_MAT4x2: return ("mat4x2");
		case 	GL_FLOAT_MAT4x3: return ("mat4x3");
		case 	GL_DOUBLE_MAT2: return ("dmat2");
		case 	GL_DOUBLE_MAT3: return ("dmat3");
		case 	GL_DOUBLE_MAT4: return ("dmat4");
		case 	GL_DOUBLE_MAT2x3: return ("dmat2x3");
		case 	GL_DOUBLE_MAT2x4: return ("dmat2x4");
		case 	GL_DOUBLE_MAT3x2: return ("dmat3x2");
		case 	GL_DOUBLE_MAT3x4: return ("dmat3x4");
		case 	GL_DOUBLE_MAT4x2: return ("dmat4x2");
		case 	GL_DOUBLE_MAT4x3: return ("dmat4x3");
		case 	GL_SAMPLER_1D: return ("sampler1D");
		case 	GL_SAMPLER_2D: return ("sampler2D");
		case 	GL_SAMPLER_3D: return ("sampler3D");
		case 	GL_SAMPLER_CUBE: return ("samplerCube");
		case 	GL_SAMPLER_1D_SHADOW: return ("sampler1DShadow");
		case 	GL_SAMPLER_2D_SHADOW: return ("sampler2DShadow");
		case 	GL_SAMPLER_1D_ARRAY: return ("sampler1DArray");
		case 	GL_SAMPLER_2D_ARRAY: return ("sampler2DArray");
		case 	GL_SAMPLER_1D_ARRAY_SHADOW: return ("sampler1DArrayShadow");
		case 	GL_SAMPLER_2D_ARRAY_SHADOW: return ("sampler2DArrayShadow");
		case 	GL_SAMPLER_2D_MULTISAMPLE: return ("sampler2DMS");
		case 	GL_SAMPLER_2D_MULTISAMPLE_ARRAY: return ("sampler2DMSArray");
		case 	GL_SAMPLER_CUBE_SHADOW: return ("samplerCubeShadow");
		case 	GL_SAMPLER_BUFFER: return ("samplerBuffer");
		case 	GL_SAMPLER_2D_RECT: return ("sampler2DRect");
		case 	GL_SAMPLER_2D_RECT_SHADOW: return ("sampler2DRectShadow");
		case 	GL_INT_SAMPLER_1D: return ("isampler1D");
		case 	GL_INT_SAMPLER_2D: return ("isampler2D");
		case 	GL_INT_SAMPLER_3D: return ("isampler3D");
		case 	GL_INT_SAMPLER_CUBE: return ("isamplerCube");
		case 	GL_INT_SAMPLER_1D_ARRAY: return ("isampler1DArray");
		case 	GL_INT_SAMPLER_2D_ARRAY: return ("isampler2DArray");
		case 	GL_INT_SAMPLER_2D_MULTISAMPLE: return ("isampler2DMS");
		case 	GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: return ("isampler2DMSArray");
		case 	GL_INT_SAMPLER_BUFFER: return ("isamplerBuffer");
		case 	GL_INT_SAMPLER_2D_RECT: return ("isampler2DRect");
		case 	GL_UNSIGNED_INT_SAMPLER_1D: return ("usampler1D");
		case 	GL_UNSIGNED_INT_SAMPLER_2D: return ("usampler2D");
		case 	GL_UNSIGNED_INT_SAMPLER_3D: return ("usampler3D");
		case 	GL_UNSIGNED_INT_SAMPLER_CUBE: return ("usamplerCube");
		case 	GL_UNSIGNED_INT_SAMPLER_1D_ARRAY: return ("usampler2DArray");
		case 	GL_UNSIGNED_INT_SAMPLER_2D_ARRAY: return ("usampler2DArray");
		case 	GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE: return ("usampler2DMS");
		case 	GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: return ("usampler2DMSArray");
		case 	GL_UNSIGNED_INT_SAMPLER_BUFFER: return ("usamplerBuffer");
		case 	GL_UNSIGNED_INT_SAMPLER_2D_RECT: return ("usampler2DRect");
		case 	GL_IMAGE_1D: return ("image1D");
		case 	GL_IMAGE_2D: return ("image2D");
		case 	GL_IMAGE_3D: return ("image3D");
		case 	GL_IMAGE_2D_RECT: return ("image2DRect");
		case 	GL_IMAGE_CUBE: return ("imageCube");
		case 	GL_IMAGE_BUFFER: return ("imageBuffer");
		case 	GL_IMAGE_1D_ARRAY: return ("image1DArray");
		case 	GL_IMAGE_2D_ARRAY: return ("image2DArray");
		case 	GL_IMAGE_2D_MULTISAMPLE: return ("image2DMS");
		case 	GL_IMAGE_2D_MULTISAMPLE_ARRAY: return ("image2DMSArray");
		case 	GL_INT_IMAGE_1D: return ("iimage1D");
		case 	GL_INT_IMAGE_2D: return ("iimage2D");
		case 	GL_INT_IMAGE_3D: return ("iimage3D");
		case 	GL_INT_IMAGE_2D_RECT: return ("iimage2DRect");
		case 	GL_INT_IMAGE_CUBE: return ("iimageCube");
		case 	GL_INT_IMAGE_BUFFER: return ("iimageBuffer");
		case 	GL_INT_IMAGE_1D_ARRAY: return ("iimage1DArray");
		case 	GL_INT_IMAGE_2D_ARRAY: return ("iimage2DArray");
		case 	GL_INT_IMAGE_2D_MULTISAMPLE: return ("iimage2DMS");
		case 	GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY: return ("iimage2DMSArray");
		case 	GL_UNSIGNED_INT_IMAGE_1D: return ("uimage1D");
		case 	GL_UNSIGNED_INT_IMAGE_2D: return ("uimage2D");
		case 	GL_UNSIGNED_INT_IMAGE_3D: return ("uimage3D");
		case 	GL_UNSIGNED_INT_IMAGE_2D_RECT: return ("uimage2DRect");
		case 	GL_UNSIGNED_INT_IMAGE_CUBE: return ("uimageCube");
		case 	GL_UNSIGNED_INT_IMAGE_BUFFER: return ("uimageBuffer");
		case 	GL_UNSIGNED_INT_IMAGE_1D_ARRAY: return ("uimage1DArray");
		case 	GL_UNSIGNED_INT_IMAGE_2D_ARRAY: return ("uimage2DArray");
		case 	GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE: return ("uimage2DMS");
		case 	GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY: return ("uimage2DMSArray");
		case 	GL_UNSIGNED_INT_ATOMIC_COUNTER: return ("atomic_uint");


		default: return ("not a GLenum type");
		}
	}

	void checkOpengl(std::string msg)
	{
		GLenum err = glGetError();
		if (err != GL_NO_ERROR)
		{
			std::string error = msg + " : type [" + glGetTypeString(err) + "]";
			cout << "Error opengl : " << error << std::endl;
			throw std::runtime_error(error.c_str());
		}
	}

	std::string methodName(const std::string& prettyFunction)
	{
		size_t end = prettyFunction.rfind("(");
		size_t beginColon = prettyFunction.substr(0, end).rfind("::") + 2;
		size_t beginSpace = prettyFunction.substr(0, end).rfind(" ") + 1;
		size_t begin = std::max(beginColon, beginSpace);
		std::string result = prettyFunction.substr(begin, end - begin);
		return (result + "()");
	}

	std::string className(const std::string& prettyFunction)
	{
		size_t methodEnd = prettyFunction.rfind("(");
		size_t beginColon = prettyFunction.substr(0, methodEnd).rfind("::") + 2;
		size_t beginSpace = prettyFunction.substr(0, methodEnd).rfind(" ") + 1;
		size_t methodBegin = std::max(beginColon, beginSpace);
		int closingBracket = 0;
		size_t resultStart = 0;

		for (int i = static_cast<int>(methodBegin); i > 0; i--)
		{
			if (prettyFunction[i] == '>')
				closingBracket++;
			if (prettyFunction[i] == '<')
				closingBracket--;
			if ((prettyFunction[i] == ' ' || prettyFunction[i] == '\t') && closingBracket == 0)
				break;
			resultStart = i;
		}

		if (resultStart == methodBegin)
			return ("No class");

		return (prettyFunction.substr(resultStart, methodBegin - resultStart - 2));
	}
	float degreeToRadian(const float& angle)
	{
		return ((angle * static_cast<float>(M_PI)) / 180.0f);
	}

	float radianToDegree(const float& radian)
	{
		return ((radian * 180.0f) / static_cast<float>(M_PI));
	}

	long generateNumberFromSeed(const long seed, const long x, const long y, const long z)
	{
		long h = static_cast<long>(seed + static_cast<unsigned long>(x) * 374761393 + static_cast<unsigned long>(y) * 668265263); //all constants are prime
		h = (h ^ (h >> 13)) * 1274126177;
		return (h ^ (h >> 16));
	}

	long generateNumber(const long min, const long max)
	{
		if (min == max)
			return (min);

		long value = 0;

		for (size_t i = 0; i < 4; i++)
		{
			value = value << 8;
			unsigned int value_tmp = rand();
			value += value_tmp;
		}
		return((value % (max + 1 - min)) + min);
	}

	std::vector<std::string> stringSplit(const std::string& p_string, const std::string& p_delim)
	{
		std::vector<std::string> result;

		size_t start = 0;
		size_t end = p_string.find(p_delim);
		while (end != std::string::npos)
		{
			result.push_back(p_string.substr(start, end - start));
			start = end + static_cast<size_t>(p_delim.length());
			end = p_string.find(p_delim, start);
		}

		result.push_back(p_string.substr(start, end - start));

		return (result);
	}

	std::string getStr(std::fstream& p_file)
	{
		std::string line;

		if (p_file.eof())
			return ("");
		std::getline(p_file, line);
		if (line.size() == 0)
			return ("");
		if (line[line.size() - 1] == L'\n')
			line[line.size() - 1] = L'\0';
		return (line);
	}

	std::vector<std::string> getStringSplit(std::fstream& p_file, const std::string& p_delim, const int& p_expectedSize)
	{
		std::string line = getStr(p_file);
		std::vector<std::string> result = stringSplit(line, p_delim);
		if (p_expectedSize != -1 && result.size() != p_expectedSize)
		{
			throw std::runtime_error("Unexpected number of string after parsing");
		}

		return (result);
	}

	int positive_modulo(int i, int n)
	{
		return (i % n + n) % n;
	}

	std::string normalizeString(std::string p_str, size_t p_finalLength, char p_char)
	{
		if (p_str.size() >= p_finalLength)
			return (p_str);
		return (std::string(p_finalLength - p_str.size(), p_char) + p_str);
	}

	std::vector<std::string> decomposeIntoParagraph(std::string p_str, size_t p_lineLength, std::string p_delim)
	{
		std::vector<std::string> tab = stringSplit(p_str, p_delim);
		std::vector<std::string> result;

		std::string line = "";
		for (size_t i = 0; i < tab.size(); i++)
		{
			if (line.size() + tab[i].size() >= p_lineLength)
			{
				result.push_back(line);
				line = "";
			}
			if (line.size() != 0)
				line += p_delim;
			line += tab[i];
		}
		result.push_back(line);

		return (result);
	}

	void editStringContent(std::string& p_str, size_t p_position, size_t p_length, char p_c)
	{
		for (size_t i = 0; i < p_length; i++) {
			p_str[i + p_position] = p_c;
		}
	}

	size_t findCharFromEnd(std::string p_str, size_t p_base, char p_c)
	{
		for (size_t i = p_base; i > 0; i--) {
			if (p_str[i] == p_c) {
				return i;
			}
		}
		return 0;
	}

	std::vector<std::string> listFile(std::string p_path, std::string p_extension)
	{
		std::vector<std::string > result;
		for (const auto& entry : std::filesystem::directory_iterator(p_path))
		{
			if (std::filesystem::is_regular_file(entry) && (p_extension == "*" || entry.path().extension() == p_extension))
			{
				result.push_back(entry.path().string());
			}
		}
		return result;
	}

	void drawRectangleColor(Color p_color, Vector2Int p_pos, Vector2Int p_size, float p_depth)
	{
		static unsigned int elementIndex[6] = { 0, 3, 1, 2, 3, 0 };
		static Vector2Int deltaPos[4] = {
			Vector2Int(0, 0),
			Vector2Int(1, 0),
			Vector2Int(0, 1),
			Vector2Int(1, 1)
		};

		const std::string shaderName = "Color2D";
		static Shader* tmpShader = nullptr;
		static Buffer* modelBuffer = nullptr;
		static Buffer* depthBuffer = nullptr;
		static Buffer* colorBuffer = nullptr;

		if (tmpShader == nullptr)
			tmpShader = ShaderAtlas::instance()->get(shaderName);

		if (tmpShader == nullptr)
			throw std::runtime_error("Error : no shader Color2D in application");

		if (modelBuffer == nullptr)
			modelBuffer = tmpShader->buffer("model_space");
		if (depthBuffer == nullptr)
			depthBuffer = tmpShader->buffer("model_depth");
		if (colorBuffer == nullptr)
			colorBuffer = tmpShader->buffer("model_color");

		Vector2 vertex_content[4];
		float depth_content[4];
		Color color_content[4];

		for (size_t i = 0; i < 4; i++)
		{
			vertex_content[i] = Window::instance()->convertScreenToOpenGL(p_pos + p_size * deltaPos[i]);
			depth_content[i] = Window::instance()->convertDepthToOpenGL(p_depth);
			color_content[i] = p_color;
		}

		modelBuffer->send(vertex_content, 4);
		depthBuffer->send(depth_content, 4);
		colorBuffer->send(color_content, 4);
		tmpShader->elementBuffer()->send(elementIndex, 6);

		tmpShader->launch(Shader::Mode::Triangle);
	}
}