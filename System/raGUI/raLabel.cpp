#include "raGuiMain.h"
//#include <Windows.h>

namespace System
{
namespace GUI
{
	bool raLabel::Create (char *pText,
					 System::raVector2 Position,
					 System::raVector2 Groeﬂe,
					 raWND* ParentWindow,
					 unsigned long ID)
	{
		// Membervariablen initialisieren
		m_ID = ID;
		m_Parent = ParentWindow;
		m_Type = WIN_LABEL;

		m_Element = new raWND();
		if(!m_Element->Create(0, "STATIC", pText, WS_CHILD | WS_VISIBLE, Position, Groeﬂe, *m_Parent, reinterpret_cast<HMENU>(m_ID)))
			return false;

		return true;
	}
	void raLabel::Destroy()
	{
		DestroyWindow(m_Element->GetWnd());
	}

	void raLabel::SetText (char *pText)
	{
		SetWindowText(m_Element->GetWnd(), pText);
	}
	void raLabel::GetText (char *pText)
	{
		GetWindowText(m_Element->GetWnd(), pText, static_cast<int>(strlen (pText)));
	}
}
}