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
					 System::raVector2 Groe�e,
					 unsigned long ID)
	{
		m_WindowClass = WindowClass;
		m_ID = ID;

		m_Window = new raWND();
		if(!m_Window->Create(dwExStyle, WindowClass->lpszClassName,
							   pTitle,
							   WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WM_DROPFILES | WS_CLIPCHILDREN,
							   System::raVector2(CW_USEDEFAULT),
							   Groe�e,
							   NULL,
							   NULL/*reinterpret_cast<HMENU>(ID)*/,
							   m_WindowClass->hInstance))
		{
			return false;
		}
		SetWindowSize(Groe�e);

		printf("raFixedWindow { Size: x: %d y: %d ; ID: %d } erstellt\n",
				(int)Groe�e.x,(int) Groe�e.y, ID);

		raWindow::Create(dwExStyle, pTitle, WindowClass, Groe�e, ID);
		return true;
	}
}
}