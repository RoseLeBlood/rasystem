#pragma once

namespace raSystemGui
{
	class RGAPI raCombo : public raElement<raCombo>
	{
	public:
		bool Create (raSystem::raVector2 Position, 
							  raSystem::raVector2 Groeße, 
							  raWND* ParentWindow, 
						      unsigned long ID);
		void Destroy();

		// fügt einen Listeneintrag hinzu
		unsigned int AddEntry (raString pText);
		unsigned int GetSelectedIndex ();

		unsigned int GetNumEntrys ();

		void SelectEntry (unsigned int Index);

		// Leert die Liste
		void Clear ();

		template <typename T>
		bool SetData (unsigned int Index, T Data);

		template <typename T>
		T* GetData(unsigned int Index);

	};
	template <typename T>
	bool raCombo::SetData (unsigned int Index, T Data)
	{
		T *pData = NULL;

		// parameter prüfen
		if (Index > GetNumEntrys ()-1)
			throw raException("Invalid Parameter");

		// Speicher reservieren
		pData = new T;
		*pData = Data;

		if (!pData)
			throw raException("NULL Pointer");

		// Daten mit listeneintrag verknüpfen
		if ((SendMessage (m_Element->m_hwnd, CB_SETITEMDATA, Index, reinterpret_cast<LPARAM>(pData))) == CB_ERR)
			throw raException("Invalid API Call");

		return true;
	}

	template <typename T>
	T* raCombo::GetData(unsigned int Index)
	{
		T *pTemp = NULL;

		// Parameter prüfen
		if (Index > GetNumEntrys ()-1)
			throw raException("Invalid Parameter");

		// Daten abfragen
		if(pTemp = reinterpret_cast<T*>(m_Element->SendMessage(CB_GETITEMDATA, EventParam(0, Index))) == reinterpret_cast<T*>(CB_ERR))
			throw raException("Invalid API Call");
		
		//if ((pTemp = reinterpret_cast<T*>(SendMessageA (m_Element->m_hwnd, CB_GETITEMDATA, Index, 0))) == reinterpret_cast<T*>(CB_ERR))
			

		return pData;
	}
}

