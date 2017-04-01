#pragma once

	template <typename T> struct raListEntry
	{
		raListEntry<T>*	pPrevEntry;
		raListEntry<T>*	pNextEntry;
		T				Data;
	};

	template <typename T> class raList
	{
	private:
		raListEntry<T>*	m_pFirstEntry;
		raListEntry<T>*	m_pLastEntry;
		int				m_iNumEntries;

	public:
		raList();
		~raList();

		raListEntry<T>*		AddEntry(T* pData);
		bool				FindEntry(T* pData, raListEntry<T>** ppOut);
		bool				DeleteEntry(raListEntry<T>* pEntry);
		bool				Clear();
		bool				Traverse(bool (* pCallback)(raListEntry<T>*));

		T*					GetEntryData(raListEntry<T>* pEntry)	{return &pEntry->Data;}

		READONLY_PROPERTY(raListEntry<T>*, FirstEntry);
		GET(FirstEntry) { return m_pFirstEntry; }
		READONLY_PROPERTY(raListEntry<T>*, LastEntry);
		GET(LastEntry) { return m_pLastEntry; }
		READONLY_PROPERTY(int, Count);
		GET(Count) { return m_iNumEntries; }

	protected:
		virtual void __vsforeach( void (* func)(T&))
		{
			raListEntry<T>* pCurrentEntry = m_pFirstEntry;

			while(pCurrentEntry)
			{
				if(func) func(pCurrentEntry->Data);

				// Fortschreiten
				pCurrentEntry = pCurrentEntry->pNextEntry;
			}
		}
	};

	// ******************************************************************
	// Konstruktor der raList-Klasse
	template <typename T> raList<T>::raList()
	{
		// Alles zurücksetzen
		memset(this, 0,sizeof(raList<T>));
	}

	// ******************************************************************
	// Destruktor der raList-Klasse
	template <typename T> raList<T>::~raList()
	{
	}

	// ******************************************************************
	// Diese Methode fügt einen neuen Eintrag zur Liste hinzu.
	template <typename T> raListEntry<T>* raList<T>::AddEntry(T* pData)
	{
		raListEntry<T>* pNewEntry;

		// Parameter prüfen
		if(!pData) { RERROR_NULLPOINTER("pData"); return NULL; }

		// Speicher für die Listeneintragsstruktur reservieren
		pNewEntry = new raListEntry<T>;
		if(!pNewEntry) {  RERROR_OUTOFMEM(); return NULL; }

		// Der neue Eintrag steht an letzter Stelle der Liste.
		// Daher gibt es keinen nächsten Eintrag und der vorherige Eintrag
		// ist der ursprüngliche letzte Listeneintrag.
		pNewEntry->pPrevEntry = m_pLastEntry;
		if(m_pLastEntry) m_pLastEntry->pNextEntry = pNewEntry;
		pNewEntry->pNextEntry = NULL;
		m_pLastEntry = pNewEntry;

		// Wenn die Liste noch ganz leer ist, dann ist dieser Eintrag auch
		// gleichzeitig der erste Eintrag.
		if(!m_pFirstEntry) m_pFirstEntry = pNewEntry;

		// Daten kopieren
		memcpy(&pNewEntry->Data, pData, sizeof(T));

		// Eintragszähler erhöhen
		m_iNumEntries++;

		// Zeiger auf die Listeneintragsstruktur zurückliefern
		return pNewEntry;
	}

	// ******************************************************************
	// Diese Methode sucht einen Eintrag in der Liste mit den angegebenen Daten.
	template <typename T> bool raList<T>::FindEntry(T* pData, raListEntry<T>** ppOut)
	{
		raListEntry<T>* pCurrentEntry;

		// Parameter prüfen
		if(!pData) RERROR_NULLPOINTER("pData", false);

		// Zuerst den allerersten Listeneintrag testen
		pCurrentEntry = m_pFirstEntry;

		while(pCurrentEntry)
		{
			// Die Daten des aktuellen Eintrags mit den angegebenen Daten
			// vergleichen. Falls sie übereinstimmen, ist die Suche beendet.
			if(!memcmp(&pCurrentEntry->Data, pData, sizeof(T)))
			{
				// Falls gewünscht, wird der als Parameter angegebene Doppelzeiger
				// nun auf die Adresse der Listeneintragsstruktur gesetzt.
				if(ppOut) *ppOut = pCurrentEntry;
				return true;
			}

			// Fortschreiten
			pCurrentEntry = pCurrentEntry->pNextEntry;
		}

		// Es wurde nichts gefunden!
		return false;
	}

	// ******************************************************************
	// Diese Methode löscht einen Eintrag in der Liste.
	template <typename T> bool raList<T>::DeleteEntry(raListEntry<T>* pEntry)
	{
		// Parameter prüfen
		if(!pEntry) {  RERROR_NULLPOINTER("pEntry"); return false; }

		// Beim Löschen entsteht ein Loch in der Liste, welches nun "gestopft"
		// werden muss. Dabei spielt es eine Rolle, ob der Eintrag an erster
		// oder letzter Stelle oder irgendwo in der Mitte der Liste steht.
		if(pEntry == m_pFirstEntry &&
		   pEntry == m_pLastEntry)
		{
			// Der Eintrag ist der erste und einzige.
			m_pFirstEntry = NULL;
			m_pLastEntry = NULL;
		}
		else if(pEntry == m_pFirstEntry)
		{
			// Der Eintrag steht an erster Stelle.
			// Der neue erste Eintrag ist nun der Folgende des zu löschenden Eintrags.
			m_pFirstEntry = pEntry->pNextEntry;
			m_pFirstEntry->pPrevEntry = NULL;
		}
		else if(pEntry == m_pLastEntry)
		{
			// Der Eintrag steht an letzter Stelle.
			// Der neue letzte Eintrag ist nun der Vorherige des zu löschenden Eintrags.
			m_pLastEntry = pEntry->pPrevEntry;
			m_pLastEntry->pNextEntry = NULL;
		}
		else
		{
			// Der Eintrag steht irgendwo in der Mitte.
			// Der vorherige und der folgende Eintrag werden nun verknüpft.
			pEntry->pPrevEntry->pNextEntry = pEntry->pNextEntry;
			pEntry->pNextEntry->pPrevEntry = pEntry->pPrevEntry;
		}

		// Der Speicher für sie wurde beim Erstellen eines neuen Eintrags
		// reserviert und kann nun wieder freigegeben werden.
		SAFE_DELETE(pEntry);

		// Eintragszähler verringern
		m_iNumEntries--;

		return true;
	}

	// ******************************************************************
	// Diese Methode löscht die gesamte Liste.
	template <typename T> bool raList<T>::Clear()
	{
		// Es wird so lange der erste Listeneintrag gelöscht, bis keiner mehr da ist.
		while(m_pFirstEntry) DeleteEntry(m_pFirstEntry);

		return true;
	}

	// ******************************************************************
	// Durchlaufen der Liste
	template <typename T> bool raList<T>::Traverse(bool (* pCallback)(raListEntry<T>*))
	{
		raListEntry<T>* pCurrentEntry;

		// Parameter prüfen
		if(!pCallback) RERROR_NULLPOINTER("pCallback", false);

		// Die ganze Liste durchlaufen
		pCurrentEntry = m_pFirstEntry;

		while(pCurrentEntry)
		{
			// Rückruffunktion aufrufen
			if(pCallback(pCurrentEntry) == false) break;

			// Fortfahren
			pCurrentEntry = pCurrentEntry->pNextEntry;
		}

		return true;
	}