#include "raGuiMain.h"

namespace System
{
namespace GUI
{
	bool raList::Create (System::raVector2 Position,
								 System::raVector2 Groeﬂe,
								 raWND* ParentWindow,
								 unsigned long ID,
								 bool ThirdDimension,
								 bool Sort)
	{
		unsigned long ExStyle = NULL;
		unsigned long Style = WS_VISIBLE | WS_CHILD | LBS_NOTIFY | WS_VSCROLL | WS_BORDER;
					  Style |= LBS_WANTKEYBOARDINPUT;

		// Membervariablen initialisieren
		m_ID = ID;
		m_Parent = ParentWindow;
		m_Type = WIN_LIST;

		if (ThirdDimension)
		{
			ExStyle = WS_EX_CLIENTEDGE;
			Style ^=  WS_BORDER;
		}

		if (Sort)
			Style = Style | LBS_SORT;

		m_Element = new raWND();
		if(!m_Element->Create(ExStyle,"LISTBOX", "" ,Style, Position, Groeﬂe, *m_Parent, reinterpret_cast<HMENU>(m_ID)))
			return false;

		printf("raList { Position x: %d y: %d ;  Size: x: %d y: %d ; ID: %d } erstellt\n",
				(int)Position.x, (int)Position.y, (int)Groeﬂe.x, (int)Groeﬂe.y, ID);

		return true;
	}
	bool raList::DelEntry (unsigned int Item)
	{
		void *pData;

		pData = reinterpret_cast<void*>(SendMessage (m_Element->GetWnd(), LB_GETITEMDATA, Item, 0));

		if (pData != reinterpret_cast<void*>(LB_ERR))
		{
			SAFE_DELETE (pData);
		}

		return (SendMessageA(m_Element->GetWnd(), LB_DELETESTRING, Item, 0) == S_OK);
	}
	bool raList::DelSelectedEntry()
	{
		return DelEntry(GetSelectedIndex());
	}
	unsigned int raList::AddEntry (raString Text)
	{
		return static_cast<unsigned int>(SendMessage (m_Element->GetWnd(), LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(Text.c_str())));
	}
	unsigned int raList::GetSelectedIndex()
	{
		return static_cast<unsigned int>(SendMessage (m_Element->GetWnd(), LB_GETCURSEL, 0, 0));
	}
	bool raList::SetSelectedIndex (unsigned int index)
	{
		if (index > GetNumEntrys()-1)
			return false;

		SendMessage (m_Element->GetWnd(), LB_SETCURSEL, index, 0);
		return true;
	}
	unsigned int raList::GetNumEntrys()
	{
		return static_cast<unsigned int>(SendMessage (m_Element->GetWnd(), LB_GETCOUNT, 0, 0));
	}
	raString raList::GetEntry (unsigned int Item)
	{
		int count = 0;
		int length = 0;
		char * text;

		if(Item >= 0)
		{
			if(Item < GetNumEntrys())
			{
				length = SendMessage(m_Element->GetWnd(), LB_GETTEXTLEN, (WPARAM)Item, 0);
				text = new char[length + 1];

				SendMessage(m_Element->GetWnd(), LB_GETTEXT, (WPARAM)Item, (LPARAM)text);
			}
		}
		std::string s(text);
		delete[] text;

		return s;
	}
	void raList::Clear ()
	{
		void *pData;

		// Alle eintr‰ge durchgehen
		for (unsigned int i=0; i < GetNumEntrys(); ++i)
		{
			pData = reinterpret_cast<void*>(SendMessage (m_Element->GetWnd(), LB_GETITEMDATA, i, 0));

			if (pData != reinterpret_cast<void*>(LB_ERR))
			{
				SAFE_DELETE (pData);
			}
		}

		SendMessage (m_Element->GetWnd(), LB_RESETCONTENT, 0, 0);
	}

	void raList::Destroy()
	{
		Clear ();

		DestroyWindow (m_Element->GetWnd());
	}
}
}