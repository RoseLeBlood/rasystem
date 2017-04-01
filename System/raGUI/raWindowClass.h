#pragma once

namespace GUI
{
	class RGAPI Event
	{
	public:
		Event(HWND hWnd, int msg) : shwnd(hWnd), smsg(msg)		{ }

		HWND shwnd;
		int smsg;

		operator int ()		{ return smsg; }
		operator HWND ()	{ return shwnd; }
	};
	class RGAPI EventParam
	{
	public:
		EventParam(LPARAM lParam, WPARAM wParam) : slParam(lParam), swParam(wParam)	{ }

		LPARAM slParam;
		WPARAM swParam;

		operator LPARAM () { return slParam; }
		operator WPARAM () { return swParam; }
	};
	class RGAPI raWindowClass : public WNDCLASSEX
	{
	public:
		raWindowClass (LPCSTR className);
		void Register ()
		{
			if (RegisterClassEx (this) == 0)
				throw System::raException ("RegisterClassEx");
		}
		void Register(HINSTANCE hInst)
		{
			hInstance = hInst;
			Register ();
		}
		bool GetWndClassEx(LPWNDCLASSEX lpwcx)
		{
			if(GetClassInfoExA(hInstance, lpszClassName, lpwcx) == 0)
				return false;
			else
				return true;
		}
		void Unregister()
		{
			if(UnregisterClass(lpszClassName, hInstance) == 0)
				throw System::raException("UnregisterClass");
		}
	};
}