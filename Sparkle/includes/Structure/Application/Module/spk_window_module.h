#pragma once

#include <Windows.h>
#include "spk_imessage_consumer.h"

#include "Structure/Graphics/spk_color.h"
#include "Structure/Mathematic/spk_vector2.h"

#include "spk_windows_api_module.h"

#define GLEW_STATIC
#define WGL_WGLEXT_PROTOTYPES
#include "GL/glew.h"
#include "GL/glut.h"
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/wglew.h>
#pragma comment(lib, "opengl32.lib")

namespace spk
{
	class WindowModule : public IMessageConsumerModule
	{
	public:
		static inline float C_MAX_DEPTH = 10000;
	private:
		static inline int C_MAJOR_VERSION = 4;
		static inline int C_MINOR_VERSION = 1;

		void* _api_module;

		wchar_t* _convertedTitle;
		HINSTANCE _hInstance;
		HWND _windowFrame;
		RECT _windowSize;
		WNDCLASS _windowClass;
		DWORD _windowStyle;
		DWORD _windowExStyle;

		HGLRC _hrc;
		HDC _hdc;
		PIXELFORMATDESCRIPTOR _pfd;

		spk::Color _backgroundColor;
		spk::Vector2Int _size;
		spk::Vector2Int _viewportSize;
		spk::Vector2Int _origin = 0;

		void _convertTitle(std::string p_title)
		{
			_convertedTitle = new wchar_t[p_title.size() + 1];
			size_t convertedChars = 0;
			mbstowcs_s(&convertedChars, _convertedTitle, p_title.size() + 1, p_title.c_str(), _TRUNCATE);
		}
		void _createGhostInstance()
		{
			_hInstance = GetModuleHandle(NULL);
		}
		void _registerWindowClass()
		{
			_windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
			_windowClass.lpfnWndProc = (WNDPROC)spk::WindowsAPI::WindowProc;
			_windowClass.cbClsExtra = 0;
			_windowClass.cbWndExtra = 0;
			_windowClass.hInstance = _hInstance;
			_windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
			_windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
			_windowClass.hbrBackground = NULL;
			_windowClass.lpszMenuName = NULL;
			_windowClass.lpszClassName = _convertedTitle;

			RegisterClass(&_windowClass);
		}
		void _createWindowFrame(spk::Vector2Int p_size)
		{
			_windowSize.left = (long)0;
			_windowSize.right = (long)p_size.x;
			_windowSize.top = (long)0;
			_windowSize.bottom = (long)p_size.y;

			_windowExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
			_windowStyle = WS_OVERLAPPEDWINDOW;

			AdjustWindowRectEx(&_windowSize, _windowStyle, FALSE, _windowExStyle);

			_windowFrame = CreateWindowEx(_windowExStyle, (LPCTSTR)(_convertedTitle), (LPCTSTR)(_convertedTitle), WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT, 0, _windowSize.right - _windowSize.left, _windowSize.bottom - _windowSize.top, NULL, NULL, _hInstance, _api_module);
		}
		void _composeOpenGLContext()
		{
			_hdc = GetDC(_windowFrame);

			memset(&_pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

			_pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
			_pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
			_pfd.iPixelType = PFD_TYPE_RGBA;
			_pfd.cColorBits = 24;
			_pfd.cDepthBits = 32;
			_pfd.cStencilBits = 32;
			_pfd.iLayerType = PFD_MAIN_PLANE;

			int nPixelFormat = ChoosePixelFormat(_hdc, &_pfd);
			SetPixelFormat(_hdc, nPixelFormat, &_pfd);

			HGLRC tempOpenGLContext = wglCreateContext(_hdc);
			wglMakeCurrent(_hdc, tempOpenGLContext);

			glewInit();

			int attributes[] = {
				WGL_CONTEXT_MAJOR_VERSION_ARB, C_MAJOR_VERSION,
				WGL_CONTEXT_MINOR_VERSION_ARB, C_MINOR_VERSION,
				WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
				0
			};

			if (wglewIsSupported("WGL_ARB_create_context") == 1)
			{
				_hrc = wglCreateContextAttribsARB(_hdc, NULL, attributes);
				wglMakeCurrent(NULL, NULL);
				wglDeleteContext(tempOpenGLContext);
				wglMakeCurrent(_hdc, _hrc);
			}

			int glVersion[2] = { -1, -1 };
			glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]);
			glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glFrontFace(GL_CCW);

			glEnable(GL_DEPTH_TEST);
			glClearDepth(1.0f);
			glDepthFunc(GL_LESS);

			glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_ALWAYS, 0, 0xFF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glStencilMask(0xFF);

			glEnable(GL_SCISSOR_TEST);
		}
		void _activateWindow()
		{
			ShowWindow(_windowFrame, SW_SHOW);
			UpdateWindow(_windowFrame);
			wglSwapIntervalEXT(0);
		}

		void _treatMessage(spk::DataBuffer* p_message)
		{
			unsigned int messageId;
			*p_message >> messageId;

			switch (messageId)
			{
			case WM_SIZE:
				unsigned int width;
				unsigned int height;

				*p_message >> width;
				*p_message >> height;

				resize(width, height);
				break;
			}
		}

		void _onUpdate()
		{
			render();
			clear();
		}

	public:
		WindowModule(WindowsAPI* p_windowsAPI, std::string p_title, spk::Vector2Int p_size, spk::Color p_backgroundColor) :
			IMessageConsumerModule(p_windowsAPI->windowMessages()),
			_api_module(p_windowsAPI)
		{
			createWindow(p_title, p_size, p_backgroundColor);
		}

		void createWindow(std::string p_title, spk::Vector2Int p_size, spk::Color p_backgroundColor)
		{
			_backgroundColor = p_backgroundColor;

			_convertTitle(p_title);

			_createGhostInstance();

			_registerWindowClass();

			_createWindowFrame(p_size);

			_composeOpenGLContext();

			resize(p_size.x, p_size.y);
			reset();

			_activateWindow();
		}

		const spk::Vector2Int& origin() const { return (_origin); }
		const spk::Vector2Int& size() const { return (_viewportSize); }

		void reset()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glClearColor(_backgroundColor.r, _backgroundColor.g, _backgroundColor.b, _backgroundColor.a);
			clear();
		}
		void resize(int w, int h)
		{
			_size.x = w;
			_size.y = h;
			reset();
		}
		void clear()
		{
			_origin = 0;
			glViewport(0, 0, _size.x, _size.y);
			glScissor(0, 0, _size.x, _size.y);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		}
		void render()
		{
			SwapBuffers(_hdc);
		}
	};
}