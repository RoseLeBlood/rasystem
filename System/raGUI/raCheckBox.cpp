#include "raGuiMain.h"

namespace System
{
namespace GUI
{
	bool raCheckbox::Create (char *pText,
					 System::raVector2 Position,
					 System::raVector2 Groeﬂe,
					 raWND* ParentWindow,
					 unsigned long ID)
	{
		m_ID = ID;
		m_Parent = ParentWindow;
		m_Type = WIN_CHECK;

		m_Element = new raWND();

		if(!m_Element->Create(0, "BUTTON", pText, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, Position, Groeﬂe, *m_Parent, reinterpret_cast<HMENU>(m_ID)))
			return false;

		printf("raCheckbox { Position x: %d y: %d ;  Size: x: %d y: %d ; ID: %d } erstellt\n",
				(int)Position.x, (int)Position.y, (int)Groeﬂe.x, (int)Groeﬂe.y, ID);

		return true;
	}

	bool raCheckbox::IsChecked()
	{
		return (SendMessageA (m_Element->GetWnd(), BM_GETCHECK, 0, 0) != 0);
	}
	void raCheckbox::Check(bool check)
	{
		SendMessage (m_Element->GetWnd(), BM_SETCHECK, (WPARAM)(check), 0);
	}

	void raCheckbox::Destroy()
	{
		DestroyWindow (m_Element->GetWnd());
	}
}
}