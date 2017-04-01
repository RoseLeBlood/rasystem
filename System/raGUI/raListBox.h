#pragma once

namespace GUI
{
	class RGAPI  raList : public raElement<raList>
	{
	public:
		bool			Create (System::raVector2 Position,
								 System::raVector2 Groeße,
								 raWND* ParentWindow,
								 unsigned long ID,
								 bool ThirdDimension = true,
								 bool Sort = false);
		void			Destroy();

		unsigned int	AddEntry (raString Text);
		bool			DelEntry (unsigned int Item);
		raString		GetEntry (unsigned int Item);

		bool			DelSelectedEntry();
		unsigned int	GetSelectedIndex ();

		unsigned int	GetNumEntrys ();

		bool			SetSelectedIndex (unsigned int index);			// return false wenn index größer als GetNumEntrys ()-1 ist
		void			Clear ();

		void			DragAcceptFiles(bool fAccept = true)			{ ::DragAcceptFiles((HWND)this->m_Element,fAccept); }
		template <typename T>
		bool SetData (unsigned int Index, T Data);

		template <typename T>
		bool GetDate(unsigned int Index, T *pData);

		template <typename T>
		bool GetSelectedDate(T *pData);
	};

	template <typename T>
	bool raList::SetData(unsigned int Index, T Data)
	{
		T *pData = NULL;

		if (Index > GetNumEntrys()-1)
			return false;

		// Speicher reservieren
		pData = new T;
		*pData = Data;

		// Falls Nullpointer
		if (!pData)
			return false;

		// Daten mit listeneintrag verknüpfen
		if ((SendMessage (m_Element->GetWnd(), LB_SETITEMDATA, Index, reinterpret_cast<LPARAM>(pData))) == LB_ERR)
			return false;

		return true;
	}
	template <typename T>
	bool raList::GetSelectedDate(T *pData)
	{
			T *pData = NULL;

		if (GetSelectedIndex() > GetNumEntrys()-1)
			return false;

		// Speicher reservieren
		pData = new T;
		*pData = Data;

		// Falls Nullpointer
		if (!pData)
			return false;

		// Daten mit listeneintrag verknüpfen
		if ((SendMessage (m_Element->GetWnd(), LB_SETITEMDATA, Index, reinterpret_cast<LPARAM>(pData))) == LB_ERR)
			return false;

		return true;
	}
	template <typename T>
	bool raList::GetDate(unsigned int Index, T *pData)
	{
		T *pTemp = NULL;

		// Parameter prüfen
		if (Index > GetNumEntrys ()-1)
			return false;

		// Daten abfragen
		if ((pTemp = reinterpret_cast<T*>(SendMessage (m_Element->GetWnd(), LB_GETITEMDATA, Index, 0))) == reinterpret_cast<T*>(LB_ERR))
			return false;

		*pData = *pTemp;

		return true;
	}
}