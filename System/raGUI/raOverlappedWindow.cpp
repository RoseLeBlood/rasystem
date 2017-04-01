#include "raGuiMain.h"

namespace System
{
namespace GUI
{
	raOverlappedWindow::raOverlappedWindow(raInstance* hInst) : raWindow(hInst)
	{
	}

	raOverlappedWindow::~raOverlappedWindow(void)
	{
	}
	bool raOverlappedWindow::Create (DWORD dwExStyle, char *pTitle,
					 raWindowClass *WindowClass,
					 System::raVector2 Groe�e,
					 unsigned long ID)
	{
		m_WindowClass = WindowClass;

		m_ID = ID;
		m_Window = new raWND();
		if(!m_Window->Create(dwExStyle, WindowClass->lpszClassName,
							   pTitle,
							   WS_OVERLAPPEDWINDOW | WM_DROPFILES | WS_CLIPCHILDREN,
							   System::raVector2(CW_USEDEFAULT),
							   Groe�e,
							   HWND_DESKTOP,
							   NULL/*reinterpret_cast<HMENU>(ID)*/,
							   m_WindowClass->hInstance))
		{
			return false;
		}
		SetWindowSize(Groe�e);
		raWindow::Create(dwExStyle, pTitle, WindowClass, Groe�e, ID);
		return true;
	}
}
}