#pragma once

namespace GUI
{
	// Ein nicht gr��en anderbares Fenster
	class RGAPI raFixedWindow : public raWindow
	{
	public:
		raFixedWindow(raInstance* hInst);
		~raFixedWindow(void);

		virtual bool Create (DWORD dwExStyle,char *pTitle,
						 raWindowClass *WindowClass,
						 System::raVector2 Groe�e,
						 unsigned long ID);
	};
}