#include "raGuiMain.h"

namespace System
{
namespace GUI
{
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return ::DefWindowProcA(hwnd, msg,wParam,lParam);
	}
	raWindowClass::raWindowClass (LPCSTR className)
	{
		cbSize = sizeof (WNDCLASSEX);
		style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		cbClsExtra = 0;
		cbWndExtra = 0;
		lpfnWndProc = WindowProc;
		hInstance = raGuiManager::Instance().Handler;
		hIcon = LoadIcon (hInstance, className);
		hIconSm = LoadIcon (hInstance, "IDI_SMALL");
		hCursor = LoadCursor (0, IDC_ARROW);
		hbrBackground = (HBRUSH)(COLOR_WINDOW);
		lpszMenuName = NULL;
		lpszClassName = className;
	}
}
}