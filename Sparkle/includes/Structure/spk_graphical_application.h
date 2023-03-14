#pragma once

#include "Structure/spk_singleton.h"
#include "Structure/spk_application.h"
#include "spk_imessage_consumer.h"
#include "spk_keyboard_module.h"
#include "spk_mouse_module.h"
#include "spk_system_module.h"
#include "spk_window_module.h"
#include "spk_widget_module.h"

#include "spk_shader_atlas.h"

namespace spk
{
	class GraphicalApplication : public spk::Application, public spk::Singleton<GraphicalApplication>
	{
		friend class spk::Singleton<GraphicalApplication>;
		friend class Viewport;
	private:

		spk::WindowsAPI* _windowsAPI;
		spk::KeyboardModule* _keyboardModule;
		spk::MouseModule* _mouseModule;
		spk::SystemModule* _systemModule;
		spk::WindowModule* _windowModule;
		spk::WidgetModule* _widgetModule;
		spk::ShaderAtlas* _shaderAtlas;

		void _setupJobs()
		{
			_addJob([&]() { _windowModule->reset(); return (0); });
			_addJob([&]() { _windowsAPI->update(); return (0); });
			_addJob([&]() { _widgetModule->render(); return (0); });
			_addJob([&]() { _windowModule->render(); return (0); });

			_addJob("UpdateThread", [&]() { _windowModule->update(); return (0); });
			_addJob("UpdateThread", [&]() { _systemModule->update(); return (0); });
			_addJob("UpdateThread", [&]() { _mouseModule->update(); return (0); });
			_addJob("UpdateThread", [&]() { _keyboardModule->update(); return (0); });
			_addJob("UpdateThread", [&]() { _widgetModule->update(); return (0); });
		}

		GraphicalApplication(std::string p_title, spk::Vector2Int p_size, spk::Color p_backgroundColor)
		{
			_windowsAPI = new spk::WindowsAPI();

			_keyboardModule = new spk::KeyboardModule(_windowsAPI);
			_mouseModule = new spk::MouseModule(_windowsAPI);
			_systemModule = new spk::SystemModule(_windowsAPI, this);
			_windowModule = new spk::WindowModule(_windowsAPI, p_title, p_size, p_backgroundColor);
			_widgetModule = new spk::WidgetModule();

			_shaderAtlas = spk::ShaderAtlas::instanciate();
			
			_create2DColorShader();
			_create2DTextureShader();
			_create2DTextTextureShader();

			_setupJobs();
		}
		void _create2DColorShader()
		{
			std::string colorShaderVertex =
				R"( #version 330 core
				layout(location = 0) in vec2 model_space;
				layout(location = 1) in vec4 model_color;
				layout(location = 2) in float model_depth;

				out vec4 fragmentColor;

				void main()
				{
					gl_Position = vec4(model_space, model_depth, 1.0f);
					fragmentColor = model_color;
				})";

			std::string colorShaderFragment =
				R"( #version 330 core

				in vec4 fragmentColor;

				layout(location = 0) out vec4 color;

				void main()
				{
					color = fragmentColor;
					if (color.a == 0)
						discard;
				})";

			_shaderAtlas->add("Color2D", new spk::Shader(colorShaderVertex, colorShaderFragment));
		}
		void _create2DTextureShader()
		{
			std::string textureShaderVertex =
				R"(	#version 330 core
				layout(location = 0) in vec2 model_space;
				layout(location = 1) in vec2 model_uvs;
				layout(location = 2) in float model_depth;

				out vec2 UV;
				void main()
				{
					gl_Position = vec4(model_space, model_depth, 1.0f);
					UV = model_uvs;
				})";

			std::string textureShaderFragment =
				R"( #version 330 core
				in vec2 UV;

				layout(location = 0) out vec4 color;

				uniform sampler2D textureID;

				void main()
				{
					if (UV.x < 0 || UV.x > 1 || UV.y < 0 || UV.y > 1)
						discard;
					color = texture(textureID, UV).rgba;
					if (color.a == 0)
						discard;
				})";

			_shaderAtlas->add("Texture2D", new spk::Shader(textureShaderVertex, textureShaderFragment));
		}

		void _create2DTextTextureShader()
		{
			std::string textTextureShaderVertex =
				R"(	#version 330 core
				layout(location = 0) in vec2 model_space;
				layout(location = 1) in vec2 model_uvs;
				layout(location = 2) in float model_depth;

				uniform vec4 textColor;
				uniform vec4 outlineColor;

				out vec2 fragmentUV;
				out vec4 fragmentColor;
				out vec4 fragmentOutlineColor;

				void main()
				{
					gl_Position = vec4(model_space, model_depth, 1.0f);
					fragmentUV = model_uvs;
					fragmentColor = textColor;
					fragmentOutlineColor = outlineColor;
				})";

			std::string textTextureShaderFragment =
				R"( #version 330 core
				in vec2 fragmentUV;
				in vec4 fragmentColor;
				in vec4 fragmentOutlineColor;

				layout(location = 0) out vec4 color;

				uniform sampler2D textureID;
				uniform uint outlineSize;

				void main()
				{
					int outlineConvertedSize = int(outlineSize);
					if (fragmentUV.x < 0 || fragmentUV.x > 1 || fragmentUV.y < 0 || fragmentUV.y > 1)
						discard;
					vec4 tmp_color = texture(textureID, fragmentUV).rgba;
					if (tmp_color.r == 0.0f)
					{
						if (outlineSize == 0u)
							discard;
						else 
						{
							vec2 unit = 1.0f / textureSize(textureID, 0);
							bool isBorder = false;
							for (int i = -outlineConvertedSize; isBorder == false && i < outlineConvertedSize; i++)
							{
								for (int j = -outlineConvertedSize; isBorder == false && j < outlineConvertedSize; j++)
								{
									if (distance(vec2(i, j), vec2(0, 0)) < outlineConvertedSize)
									{
										vec2 newUv = fragmentUV + vec2(i, j) * unit;
										vec4 tmpColor = texture(textureID, newUv).rgba;
										if (tmpColor.r == 1)
											isBorder = true;
									}
								}
							}
							if (isBorder == true)
								color = fragmentOutlineColor;
						}
					}
					else
						color = fragmentColor;
					if (color.a == 0)
						discard;
				})";

			_shaderAtlas->add("TextTexture2D", new spk::Shader(textTextureShaderVertex, textTextureShaderFragment));
		}

	public:
	template <typename TChildrenType, typename ... Args>
		TChildrenType* addWidget(Args&& ...p_args)
		{
			return (_widgetModule->addWidget<TChildrenType>(std::forward<Args>(p_args)...));
		}
	};

}