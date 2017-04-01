#pragma once
namespace GUI
{
	class RGAPI raWND
	{
	public:
		raWND(void) : m_hwnd(NULL)			{}
		raWND(HWND hWnd) : m_hwnd(hWnd)			{}

		~raWND(void)							{ m_hwnd = NULL; }

		bool Create(LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, System::raVector2 Position, System::raVector2 Size,
			raWND Parent, HMENU hMenu, LPVOID lpParam=NULL);
		bool Create(DWORD dwExStyle,LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, System::raVector2 Position, System::raVector2 Size,
			raWND Parent, HMENU hMenu, LPVOID lpParam=NULL);

		HWND			GetWnd()			{ return m_hwnd; }
		void			SetWnd(HWND hWnd)	{ m_hwnd = hWnd; }

		bool		   MoveWindow(System::raVector2 Position, System::raVector2 Size, bool repaint = false);

		bool           Show(int nCmdShow = SW_SHOW);
		void           Destroy();
		bool           Refresh();
		bool           SetFocus();

		raString       GetText() const;
		LONG_PTR       GetWindowLong(int nIndex);

		bool           SetText(const raString Text);
		bool           SetParent(raWND* hWndParent);
		bool           SetTransparenz(BYTE bRate, COLORREF crColor = 0);
		bool           SetClientRect(int x, int y);
		LONG_PTR	   SetWindowLong(int nIndex, LONG_PTR dwNewLong);

	private:
		HWND			m_hwnd;
	};
}