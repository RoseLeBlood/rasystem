#pragma once

namespace GUI
{
	class RGAPI raMenu
	{
	public:
		bool Destroy();

		bool Create();

		bool AppendItem(UINT uiID, LPCTSTR lpItem, UINT uiFlags = MFS_ENABLED); // Use MFT_* and MFS_* constants
		bool AppendSubMenu(raMenu *hSubMenu, LPCTSTR lpSubMenuText);
		bool InsertItem(UINT uiID, UINT uiPrevID, LPCTSTR lpItem, UINT uiFlags = MFS_ENABLED);
		bool InsertSubMenu(UINT uiPrevID, raMenu *hSubMenu, LPCTSTR lpSubMenuText);
		bool DeleteItem(UINT uiID);

		bool SetItemChecked(UINT uiID, bool bChecked);
		bool SetItemEnabled(UINT uiID, bool bEnabled);
		bool SetRadioChecked(UINT uiIDFirst, UINT uiIDLast, UINT uiIDCheck);
		bool SetDefaultItem(UINT uiID);

		UINT GetItemState(UINT uiID) const; // MFS_DEFAULT, MFS_DISABLED, MFS_HILITE, MFS_CHECKED (radio item with MFT_RADIOCHECK)
		UINT GetItemID(int nPos) const;
		int  GetItemCount() const;
		UINT GetDefaultItem() const;

		HMENU GetSubMenu(int nPos) const;

		HMENU	GetMenu()	{ return m_hMenu; }

	protected:
		HMENU			m_hMenu;
	};
}