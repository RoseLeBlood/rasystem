#pragma once
namespace GUI
{
	class RGAPI raPopupWindow : public raWindow
	{
	public:
		raPopupWindow(raInstance* hInst);
		~raPopupWindow(void);

		bool Create (DWORD dwExStyle,char *pTitle,
					 raWindowClass *WindowClass,
					 System::raVector2 Groe�e,
					 unsigned long ID);
	};
}