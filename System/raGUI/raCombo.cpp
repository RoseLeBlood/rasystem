#include "raGuiMain.h"

using namespace raSystemGui;


bool raCombo::Create (raSystem::raVector2 Position, 
							  raSystem::raVector2 Groeße, 
							  raWND* ParentWindow, 
						      unsigned long ID)
{
	m_ID = ID;
	m_Parent = ParentWindow;
	m_Type = WIN_COMBO;

	//m_Element = new raWND();
	if(!raWND::Create("COMBOBOX", "", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL | WS_TABSTOP, 
		Position, Groeße, *m_Parent, reinterpret_cast<HMENU>(m_ID)))
		return false;


	return true;

}
unsigned int raCombo::AddEntry(raString pText)
{
	unsigned int Temp = 0;

	Temp = SendMessage(CB_ADDSTRING, EventParam(reinterpret_cast<LPARAM>(pText.c_str()), 0));

	//Temp = static_cast<unsigned int>(SendMessageW (m_Element, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(pText)));

	return Temp;
}
unsigned int raCombo::GetSelectedIndex()
{
	return static_cast<unsigned int>(SendMessage(CB_GETCURSEL, EventParam()));
	//return static_cast<unsigned int>(SendMessageW (m_Element, CB_GETCURSEL, 0, 0));
}
unsigned int raCombo::GetNumEntrys()
{
	return static_cast<unsigned int>(SendMessage(CB_GETCOUNT, EventParam()));
	//return static_cast<unsigned int>(SendMessageW (m_Element->m_hwnd, CB_GETCOUNT, 0, 0));
}
void raCombo::Clear()
{
	void *pData = NULL;

	for (unsigned int i=0; i < GetNumEntrys (); ++i)
	{
		pData = reinterpret_cast<void*>(SendMessage(CB_GETITEMDATA, EventParam(0, i)));

		//pData = reinterpret_cast<void*>(SendMessageW (m_Element, CB_GETITEMDATA, i, 0));

		if (pData != reinterpret_cast<void*>(CB_ERR))
		{
			SAFE_DELETE (pData);
		}
	}

	SendMessage(CB_RESETCONTENT, EventParam());
	//SendMessageW (m_Element, CB_RESETCONTENT, 0, 0);
}
void raCombo::SelectEntry (unsigned int Index)
{
	if (Index > GetNumEntrys () - 1)
		throw raException("Invalid Call");

	//SendMessageW (m_Element, CB_SETCURSEL, Index, 0);
	SendMessage(CB_SETCURSEL, EventParam(0, Index));
}
void raCombo::Destroy ()
{
	// Liste leeren
	Clear ();

	// das Element zerstören
	raWND::Destroy();
	//DestroyWindow (m_Element);
}
