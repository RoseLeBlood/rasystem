#pragma once

	template <typename T, typename TM> class raMap
	{
		public:
			raMap();
			~raMap();

			void AddItem(T what, TM white);
			bool GetItem(T what, TM& white);
			void DeleteItem(T what);
			void Clear();

		protected:
			struct raMapItem
			{
				T    m_what;
				TM	 m_white;
			};

			raMapItem*  pItems;
			size_t nItems;

			int FindItem(T known);
	};
	template <typename T, typename TM>
	raMap<T, TM>::raMap()
	{
		pItems = NULL;
		nItems = 0;
	}
	template <typename T, typename TM>
	raMap<T, TM>::~raMap()
	{
		Clear();
	}
	template <typename T, typename TM>
	void raMap<T, TM>::Clear()
	{
		if (pItems)
		{
			delete [] pItems;
			pItems = NULL;
		}
		nItems = 0;
	}

	template <typename T, typename TM>
	int raMap<T, TM>::FindItem(T what)
	{
		if (!pItems) return -1;
		for (size_t i = 0; i < nItems; i++)
		{
			if (pItems[i].m_what == what)
				return static_cast<int>(i);
		}
		return -1;
	}

	template <typename T, typename TM>
	void raMap<T, TM>::AddItem(T what, TM white)
	{
		raMapItem* pNewItems = new raMapItem[nItems + 1];

		if (pItems) memcpy(pNewItems, pItems, nItems * sizeof(raMapItem));
		pNewItems[nItems].m_what    = what;
		pNewItems[nItems].m_white = white;

		if (pItems) delete [] pItems;
		pItems = pNewItems;
		nItems++;
	}

	template <typename T, typename TM>
	bool raMap<T, TM>::GetItem(T what, TM& white)
	{
		int iIndex = FindItem(known);
		if (iIndex < 0) return false;
		white = pItems[iIndex].m_white;
		return true;
	}

	template <typename T, typename TM>
	void raMap<T, TM>::DeleteItem(T what)
	{
		int iIndex = FindItem(what);
		if (iIndex == -1) return;
		if (nItems == 1)
		{
			Clear();
			return;
		}

		raMapItem* pNewItems = new ITEM[nItems - 1];

		memcpy(pNewItems, pItems, iIndex * sizeof(raMapItem));
		memcpy(&pNewItems[iIndex], pItems, (nItems - iIndex - 1) * sizeof(raMapItem));

		delete [] pItems;
		pItems = pNewItems;
		nItems--;
	}