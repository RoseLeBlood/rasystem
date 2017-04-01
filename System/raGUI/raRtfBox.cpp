#include "raGuiMain.h"

#define MSFTEDIT_CLASS  "RICHEDIT50W"

namespace System
{
namespace GUI
{
	bool raRtfBox::Create (char *pText,
						 System::raVector2 Position,
						 System::raVector2 Groeﬂe,
						 raWND* ParentWindow,
						 unsigned long ID,
						bool ReadOnly,
						bool MultiLine,
						bool ThirdDimension)
	{
		m_ID = ID;
		m_Parent = ParentWindow;
		m_Type = WIN_RTF;

		LoadLibrary("Msftedit.dll");

		DWORD Style =   WS_VISIBLE | WS_CHILD | WS_BORDER | WS_TABSTOP;
		DWORD ExStyle = 0;

		if(MultiLine)
			Style |= ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | WS_VSCROLL;

		if (ThirdDimension)
			ExStyle = WS_EX_CLIENTEDGE;

		if(ReadOnly)
			Style |= ES_READONLY;

		m_Element = new raWND();
		if(!m_Element->Create(ExStyle,MSFTEDIT_CLASS, pText,Style, Position, Groeﬂe, *m_Parent, reinterpret_cast<HMENU>(m_ID)))
			return false;

		//::UpdateWindow(m_Element->GetWnd());

		return true;
	}
	void raRtfBox::Destroy ()
	{
		DestroyWindow (m_Element->GetWnd());
	}
}
}