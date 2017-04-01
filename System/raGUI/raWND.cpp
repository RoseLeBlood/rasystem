#include "raGuiMain.h"

namespace System
{
namespace GUI
{
		bool raWND::Create(LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, System::raVector2 Position, System::raVector2 Size,
			raWND Parent, HMENU hMenu,  LPVOID lpParam)
		{
			HINSTANCE hinst = (HINSTANCE)Parent.GetWindowLong(-6);

			m_hwnd = CreateWindowEx(0,lpClassName, lpWindowName, dwStyle, Position.x, Position.y, Size.x, Size.y, Parent.GetWnd(), hMenu,
				hinst, lpParam);

			return (m_hwnd == NULL) ?  false : true;
		}
		bool raWND::Create(DWORD dwExStyle,LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, System::raVector2 Position, System::raVector2 Size,
			raWND Parent, HMENU hMenu,  LPVOID lpParam)
		{
			HINSTANCE hinst = (HINSTANCE)Parent.GetWindowLong(-6);

			m_hwnd = CreateWindowEx(dwExStyle, lpClassName, lpWindowName, dwStyle, Position.x, Position.y, Size.x, Size.y, Parent.GetWnd(), hMenu,
				hinst, lpParam);

			return (m_hwnd == NULL) ?  false : true;
		}
		bool raWND::MoveWindow(System::raVector2 Position, System::raVector2 Size, bool repaint)
		{
			return (::MoveWindow(m_hwnd, Position.x, Position.y, Size.x, Size.y, repaint) != 0);
		}
		bool raWND::Show(int nCmdShow)
		{
			return (::ShowWindow(m_hwnd, nCmdShow) != 0);
		}
		void raWND::Destroy()
		{
			DestroyWindow(m_hwnd);
		}
		bool raWND::Refresh()
		{
			return (::UpdateWindow(m_hwnd) != 0);
		}
		bool raWND::SetFocus()
		{
			return (::SetForegroundWindow(m_hwnd) != 0);
		}

		raString raWND::GetText() const
		{
			LPTSTR  lpText;
			char* sText;
			int iTextLength = GetWindowTextLength(m_hwnd) + 1;

			lpText = new char[iTextLength];
			if (GetWindowText(m_hwnd, lpText, iTextLength) != 0) sText = lpText;

			delete [] lpText;
			return sText;
		}

		bool raWND::SetText(const raString Text)
		{
			return (::SetWindowText(m_hwnd, Text.c_str()) != 0);
		}
		bool raWND::SetParent(raWND* hWndParent)
		{
			return (::SetParent(m_hwnd, hWndParent->GetWnd()) != NULL);
		}
		bool raWND::SetTransparenz(BYTE bRate, COLORREF crColor)
		{
			DWORD dwFlags = LWA_ALPHA;
			if (crColor != 0) dwFlags |= LWA_COLORKEY;

			return (::SetLayeredWindowAttributes(m_hwnd, crColor, (255 * bRate) / 100, dwFlags) != 0);
		}
		bool raWND::SetClientRect(int x, int y)
		{
			RECT rcPos, rcNew = { 0, 0, x, y };
			AdjustWindowRectEx(&rcNew, GetWindowLong(GWL_STYLE), (GetMenu(m_hwnd) != NULL), GetWindowLong(GWL_EXSTYLE));
			GetWindowRect(m_hwnd, &rcPos);
			return MoveWindow(System::raVector2(rcPos.left, rcPos.top), System::raVector2(rcNew.right - rcNew.left, rcNew.bottom - rcNew.top));
		}
		LONG_PTR raWND::SetWindowLong(int nIndex, LONG_PTR dwNewLong)
		{
			return ::SetWindowLongPtr(m_hwnd, nIndex, dwNewLong);
		}
		LONG_PTR raWND::GetWindowLong(int nIndex)
		{
			return ::GetWindowLongPtr(m_hwnd, nIndex);
		}
	}
}