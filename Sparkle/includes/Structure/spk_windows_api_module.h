#pragma once

#include <Windows.h>
#include "Structure/spk_locked_queue.h"
#include "Structure/spk_pool.h"
#include "Structure/spk_data_buffer.h"
#include "Structure/spk_singleton.h"

namespace spk
{
	class WindowsAPI
	{
	public:
		using MessagePool = spk::Singleton<spk::Pool<spk::DataBuffer>>;
		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			WindowsAPI* pThis = NULL;

			if (uMsg == WM_NCCREATE)
			{
				CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
				pThis = (WindowsAPI*)pCreate->lpCreateParams;
				SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

				pThis->_windowFrame = hwnd;
			}
			else
			{
				pThis = (WindowsAPI*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			}

			if (pThis)
			{
				return pThis->_handleMessage(uMsg, wParam, lParam);
			}
			else
			{
				return DefWindowProc(hwnd, uMsg, wParam, lParam);
			}
		}

	private:

		void _pullWinMessage()
		{
			MSG msg = {};
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				int translateResult = TranslateMessage(&msg);
				int dispatchResult = static_cast<int>(DispatchMessage(&msg));
			}
		}

		LRESULT _handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			spk::DataBuffer* newMessage = MessagePool::instance()->obtain();

			newMessage->clear();

			*newMessage << uMsg;
			switch (uMsg)
			{
			case WM_DESTROY:
			{
				_systemMessages.push_back(newMessage);
				break;
			}
			case WM_MOVE:
			case WM_MDIRESTORE:
			case WM_SETFOCUS:
			case WM_KILLFOCUS:
			{
				_windowMessages.push_back(newMessage);
				break;
			}
			case WM_SIZE:
			{
				unsigned int width = LOWORD(lParam);
				unsigned int height = HIWORD(lParam);

				*newMessage << width;
				*newMessage << height;

				_windowMessages.push_back(newMessage);
				break;
			}

			case WM_LBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_MBUTTONUP:
			case WM_RBUTTONUP:
			{
				_mouseMessages.push_back(newMessage);
				break;
			}
			case WM_MOUSEHWHEEL:
			case WM_MOUSEWHEEL:
			{
				short value = GET_WHEEL_DELTA_WPARAM(wParam);

				*newMessage << value;

				_mouseMessages.push_back(newMessage);
				break;
			}
			case WM_MOUSEMOVE:
			{
				int x = LOWORD(lParam);
				int y = HIWORD(lParam);

				*newMessage << x;
				*newMessage << y;

				_mouseMessages.push_back(newMessage);
				break;
			}

			// ----- Keyboard part
			case WM_CHAR:
			case WM_KEYDOWN:
			case WM_KEYUP:
			case WM_HOTKEY:
			case WM_SYSKEYDOWN:
			case WM_SYSKEYUP:
			{
				unsigned int value = static_cast<unsigned int>(wParam);

				*newMessage << value;

				_keyboardMessages.push_back(newMessage);
				break;
			}

			default:
			{
				return DefWindowProc(_windowFrame, uMsg, wParam, lParam);
			}
			}
			return TRUE;
		}

		HWND _windowFrame;

		spk::LockedQueue<spk::DataBuffer*> _mouseMessages;
		spk::LockedQueue<spk::DataBuffer*> _keyboardMessages;
		spk::LockedQueue<spk::DataBuffer*> _windowMessages;
		spk::LockedQueue<spk::DataBuffer*> _systemMessages;

	public:
		WindowsAPI()
		{
			spk::Singleton<spk::Pool<spk::DataBuffer>>::instanciate();

			_windowFrame = NULL;
		}

		void update()
		{
			_pullWinMessage();
		}

		spk::LockedQueue<spk::DataBuffer*>& mouseMessages() { return (_mouseMessages); }
		spk::LockedQueue<spk::DataBuffer*>& keyboardMessages() { return (_keyboardMessages); }
		spk::LockedQueue<spk::DataBuffer*>& windowMessages() { return (_windowMessages); }
		spk::LockedQueue<spk::DataBuffer*>& systemMessages() { return (_systemMessages); }
	};
}