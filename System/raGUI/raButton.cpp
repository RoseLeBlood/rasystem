#include "raGuiMain.h"

namespace System
{
namespace GUI
{
	bool raButton::Create (char *pText,
					 System::raVector2 Position,
					 System::raVector2 Groeﬂe,
					 raWND* ParentWindow,
					 unsigned long ID)
	{
		m_ID = ID;
		m_Parent = ParentWindow;
		m_Type = WIN_BUTTON;

		// Button erstellen
		m_Element = new raWND();
		if(!m_Element->Create(0, "BUTTON", pText, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, Position, Groeﬂe, *m_Parent, reinterpret_cast<HMENU>(m_ID)))
			return false;

		printf("raButton { Position x: %d y: %d ; Size: x: %d y: %d ; ID: %d }",
				(int)Position.x, (int)Position.y, (int)Groeﬂe.x, (int)Groeﬂe.y, ID);

		printf("erstellt\n");

		return true;
	}

	void raButton::Destroy()
	{
		DestroyWindow(m_Element->GetWnd());
	}

	void raButton::SetText (char *pText)
	{
		SetWindowText(m_Element->GetWnd(), pText);
	}
	void raButton::GetText (char *pText)
	{
		GetWindowText(m_Element->GetWnd(), pText, static_cast<int>(strlen (pText)));
	}
}
}