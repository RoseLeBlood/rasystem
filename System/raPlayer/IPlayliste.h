#pragma once

// Item eines Playlist items
struct PlaylistItem
{
	PlaylistItem() { }
	PlaylistItem(raString item, raString summary)  : m_Item(item), m_Summary(summary)  { }

	raString m_Item;			// Das Item Path + Filename
	raString m_Summary;			// Die Beschreibung
};
inline bool operator == (const PlaylistItem& a, const PlaylistItem& b)						{ return ((a.m_Item == b.m_Item) && (a.m_Summary == b.m_Summary)); }
inline bool operator != (const PlaylistItem& a, const PlaylistItem& b)						{ return ((a.m_Item != b.m_Item) && (a.m_Summary != b.m_Summary)); }

// abstractes Interface zum speichern von Playlists
class IPlayliste
{
public:
	IPlayliste(void)
	{
	}

	virtual ~IPlayliste(void) { }

	// Playlist erstellen
	virtual bool Create()
	{
		m_items = (new raArray<PlaylistItem>());		// Array Object für die Items erstellen

		//if(m_items) return true;
		//else return false;

		return true;
	}
	// Erstellen aus einem Playlist object (raGUI)
	virtual bool Create(System::GUI::raList *lst)
	{
		// Normal erstellen ?
		if(Create())
		{
			raString Data = "";
			// Solange bis alles items aus der liste durchgegangen sind
			for(unsigned int i = 0; i <=  lst->GetNumEntrys() - 1; i++)
			{
				if(lst->GetDate<raString>(i, &Data))	// Entry ok?
				{
					// Item hinzufügen
					AddItem(PlaylistItem(Data, lst->GetEntry(i)));
				}
			}
			return true;
		}
		return false;
	}
	// Item hinzufügen als PlaylistItem
	virtual int AddItem(PlaylistItem item)
	{
		if(m_items->Add(item) == S_OK)
		{
			return m_items->IndexOf(item);
		}
		return -1;
	}
	// Item entfernen
	virtual bool RemoveItem(int item)
	{
		return m_items->Remove(item) == S_OK;
	}
	// Item in ralist (raGUI)  hinzufügen
	virtual void WriteItemsInListBox(System::GUI::raList *lst)
	{
		lst->Clear();	// Zielobject leeren

		for(int i = 0; i < m_items->GetSize(); i++)
		{
			UINT index = lst->AddEntry(m_items->GetAt(i).m_Summary);
			lst->SetData<raString>(index, m_items->GetAt(i).m_Item);
		}
	}
	PROPERTY(raArray<PlaylistItem>*, Items);
	GET(Items)
	{
		return m_items;
	}
	SET(Items)
	{
		m_items = value;
	}
	READONLY_PROPERTY(int, Count);
	GET(Count)
	{
		return m_items->GetSize();
	}

	READONLY_PROPERTY(PlaylistItem, LastItem);
	GET(LastItem)
	{
		return m_items->GetAt(m_items->GetSize()-1);
	}
	READONLY_PROPERTY(PlaylistItem, FirstItem);
	GET(FirstItem)
	{
		return m_items->GetAt(0);
	}
	//-------------------------------------------------

	virtual bool Open(raString FileName) = 0;
	virtual bool Save(raString Filename) = 0;

protected:
	raArray<PlaylistItem>	*m_items;
};