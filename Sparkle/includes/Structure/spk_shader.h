#pragma once

#include "spk_opengl.h"

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>

#include "spk_iostream.h"

#include "spk_vector2.h"
#include "spk_vector3.h"
#include "spk_vector4.h"
#include "spk_color.h"

#include "spk_basic_functions.h"

namespace spk
{
	class Buffer
	{
	public:
		enum class Mode
		{
			Error = -1,
			Indexes = GL_ELEMENT_ARRAY_BUFFER,
			Array = GL_ARRAY_BUFFER
		};

		enum class Type
		{
			Error = -1,
			Float = GL_FLOAT,
			Int = GL_INT,
			UInt = GL_UNSIGNED_INT,
			Bool = GL_BOOL,
			Double = GL_DOUBLE
		};

		static std::string to_string(Mode p_type) {
			switch (p_type)
			{
			case Mode::Error:
				return ("Error");
			case Mode::Indexes:
			{
				return ("Indexes");
			}
			case Mode::Array:
			{
				return ("Array");
			}
			default:
				return ("undefined");
			}
		}

		static std::string to_string(Type p_type) {
			switch (p_type)
			{
			case Type::Error:
				return ("Error");
			case Type::Float:
			{
				return ("Float");
			}
			case Type::Int:
			{
				return ("Int");
			}
			case Type::UInt:
			{
				return ("UInt");
			}
			case Type::Bool:
			{
				return ("Bool");
			}
			case Type::Double:
			{
				return ("Double");
			}
			default:
				return ("undefined");
			}
		}

	private:
		std::string _name;
		int _location;
		unsigned int _elements;
		GLuint _id;
		Type _dataType;
		Mode _mode;
		size_t _size;
		size_t _maxSize;

		~Buffer()
		{
			glDeleteBuffers(1, &_id);
		}

	public:
		Buffer(std::string p_name = "undefined", int p_location = -1, unsigned int p_elements = 3, Type p_dataType = Type::Float, Mode p_mode = Mode::Array) :
			_name(p_name),
			_location(p_location),
			_elements(p_elements),
			_dataType(p_dataType),
			_size(0),
			_maxSize(0),
			_mode(p_mode)
		{
			glGenBuffers(1, &_id);
		}
		Buffer* copy() const {
			return (new Buffer(_name, _location, _elements, _dataType, _mode));
		}

		const std::string name() const { return (_name); }
		const int location() const { return (_location); }
		const unsigned int indexes() const { return (_elements); }
		const Type dataType() const { return (_dataType); }
		const Mode mode() const { return (_mode); }

		const size_t size() const { return (_size); }
		const GLint id() const { return (_id); }

		void activate()
		{
			if (_location == -1)
				return;

			glBindBuffer(static_cast<GLenum>(_mode), _id);
			if (_mode == Mode::Array)
			{
				glEnableVertexAttribArray(_location);
				glVertexAttribPointer(_location, _elements, static_cast<GLenum>(_dataType), GL_FALSE, 0, NULL);
			}
		}

		template<typename T>
		void send(T* data_array, size_t nb)
		{
			if (_location == -1)
				return;

			_size = sizeof(T) * nb;
			glBindBuffer(static_cast<GLenum>(_mode), _id);

			if (_maxSize < _size)
			{
				glBufferData(static_cast<GLenum>(_mode), _size, data_array, GL_DYNAMIC_DRAW);
				_maxSize = _size;
			}
			else
			{
				glBufferSubData(static_cast<GLenum>(_mode), 0, _size, data_array);
			}
		}

		template<typename T>
		std::vector<T> get()
		{
			std::vector<T> result(_size / sizeof(T));

			glGetNamedBufferSubData(_id, 0, _size, &result[0]);

			return (result);
		}
	};

	class Uniform
	{
	public:
		enum class Mode
		{
			Error,
			Array,
			Element
		};
		enum class Type
		{
			Error,
			Float,
			Int,
			UInt,
			Matrix,
			Sampler
		};
		enum class Size
		{
			Error,
			One,
			Two,
			Three,
			Four
		};
		static std::string str(Mode value)
		{
			switch (value)
			{
			case Mode::Error:
				return ("Error");
			case Mode::Element:
				return ("Element");
			case Mode::Array:
				return ("Array");
			default:
				return ("undefined");
			}
		}

