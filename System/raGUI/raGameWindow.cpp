#include "raGuiMain.h"

namespace System
{
namespace GUI
{
	raGameWindow::raGameWindow(raInstance* hInst) : raWindow(hInst)
	{
	}

	raGameWindow::~raGameWindow(void)
	{
	}
	bool raGameWindow::Create (DWORD dwExStyle,char *pTitle, raWindowClass *WindowClass,
						 System::raVector2 Groeﬂe, unsigned long ID)
	{
		m_WindowClass = WindowClass;
		m_ID = ID;

		m_Window = new raWND();

		System::raConfig conf = System::raConfigLoader::GetConfig();

		if(conf.FullScreen)
		{
			if(!m_Window->Create(dwExStyle, WindowClass->lpszClassName,
							   "raGame",
							   WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WM_DROPFILES | WS_CLIPCHILDREN,
							   System::raVector2(CW_USEDEFAULT),
							   Groeﬂe,
							   HWND_DESKTOP,
							   NULL/*reinterpret_cast<HMENU>(ID)*/,
							   m_WindowClass->hInstance))
			{
				return false;
			}
			SetWindowSize(System::raVector2(10, 10));
		}
		else
		{
			if(!m_Window->Create(dwExStyle, WindowClass->lpszClassName,
							   "raGame",
							   WS_OVERLAPPEDWINDOW | WM_DROPFILES | WS_CLIPCHILDREN,
							   System::raVector2(CW_USEDEFAULT),
							   Groeﬂe,
							   HWND_DESKTOP,
							   NULL/*reinterpret_cast<HMENU>(ID)*/,
							   m_WindowClass->hInstance))
			{
				return false;
			}
			SetWindowSize(System::raVector2(conf.weite, conf.Hoehe));
		}
		raWindow::Create(dwExStyle, "raGame", WindowClass, Groeﬂe, ID);

		return m_dx.Init(m_Window->GetWnd());
	}
}
}