#pragma once

namespace GUI
{
	#define WM_TRAYICON (WM_USER + 100) // lParam: WM_* and NIN_* messages, wParam: uID

	class RGAPI raTrayIcon
	{
	public:
		raTrayIcon(void);
		~raTrayIcon(void);

		bool Create(raWND* wnd, HICON hIcon, LPCTSTR lpTip);
		void Destroy();
		void ShowBalloonTip(LPCTSTR lpTitle, LPCTSTR lpMessage, DWORD dwIcon = NIIF_INFO, UINT uTimeout = 0);
		void ShowMenu(raMenu* hMenu);

		void SetCaption(LPCTSTR lpTip);
		void SetIcon(HICON hIcon);
		const UINT& GetID() { return m_uID; }
	protected:
        NOTIFYICONDATA m_nidTrayIcon;
		UINT           m_uID;
		static UINT    m_uPreviousID;
	};
}