		static std::string str(Type p_type, Size p_size) {
			switch (p_type)
			{
			case Type::Error:
				return ("Error");
			case Type::Float:
			{
				switch (p_size)
				{
				case Size::Error:
					return ("Error");
				case Size::One:
					return ("Float");
				case Size::Two:
					return ("Vector2");
				case Size::Three:
					return ("Vector3");
				case Size::Four:
					return ("Vector4");
				default:
					return ("undefined");
				}
			}
			case Type::Int:
			{
				switch (p_size)
				{
				case Size::Error:
					return ("Error");
				case Size::One:
					return ("Int");
				case Size::Two:
					return ("Ivector2");
				case Size::Three:
					return ("Ivector3");
				case Size::Four:
					return ("Ivector4");
				default:
					return ("undefined");
				}
			}
			case Type::UInt:
			{
				switch (p_size)
				{
				case Size::Error:
					return ("Error");
				case Size::One:
					return ("Uint");
				case Size::Two:
					return ("Uivector2");
				case Size::Three:
					return ("Uivector3");
				case Size::Four:
					return ("Uivector4");
				default:
					return ("undefined");
				}
			}
			case Type::Matrix:
			{
				switch (p_size)
				{
				case Size::Error:
					return ("Error");
				case Size::Two:
					return ("Matrix2x2");
				case Size::Three:
					return ("Matrix3x3");
				case Size::Four:
					return ("Matrix4x4");
				default:
					return ("undefined");
				}
			}
			case Type::Sampler:
			{
				switch (p_size)
				{
				case Size::Error:
					return ("Error");
				case Size::One:
					return ("Sampler2D");
				default:
					return ("undefined");
				}
			}
			default:
				return ("undefined");
			}
		}

	private:
		std::string _name;
		int _location;
		Mode _mode;
		Type _type;
		Size _size;

		~Uniform()
		{

		}

		bool _uniformChecker(Mode expected_mode, Type expected_type, Size expected_size)
		{
			std::string errorMessage = "";

			if (_mode == Mode::Error || _type == Type::Error || _size == Size::Error)
				errorMessage = "Trying to use uniform [" + _name + "] who isn't parsed correctly";
			if (_mode != expected_mode)
				errorMessage = "Trying to send an " + str(expected_mode) + " into [" + _name + "] who is setup as an " + str(_mode) + " uniform";
			if (_type != expected_type || _size != expected_size)
				errorMessage = "Trying to send a " + str(expected_type, expected_size) + " to uniform [" + _name + "] who is setup as " + str(_type, _size) + " uniform";
			if (_location == -1)
				errorMessage = "Trying to use uniform [" + _name + "] who isn't parsed correctly (location = -1)";

			if (errorMessage != "")
			{
				spk::cout << errorMessage << std::endl;
				throw std::runtime_error(errorMessage.c_str());
			}

			return (true);
		}

	public:
		Uniform(std::string p_name = "undefined", int p_location = -1, Mode p_mode = Mode::Element, Type p_type = Type::Float, Size p_size = Size::One) :
			_name(p_name),
			_location(p_location),
			_mode(p_mode),
			_type(p_type),
			_size(p_size)
		{

		}

		Uniform* copy() const {
			return (new Uniform(_name, _location, _mode, _type, _size));
		}
		const std::string name() const { return (_name); }
		const int location() const { return (_location); }
		const Mode mode() const { return (_mode); }
		const Type type() const { return (_type); }
		const Size size() const { return (_size); }

