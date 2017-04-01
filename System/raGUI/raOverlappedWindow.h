#pragma once

namespace GUI
{
	class RGAPI raOverlappedWindow : public raWindow
	{
	public:
		raOverlappedWindow(raInstance* hInst);
		~raOverlappedWindow(void);

		virtual bool Create (DWORD dwExStyle,char *pTitle,
						 raWindowClass *WindowClass,
						 System::raVector2 Groeﬂe,
						 unsigned long ID);
	};
}