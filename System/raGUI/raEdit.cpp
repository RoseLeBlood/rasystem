#include "raGuiMain.h"
#include <Windows.h>

namespace System
{
namespace GUI
{
	bool raEdit::Create(char *pText,
					 System::raVector2 Position,
					 System::raVector2 Groeﬂe,
					 raWND* ParentWindow,
					 unsigned long ID,
					 bool Password,
					 bool ThirdDimension)
	{
		// Membervariablen initialisieren
		m_ID = ID;
		m_Parent = ParentWindow;
		m_Type = WIN_EDIT;

		unsigned long Style = WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL;
		unsigned long ExStyle = NULL;

		if (Password)
			Style |= ES_PASSWORD;

		if (ThirdDimension)
			ExStyle = WS_EX_CLIENTEDGE;

		m_Element = new raWND();
		if(!m_Element->Create(ExStyle,"EDIT", pText,Style, Position, Groeﬂe, *m_Parent, reinterpret_cast<HMENU>(m_ID)))
			return false;

		return true;
	}

	void raEdit::SetText (char *pText)
	{
		SetWindowText (m_Element->GetWnd(), pText);
	}
	void raEdit::GetText (char *pText)
	{
		GetWindowText (m_Element->GetWnd(), pText, static_cast<int>(strlen (pText)));
	}
	void raEdit::Destroy ()
	{
		DestroyWindow (m_Element->GetWnd());
	}
}
}