#include "raGuiMain.h"

namespace System
{
namespace GUI
{
	bool raElapsedSlider::Create (int TimePosition, int TimeLength,
					 System::raVector2 Position,
					 System::raVector2 Groeﬂe,
					 raWND* ParentWindow,
					 unsigned long ID)
	{
		m_ID = ID;
		HINSTANCE m_hInstance = (HINSTANCE)ParentWindow->GetWindowLong(-6);
		m_Parent = ParentWindow;
		m_Type = WIN_ESLIDER;

		const int staticWidth = 50;
		const int staticHeight = 20;

		System::raVector2 Sliderpos(Position);
		System::raVector2 Slidersize(Groeﬂe);

		m_slider.Create(Sliderpos, Slidersize, ParentWindow, ID, false);
		m_Element = m_slider.GetHandle();

		m_timePos = new raWND();
		m_timelength = new raWND();
		// Create buddies.
		m_timePos->Create(0, "STATIC", "00:00", SS_RIGHT | WS_CHILD | WS_VISIBLE,
			System::raVector2(0,0), System::raVector2(staticWidth, staticHeight),
			*m_Parent, NULL, m_hInstance);

		SendMessage(m_slider.GetHandle()->GetWnd(), TBM_SETBUDDY,
			(WPARAM)TRUE, (LPARAM)m_timePos->GetWnd());

		m_timelength->Create(0, "STATIC", "01:00", SS_LEFT | WS_CHILD | WS_VISIBLE,
			System::raVector2(0,0), System::raVector2(staticWidth, staticHeight),
			*m_Parent, NULL, m_hInstance);

		SendMessage(m_slider.GetHandle()->GetWnd(), TBM_SETBUDDY,
			(WPARAM)FALSE, (LPARAM)m_timelength->GetWnd());

		SendMessage(m_Element->GetWnd(), TBM_SETRANGE,  (WPARAM) TRUE, (LPARAM) MAKELONG(0, 1001));

		printf("raElapsedSlider { Position x: %02d y: %02d ;  Size: x: %d y: %d ; ID: %d } erstellt\n",
				(int)Position.x, (int)Position.y, (int)Groeﬂe.x, (int)Groeﬂe.y, ID);

		SetClassLongPtr((HWND)m_Element,  -10, (LONG)CreateSolidBrush(RGB(rand()%255,rand()%255,rand()%255)));
		InvalidateRect((HWND)ParentWindow,NULL,TRUE);
		return true;
	}
	raString  raElapsedSlider::MicroSecToChar(int ms)
	{
		raString buffer = ( ms / 1000 / 60) + ":" + (ms / 1000 % 60);
		char buf[255];
		sprintf(buf, "%02d:%02d", ( ms / 1000 / 60), (ms / 1000 % 60));
		return raString(buf);
	}
	void raElapsedSlider::SetTimePosition (int posms)
	{
		if(posms <= 0)
			return;

		m_timespos = posms;
		// 100% = TimeLength
		//  x % = posms
		// x = (posms * 100) / TimeLength

		int timelen = GetTimeLength();
		int posx = (posms * 1001) / timelen;

		if(posx <= -1)
			posx = 0;

		SendMessage(m_Element->GetWnd(), TBM_SETPOS,  (WPARAM) TRUE, (LPARAM)posx);
		SetWindowText(m_timePos->GetWnd(), MicroSecToChar(posms).c_str());
	}

	void raElapsedSlider::SetTimeLength (int ms)
	{
		m_timelengt = ms;
		SetWindowText(m_timelength->GetWnd(), MicroSecToChar(ms).c_str());
	}

	void raElapsedSlider::SetPageSize(int pagesize)
	{
		SendMessage(m_Element->GetWnd(), TBM_SETPAGESIZE, 0, (LPARAM) pagesize);
	}
	void raElapsedSlider::SetPosition(int pos)
	{
		SendMessage(m_Element->GetWnd(), TBM_SETPOS, (WPARAM) TRUE, (LPARAM) pos);
	}
	int raElapsedSlider::GetPosition(void)
	{
		return SendMessage(m_Element->GetWnd(), TBM_GETPOS, 0, 0);
	}
}
}