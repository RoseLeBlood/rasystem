#include "raGuiMain.h"

namespace System
{
namespace GUI
{
	raPopupWindow::raPopupWindow(raInstance* hInst) : raWindow(hInst)
	{
	}

	raPopupWindow::~raPopupWindow(void)
	{
	}
	bool raPopupWindow::Create (DWORD dwExStyle, char *pTitle,
					 raWindowClass *WindowClass,
					 System::raVector2 Groeﬂe,
					 unsigned long ID)
	{
		m_WindowClass = WindowClass;

		m_ID = ID;
		m_Window = new raWND();
		if(!m_Window->Create(dwExStyle, WindowClass->lpszClassName,
							   pTitle,
							   WS_POPUPWINDOW | WM_DROPFILES | WS_CLIPCHILDREN,
							   System::raVector2(CW_USEDEFAULT),
							   Groeﬂe,
							   HWND_DESKTOP,
							   NULL/*reinterpret_cast<HMENU>(ID)*/,
							   m_WindowClass->hInstance))
		{
			return false;
		}
		SetWindowSize(Groeﬂe);
		raWindow::Create(dwExStyle, pTitle, WindowClass, Groeﬂe, ID);
		return true;
	}
}
}