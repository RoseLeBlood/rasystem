#include "raGuiMain.h"

namespace System
{
namespace GUI
{
	raFixedWindow::raFixedWindow(raInstance* hInst) : raWindow(hInst)
	{
	}

	raFixedWindow::~raFixedWindow(void)
	{
	}
	bool raFixedWindow::Create (DWORD dwExStyle,char *pTitle,
					 raWindowClass *WindowClass,
					 System::raVector2 Groeﬂe,
					 unsigned long ID)
	{
		m_WindowClass = WindowClass;
		m_ID = ID;

		m_Window = new raWND();
		if(!m_Window->Create(dwExStyle, WindowClass->lpszClassName,
							   pTitle,
							   WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WM_DROPFILES | WS_CLIPCHILDREN,
							   System::raVector2(CW_USEDEFAULT),
							   Groeﬂe,
							   NULL,
							   NULL/*reinterpret_cast<HMENU>(ID)*/,
							   m_WindowClass->hInstance))
		{
			return false;
		}
		SetWindowSize(Groeﬂe);

		printf("raFixedWindow { Size: x: %d y: %d ; ID: %d } erstellt\n",
				(int)Groeﬂe.x,(int) Groeﬂe.y, ID);

		raWindow::Create(dwExStyle, pTitle, WindowClass, Groeﬂe, ID);
		return true;
	}
}
}