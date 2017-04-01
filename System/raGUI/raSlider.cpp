#include "raGuiMain.h"

namespace System
{
namespace GUI
{
	raSlider::raSlider(void)
	{
	}

	raSlider::~raSlider(void)
	{
	}
	bool raSlider::Create (System::raVector2 Position,
					 System::raVector2 Groeﬂe,
					 raWND* ParentWindow,
					 unsigned long ID,
					 bool ThirdDimension)
	{
		// Membervariablen initialisieren
		m_ID = ID;
		m_Parent = ParentWindow;
		m_Type = WIN_SLIDER;

		unsigned long Style = WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_TRANSPARENTBKGND | TBS_NOTICKS | TBS_NOTIFYBEFOREMOVE;
		unsigned long ExStyle = NULL;

		if (ThirdDimension)
			ExStyle = WS_EX_CLIENTEDGE;

		m_Element = new raWND();
		if(!m_Element->Create(ExStyle,TRACKBAR_CLASS, "Trackbar Control",Style, Position, Groeﬂe, *m_Parent, reinterpret_cast<HMENU>(m_ID)))
			return false;

		SetRange(0,1000);
		SetPageSize(4);
		SetPosition(25);

		return true;
	}
	void raSlider::SetRange(int iMin, int iMax)
	{
		SendMessage(m_Element->GetWnd(), TBM_SETRANGE,  (WPARAM) TRUE, (LPARAM) MAKELONG(iMin, iMax));
	}
	void raSlider::SetPageSize(int pagesize)
	{
		SendMessage(m_Element->GetWnd(), TBM_SETPAGESIZE, 0, (LPARAM) pagesize);
	}
	void raSlider::SetPosition(int pos)
	{
		SendMessage(m_Element->GetWnd(), TBM_SETPOS, (WPARAM) TRUE, (LPARAM) pos);
	}
	int raSlider::GetPosition()
	{
		return (int)SendMessageA(m_Element->GetWnd(), TBM_GETPOS, 0,0);
	}
	void raSlider::Destroy()
	{
	}
}
}