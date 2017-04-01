#include "raGuiMain.h"

namespace System
{
namespace GUI
{
	raTrayIcon::raTrayIcon(void)
	{
		ZeroMemory(&m_nidTrayIcon, sizeof(m_nidTrayIcon));
		m_uPreviousID++;
		m_uID = m_uPreviousID;
	}

	raTrayIcon::~raTrayIcon(void)
	{
		Destroy();
	}
	UINT raTrayIcon::m_uPreviousID = 20;

	void raTrayIcon::Destroy()
	{
		Shell_NotifyIcon(NIM_DELETE, &m_nidTrayIcon);
	}

	bool raTrayIcon::Create(raWND* wnd, HICON hIcon, LPCTSTR lpTip)
	{
		m_nidTrayIcon.cbSize           = sizeof(m_nidTrayIcon);
		m_nidTrayIcon.hWnd             = (wnd) ? wnd->GetWnd() : NULL;
		m_nidTrayIcon.uID              = m_uID;
		m_nidTrayIcon.uFlags           = NIF_ICON | NIF_INFO | NIF_MESSAGE | NIF_TIP;
		m_nidTrayIcon.uCallbackMessage = WM_TRAYICON;
		m_nidTrayIcon.hIcon            = hIcon;

		strcpy(m_nidTrayIcon.szTip, lpTip);

		Shell_NotifyIconA(NIM_ADD, &m_nidTrayIcon);

		return true;
	}

	void raTrayIcon::SetIcon(HICON hIcon)
	{
		m_nidTrayIcon.hIcon = hIcon;
		Shell_NotifyIcon(NIM_MODIFY, &m_nidTrayIcon);
	}

	void raTrayIcon::SetCaption(LPCTSTR lpTip)
	{
		strcpy(m_nidTrayIcon.szTip, lpTip);
		Shell_NotifyIcon(NIM_MODIFY, &m_nidTrayIcon);
	}

	void raTrayIcon::ShowBalloonTip(LPCTSTR lpTitle, LPCTSTR lpMessage, DWORD dwIcon, UINT uTimeout)
	{
		m_nidTrayIcon.dwInfoFlags = dwIcon;
		m_nidTrayIcon.uTimeout    = uTimeout;

		strcpy(m_nidTrayIcon.szInfo, lpMessage);
		strcpy(m_nidTrayIcon.szInfoTitle, lpTitle);

		Shell_NotifyIcon(NIM_MODIFY, &m_nidTrayIcon);
	}

	void raTrayIcon::ShowMenu(raMenu* hMenu)
	{
		POINT pt;

		SetForegroundWindow(m_nidTrayIcon.hWnd);
		GetCursorPos(&pt);

		TrackPopupMenu(hMenu->GetMenu(), TPM_RIGHTBUTTON, pt.x, pt.y, 0, m_nidTrayIcon.hWnd, NULL);

		PostMessage(m_nidTrayIcon.hWnd, WM_NULL, 0, 0);
	}
}
}