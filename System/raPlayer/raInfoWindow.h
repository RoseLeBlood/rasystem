#pragma once

#define PLAYER_INFO_WINDOW		9000
#define PLAYER_INFO_SIZE		System::raVector2(500, 300)

#define PLAYER_INFO_OK			9001
#define PLAYER_INFO_FMODLABEL	9002

class raInfoWindow : public raFixedWindow
{
public:
	raInfoWindow(raInstance* Inst);
	virtual ~raInfoWindow(void);

	virtual bool Create (DWORD dwSytle, char *pTitle,
						 raWindowClass *WindowClass,
						 raVector2 Groeße,
						 unsigned long ID);

	virtual long OnMessage(Event e, EventParam ep)
	{
		if(this->Wnd != e.shwnd)
			return 0;

		switch(e.smsg)
		{
			// So kann das Fenster nicht mehr geschlossen werden.
		case WM_DESTROY:
			break;
		case WM_QUIT:
			break;
		case WM_CLOSE:
			HideMe();
			break;
		case WM_SIZE:
			break;
		default:
			// jegliche andere Nachricht
			return raWindow::OnMessage(e, ep);
		};
		return 0;
	}
	void HideMe()
	{
		ShowWindow((!m_hide) ? SW_HIDE : SW_SHOW);
		m_hide = !m_hide;
	}
private:
	bool					m_hide;
	raRtfBox*					m_pTextBox;
	raLabel*					m_pLblCoppyright, m_pLabel2;
};