		// Element part
		template <typename T, typename std::enable_if < std::is_same <float, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			if (_uniformChecker(Mode::Element, Type::Float, Size::One) == true)
				glUniform1f(_location, data);
		}
		template <typename T, typename std::enable_if < std::is_same <int, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			if (_uniformChecker(Mode::Element, Type::Int, Size::One) == true)
				glUniform1i(_location, data);
		}
		template <typename T, typename std::enable_if < std::is_same <unsigned int, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			if (_uniformChecker(Mode::Element, Type::UInt, Size::One) == true)
				glUniform1ui(_location, data);
		}
		template <typename T, typename std::enable_if < std::is_same <Vector2, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			if (_uniformChecker(Mode::Element, Type::Float, Size::Two) == true)
				glUniform2f(_location, data.x, data.y);
		}
		template <typename T, typename std::enable_if < std::is_same <Vector2Int, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			if (_uniformChecker(Mode::Element, Type::Int, Size::Two) == true)
				glUniform2i(_location, data.x, data.y);
		}
		template <typename T, typename std::enable_if < std::is_same <Vector2UInt, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			if (_uniformChecker(Mode::Element, Type::UInt, Size::Two) == true)
				glUniform2ui(_location, data.x, data.y);
		}
		template <typename T, typename std::enable_if < std::is_same <Vector3, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			if (_uniformChecker(Mode::Element, Type::Float, Size::Three) == true)
				glUniform3f(_location, data.x, data.y, data.z());
		}
		template <typename T, typename std::enable_if < std::is_same <Vector3Int, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			if (_uniformChecker(Mode::Element, Type::Int, Size::Three) == true)
				glUniform3i(_location, data.x, data.y, data.z());
		}
		template <typename T, typename std::enable_if < std::is_same <Vector3UInt, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			if (_uniformChecker(Mode::Element, Type::UInt, Size::Three) == true)
				glUniform3ui(_location, data.x, data.y, data.z());
		}
		template <typename T, typename std::enable_if < std::is_same <Vector4, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			if (_uniformChecker(Mode::Element, Type::Float, Size::Four) == true)
				glUniform4f(_location, data.x, data.y, data.z(), data.w());
		}
		template <typename T, typename std::enable_if < std::is_same <Color, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			if (_uniformChecker(Mode::Element, Type::Float, Size::Four) == true)
				glUniform4f(_location, data.r, data.g, data.b, data.a);
		}
		template <typename T, typename std::enable_if < std::is_same <Vector4Int, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			if (_uniformChecker(Mode::Element, Type::Int, Size::Four) == true)
				glUniform4i(_location, data.x, data.y, data.z(), data.w());
		}
		template <typename T, typename std::enable_if < std::is_same <Vector4UInt, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			if (_uniformChecker(Mode::Element, Type::UInt, Size::Four) == true)
				glUniform4ui(_location, data.x, data.y, data.z(), data.w());
		}
		template <typename T, typename std::enable_if < std::is_same <glm::mat2x2, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			if (_uniformChecker(Mode::Element, Type::Matrix, Size::Two) == true)
				glUniformMatrix2fv(_location, 1, GL_FALSE, reinterpret_cast<GLfloat*>(&(data)));
		}
		template <typename T, typename std::enable_if < std::is_same <glm::mat3x3, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			if (_uniformChecker(Mode::Element, Type::Matrix, Size::Three) == true)
				glUniformMatrix3fv(_location, 1, GL_FALSE, reinterpret_cast<GLfloat*>(&(data)));
		}
		template <typename T, typename std::enable_if < std::is_same <glm::mat4x4, T>::value == true > ::type* = nullptr >
		void send(T data)
		{
			if (_uniformChecker(Mode::Element, Type::Matrix, Size::Four) == true)
				glUniformMatrix4fv(_location, 1, GL_FALSE, reinterpret_cast<GLfloat*>(&(data)));
		}

		// Array part
		template <typename T, typename std::enable_if < std::is_same <float, T>::value == true > ::type* = nullptr >
		void send(T* data, size_t nb_element)
		{
			if (_uniformChecker(Mode::Array, Type::Float, Size::One) == true)
				glUniform1fv(_location, nb_element, data);
		}
		template <typename T, typename std::enable_if < std::is_same <int, T>::value == true > ::type* = nullptr >
		void send(T* data, size_t nb_element)
		{
			if (_uniformChecker(Mode::Array, Type::Int, Size::One) == true)
				glUniform1iv(_location, nb_element, data);
		}
		template <typename T, typename std::enable_if < std::is_same <unsigned int, T>::value == true > ::type* = nullptr >
		void send(T* data, size_t nb_element)
		{
			if (_uniformChecker(Mode::Array, Type::UInt, Size::One) == true)
				glUniform1uiv(_location, nb_element, data);
		}
		template <typename T, typename std::enable_if < std::is_same <Vector2, T>::value == true > ::type* = nullptr >
		void send(T* data, size_t nb_element)
		{
			if (_uniformChecker(Mode::Array, Type::Float, Size::Two) == true)
				glUniform2fv(_location, nb_element, data);
		}
		template <typename T, typename std::enable_if < std::is_same <Vector2Int, T>::value == true > ::type* = nullptr >
		void send(T* data, size_t nb_element)
		{
			if (_uniformChecker(Mode::Array, Type::Int, Size::Two) == true)
				glUniform2iv(_location, nb_element, data);
		}
		template <typename T, typename std::enable_if < std::is_same <Vector2UInt, T>::value == true > ::type* = nullptr >
		void send(T* data, size_t nb_element)
		{
			if (_uniformChecker(Mode::Array, Type::UInt, Size::Two) == true)
				glUniform2uiv(_location, nb_element, data);
		}
		template <typename T, typename std::enable_if < std::is_same <Vector3, T>::value == true > ::type* = nullptr >
		void send(T* data, size_t nb_element)
		{
			if (_uniformChecker(Mode::Array, Type::Float, Size::Three) == true)
				glUniform3fv(_location, nb_element, data);
		}
		template <typename T, typename std::enable_if < std::is_same <Vector3Int, T>::value == true > ::type* = nullptr >
		void send(T* data, size_t nb_element)
		{
			if (_uniformChecker(Mode::Array, Type::Int, Size::Three) == true)
				glUniform3iv(_location, nb_element, data);
		}
		template <typename T, typename std::enable_if < std::is_same <Vector3UInt, T>::value == true > ::type* = nullptr >
		void send(T* data, size_t nb_element)
		{
			if (_uniformChecker(Mode::Array, Type::UInt, Size::Three) == true)
				glUniform3uiv(_location, nb_element, data);
		}
		template <typename T, typename std::enable_if < std::is_same <Vector4, T>::value == true > ::type* = nullptr >
		void send(T* data, size_t nb_element)
		{
			if (_uniformChecker(Mode::Array, Type::Float, Size::Four) == true)
				glUniform4fv(_location, nb_element, data);
		}
		template <typename T, typename std::enable_if < std::is_same <Color, T>::value == true > ::type* = nullptr >
		void send(T* data, size_t nb_element)
		{
			if (_uniformChecker(Mode::Array, Type::Float, Size::Four) == true)
				glUniform4fv(_location, nb_element, data);
		}
		template <typename T, typename std::enable_if < std::is_same <Vector4Int, T>::value == true > ::type* = nullptr >
		void send(T* data, size_t nb_element)
		{
			if (_uniformChecker(Mode::Array, Type::Int, Size::Four) == true)
				glUniform4iv(_location, nb_element, data);
		}
		template <typename T, typename std::enable_if < std::is_same <Vector4UInt, T>::value == true > ::type* = nullptr >
		void send(T* data, size_t nb_element)
		{
			if (_uniformChecker(Mode::Array, Type::UInt, Size::Four) == true)
				glUniform4uiv(_location, nb_element, data);
		}
		template <typename T, typename std::enable_if < std::is_same <glm::mat2x2, T>::value == true > ::type* = nullptr >
		void send(T* data, size_t nb_element)
		{
			if (_uniformChecker(Mode::Array, Type::Matrix, Size::Two) == true)
				glUniformMatrix2fv(_location, nb_element, GL_FALSE, &(data));
		}
		template <typename T, typename std::enable_if < std::is_same <glm::mat3x3, T>::value == true > ::type* = nullptr >
		void send(T* data, size_t nb_element)
		{
			if (_uniformChecker(Mode::Array, Type::Matrix, Size::Three) == true)
				glUniformMatrix3fv(_location, nb_element, GL_FALSE, &(data));
		}
		template <typename T, typename std::enable_if < std::is_same <glm::mat4x4, T>::value == true > ::type* = nullptr >
		void send(T* data, size_t nb_element)
		{
			if (_uniformChecker(Mode::Array, Type::Matrix, Size::Four) == true)
				glUniformMatrix4fv(_location, nb_element, GL_FALSE, &(data));
		}
	};

	class Shader
	{
	public:
		enum class Mode
		{
			Point = GL_POINTS,
			Line = GL_LINES,
			Triangle = GL_TRIANGLES,
		};

	private:
		static const inline std::string elementType[4][5] = {
			{"float", "int", "uint", "bool", "double"},
			{"vec2", "ivec2", "uivec2", "bvec2", "dvec2"},
			{"vec3", "ivec3", "uivec3", "bvec3", "dvec3"},
			{"vec4", "ivec4", "uivec4", "bvec4", "dvec4"}
		};
		static const inline std::vector<spk::Buffer::Type> elementTypeResult = {
			spk::Buffer::Type::Float, spk::Buffer::Type::Int, spk::Buffer::Type::UInt, spk::Buffer::Type::Bool, spk::Buffer::Type::Double
		};

		unsigned int _program;
		unsigned int _bufferArray;
		Buffer* _elementBuffer;
		std::map<std::string, spk::Uniform*> _uniforms;
		std::map<std::string, spk::Buffer*> _buffers;

		void _initialize()
		{
			_program = glCreateProgram();
			glGenVertexArrays(1, &_bufferArray);
			glBindVertexArray(_bufferArray);
		}
		void _compileShader(unsigned int p_shaderIndex, std::string p_shaderCode)
		{
			int result;
			int len;
			const char* content = p_shaderCode.c_str();

			result = GL_FALSE;

			glShaderSource(p_shaderIndex, 1, &content, NULL);
			glCompileShader(p_shaderIndex);
			glGetShaderiv(p_shaderIndex, GL_COMPILE_STATUS, &result);

			if (result != GL_TRUE)
			{
				glGetShaderiv(p_shaderIndex, GL_INFO_LOG_LENGTH, &len);
				char* errorMsg = new char[len + 1];
				glGetShaderInfoLog(p_shaderIndex, len, NULL, errorMsg);
				spk::cout << errorMsg << std::endl;
				throw std::exception(errorMsg);
			}
		}
		void _compileProgram(unsigned int p_programID, unsigned int p_vertexID, unsigned int p_fragmentID)
		{
			int result = GL_FALSE;
			glAttachShader(p_programID, p_vertexID);
			glAttachShader(p_programID, p_fragmentID);
			glLinkProgram(p_programID);
			glGetProgramiv(p_programID, GL_LINK_STATUS, &result);
			if (result != GL_TRUE)
			{
				throw std::runtime_error("Error while linking a program");
			}
			glDetachShader(p_programID, p_vertexID);
			glDetachShader(p_programID, p_fragmentID);
			glDeleteShader(p_vertexID);
			glDeleteShader(p_fragmentID);
		}

		void _compile(std::string p_vertexShaderCode, std::string p_fragmentShaderCode)
		{
			unsigned int _vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
			unsigned int _fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

			_compileShader(_vertex_shader_id, p_vertexShaderCode);
			_compileShader(_fragment_shader_id, p_fragmentShaderCode);
			_compileProgram(_program, _vertex_shader_id, _fragment_shader_id);

			_parseUniform(p_vertexShaderCode);
			_parseUniform(p_fragmentShaderCode);

			_parseBuffer(p_vertexShaderCode);
		}

		void _parseBuffer(std::string p_base) {
			std::vector<std::string> tab = stringSplit(p_base, "\n");

			glBindVertexArray(_bufferArray);
			if (tab.size() >= 2)
			{
				for (size_t j = 1; j < tab.size(); j++)
				{
					std::vector<std::string> tab2 = stringSplit(tab[j], ";");

					for (size_t i = 0; i < tab2.size(); i++)
					{
						auto skipLenght = tab2[i].find_first_not_of(" \t");
						if (tab2[i].size() >= 6u && (tab2[i].substr(skipLenght, 3) == "in " || tab2[i].substr(skipLenght, 6) == "layout"))
						{
							std::vector<std::string> tab = stringSplit(tab2[i], " ");
							std::string name = tab.back();

							GLint location_id = glGetAttribLocation(_program, name.c_str());
							if (location_id == GL_INVALID_OPERATION)
								throw std::runtime_error("Error while getting a new attrib location");

							size_t nb_component = 0;
							size_t tmp_elementType = 0;

							for (size_t i = 0; i < 4 && nb_component == 0; i++)
							{
								for (size_t j = 0; j < 5 && nb_component == 0; j++)
								{
									if (tab[tab.size() - 2] == elementType[i][j])
									{
										nb_component = j;
										tmp_elementType = i + 1;
									}
								}
							}

							_buffers[name] = new Buffer(name, location_id,static_cast<unsigned int>(tmp_elementType), elementTypeResult[nb_component], spk::Buffer::Mode::Array);
						}
					}
				}
			}
			_elementBuffer = new Buffer("element_buffer", 0, 1, spk::Buffer::Type::UInt, spk::Buffer::Mode::Indexes);
		}
		void _parseUniform(std::string base)
		{
			std::vector<std::string> tab = stringSplit(base, "\n");

			glBindVertexArray(_bufferArray);
			if (tab.size() >= 2)
			{
				for (size_t j = 1; j < tab.size(); j++)
				{
					std::vector<std::string> tab2 = stringSplit(tab[j], ";");
					for (size_t i = 0; i < tab2.size(); i++)
					{
						if (tab2[i].size() >= 8)
						{
							auto skipLenght = tab2[i].find_first_not_of(" \t");
							if (tab2[i].substr(skipLenght, 7) == "uniform")
							{
								std::vector<std::string> tab = stringSplit(tab2[i], " ");
								GLint id = glGetUniformLocation(_program, tab[2].c_str());
								if (id == GL_INVALID_VALUE || id == GL_INVALID_OPERATION)
									throw std::runtime_error("Error while getting a new attrib location");

								_parseUniformInformation(id, tab);
							}
						}
					}
				}
			}
		}

		void _parseUniformInformation(GLint p_location, std::vector<std::string> p_tab)
		{
			std::string name = p_tab[2];
			spk::Uniform::Mode mode = spk::Uniform::Mode::Error;
			spk::Uniform::Type type = spk::Uniform::Type::Error;
			spk::Uniform::Size size = spk::Uniform::Size::Error;

			if (p_tab.back().back() == ']')
				mode = spk::Uniform::Mode::Array;
			else
				mode = spk::Uniform::Mode::Element;

			if (p_tab[1] == "int")
			{
				type = spk::Uniform::Type::Int;
				size = spk::Uniform::Size::One;
			}
			else if (p_tab[1] == "float")
			{
				type = spk::Uniform::Type::Float;
				size = spk::Uniform::Size::One;
			}
			else if (p_tab[1] == "uint")
			{
				type = spk::Uniform::Type::UInt;
				size = spk::Uniform::Size::One;
			}
			else if (p_tab[1] == "vec2")
			{
				type = spk::Uniform::Type::Float;
				size = spk::Uniform::Size::Two;
			}
			else if (p_tab[1] == "vec3")
			{
				type = spk::Uniform::Type::Float;
				size = spk::Uniform::Size::Three;
			}
			else if (p_tab[1] == "vec4")
			{
				type = spk::Uniform::Type::Float;
				size = spk::Uniform::Size::Four;
			}
			else if (p_tab[1] == "ivec2")
			{
				type = spk::Uniform::Type::Int;
				size = spk::Uniform::Size::Two;
			}
			else if (p_tab[1] == "ivec3")
			{
				type = spk::Uniform::Type::Int;
				size = spk::Uniform::Size::Three;
			}
			else if (p_tab[1] == "ivec4")
			{
				type = spk::Uniform::Type::Int;
				size = spk::Uniform::Size::Four;
			}
			else if (p_tab[1] == "uvec2")
			{
				type = spk::Uniform::Type::UInt;
				size = spk::Uniform::Size::Two;
			}
			else if (p_tab[1] == "uvec3")
			{
				type = spk::Uniform::Type::UInt;
				size = spk::Uniform::Size::Three;
			}
			else if (p_tab[1] == "uvec4")
			{
				type = spk::Uniform::Type::UInt;
				size = spk::Uniform::Size::Four;
			}
			else if (p_tab[1] == "mat2")
			{
				type = spk::Uniform::Type::Matrix;
				size = spk::Uniform::Size::Two;
			}
			else if (p_tab[1] == "mat3")
			{
				type = spk::Uniform::Type::Matrix;
				size = spk::Uniform::Size::Three;
			}
			else if (p_tab[1] == "mat4")
			{
				type = spk::Uniform::Type::Matrix;
				size = spk::Uniform::Size::Four;
			}
			else if (p_tab[1].find("sampler") != std::string::npos)
			{
				type = spk::Uniform::Type::Int;
				size = spk::Uniform::Size::One;
			}

			_uniforms[name] = new spk::Uniform(name, p_location, mode, type, size);
		}

	public:
		Shader(std::fstream& p_vertexShaderFile, std::fstream& p_fragmentShaderFile)
		{
			_initialize();

			std::stringstream vShaderStream, fShaderStream;

			try
			{
				vShaderStream << p_vertexShaderFile.rdbuf();
				fShaderStream << p_fragmentShaderFile.rdbuf();

				p_vertexShaderFile.close();
				p_fragmentShaderFile.close();
			}
			catch (...)
			{
				throw std::exception("Error while reading files content");
			}

			_compile(vShaderStream.str(), fShaderStream.str());
		}
		Shader(std::string p_vertexShaderCode, std::string p_fragmentShaderCode)
		{
			_initialize();

			_compile(p_vertexShaderCode, p_fragmentShaderCode);

		}

		unsigned int id() { return (program()); }
		unsigned int program() { return (_program); }

		void activate()
		{
			glUseProgram(_program);

			glBindVertexArray(_bufferArray);
		}
		spk::Uniform* uniform(std::string p_name)
		{
			if (_uniforms.count(p_name) == 0)
				throw std::exception(("No uniform named " + p_name + " in shader").c_str());
			return (_uniforms[p_name]);
		}
		spk::Buffer* buffer(std::string p_name)
		{
			if (_buffers.count(p_name) == 0)
				throw std::exception(("No data buffer named " + p_name + " in shader").c_str());
			return (_buffers[p_name]);
		}
		spk::Buffer* elementBuffer()
		{
			return (_elementBuffer);
		}

		void cast(spk::Shader::Mode p_type, size_t p_nb_elem)
		{
			if (p_nb_elem != 0)
			{
				glDrawElements(static_cast<GLenum>(p_type), static_cast<GLsizei>(p_nb_elem), GL_UNSIGNED_INT, nullptr);
			}
		}
		void castInstancied(spk::Shader::Mode p_type, size_t p_nbVertexModel, size_t p_nbElement)
		{
			if (p_nbVertexModel != 0 && p_nbElement != 0)
			{
				glDrawElementsInstanced(static_cast<GLenum>(p_type), static_cast<GLsizei>(p_nbVertexModel), GL_UNSIGNED_INT, nullptr, static_cast<GLsizei>(p_nbElement));
			}
		}

		void launch(spk::Shader::Mode p_type)
		{
			activate();

			for (auto it = _buffers.begin(); it != _buffers.end(); it++)
			{
				it->second->activate();
			}

			_elementBuffer->activate();

			cast(p_type, _elementBuffer->size() / sizeof(unsigned int));
		}
		void launchInstancied(spk::Shader::Mode p_type, size_t p_nbElement)
		{
			activate();

			for (auto it = _buffers.begin(); it != _buffers.end(); it++)
			{
				it->second->activate();
			}

			_elementBuffer->activate();

			castInstancied(p_type, _elementBuffer->size() / sizeof(unsigned int), p_nbElement);
		}
	};
}