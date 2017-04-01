#pragma once

namespace GUI
{
	// Ein nicht größen anderbares Fenster
	class RGAPI raFixedWindow : public raWindow
	{
	public:
		raFixedWindow(raInstance* hInst);
		~raFixedWindow(void);

		virtual bool Create (DWORD dwExStyle,char *pTitle,
						 raWindowClass *WindowClass,
						 System::raVector2 Groeße,
						 unsigned long ID);
	};
}