#include "raGuiMain.h"

namespace System
{
namespace GUI
{
	bool raRadiobox::Create (char *pText,
					 System::raVector2 Position,
					 System::raVector2 Groeﬂe,
					 raWND* ParentWindow,
					 unsigned long ID)
	{
		m_ID = ID;
		m_Parent = ParentWindow;
		m_Type = WIN_RADIO;

		m_Element = new raWND();
		if(!m_Element->Create(0, "BUTTON", pText,  WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, Position, Groeﬂe, *m_Parent, reinterpret_cast<HMENU>(m_ID)))
			return false;
		return true;
	}

	bool raRadiobox::IsChecked()
	{
		return SendMessage (m_Element->GetWnd(), BM_GETCHECK, 0, 0) == S_OK ? true : false;
	}
	void raRadiobox::Check(bool check)
	{
		SendMessage (m_Element->GetWnd(), BM_SETCHECK, (WPARAM)(check), 0);
	}

	void raRadiobox::Destroy()
	{
		DestroyWindow (m_Element->GetWnd());
	}
}
}