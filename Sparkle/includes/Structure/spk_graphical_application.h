#pragma once

#include "Structure/spk_singleton.h"
#include "Structure/spk_application.h"
#include "spk_imessage_consumer.h"
#include "spk_keyboard_module.h"
#include "spk_mouse_module.h"
#include "spk_system_module.h"
#include "spk_window_module.h"

namespace spk
{
	class GraphicalApplication : public spk::Application, public spk::Singleton<GraphicalApplication>
	{
		friend class spk::Singleton<GraphicalApplication>;
	private:

		spk::WindowsAPI* _windowsAPI;
		spk::KeyboardModule* _keyboardModule;
		spk::MouseModule* _mouseModule;
		spk::SystemModule* _systemModule;
		spk::WindowModule* _windowModule;

		void _setupJobs()
		{
			_addJob([&]() { _windowModule->reset(); return (0); });
			_addJob([&]() { _windowsAPI->update(); return (0); });
			_addJob([&]() { _windowModule->render(); return (0); });

			_addJob("UpdateThread", [&]() { _windowModule->update(); return (0); });
			_addJob("UpdateThread", [&]() { _systemModule->update(); return (0); });
			_addJob("UpdateThread", [&]() { _mouseModule->update(); return (0); });
			_addJob("UpdateThread", [&]() { _keyboardModule->update(); return (0); });
		}

		GraphicalApplication(std::string p_title, spk::Vector2Int p_size, spk::Color p_backgroundColor)
		{
			_windowsAPI = new spk::WindowsAPI();

			_keyboardModule = new spk::KeyboardModule(_windowsAPI);
			_mouseModule = new spk::MouseModule(_windowsAPI);
			_systemModule = new spk::SystemModule(_windowsAPI, this);
			_windowModule = new spk::WindowModule(_windowsAPI, p_title, p_size, p_backgroundColor);

			_setupJobs();
		}

	public:

	};

}