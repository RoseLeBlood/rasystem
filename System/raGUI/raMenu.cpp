#include "raGuiMain.h"

namespace System
{
namespace GUI
{
bool raMenu::Destroy()
{
	bool bReturn = (::DestroyMenu(m_hMenu) != FALSE);
	m_hMenu = NULL;
	return bReturn;
}

bool raMenu::DeleteItem(UINT uiID)
{
	return (::DeleteMenu(m_hMenu, uiID, MF_BYCOMMAND) != FALSE);
}

bool raMenu::SetDefaultItem(UINT uiID)
{
	return (::SetMenuDefaultItem(m_hMenu, uiID, FALSE) != FALSE);
}

UINT raMenu::GetDefaultItem() const
{
	return ::GetMenuDefaultItem(m_hMenu, FALSE, GMDI_USEDISABLED);
}

UINT raMenu::GetItemState(UINT uiID) const
{
	return ::GetMenuState(m_hMenu, uiID, MF_BYCOMMAND);
}

UINT raMenu::GetItemID(int nPos) const
{
	return ::GetMenuItemID(m_hMenu, nPos);
}

int raMenu::GetItemCount() const
{
	return ::GetMenuItemCount(m_hMenu);
}

HMENU raMenu::GetSubMenu(int nPos) const
{
	return ::GetSubMenu(m_hMenu, nPos);
}

bool raMenu::AppendItem(UINT uiID, LPCTSTR lpItem, UINT uiFlags)
{
	return (::AppendMenu(m_hMenu, MF_BYCOMMAND | uiFlags, uiID, lpItem) != FALSE);
}

bool raMenu::AppendSubMenu(raMenu * hSubMenu, LPCTSTR lpSubMenuText)
{
	return (::AppendMenu(m_hMenu, MF_BYCOMMAND | MF_POPUP, reinterpret_cast<UINT_PTR>(hSubMenu->GetMenu()), lpSubMenuText) != FALSE);
}

bool raMenu::InsertItem(UINT uiID, UINT uiPrevID, LPCTSTR lpItem, UINT uiFlags)
{
	return (::InsertMenu(m_hMenu, uiPrevID, MF_BYCOMMAND | uiFlags, uiID, lpItem) != FALSE);
}

bool raMenu::InsertSubMenu(UINT uiPrevID, raMenu * hSubMenu, LPCTSTR lpSubMenuText)
{
	return (::InsertMenu(m_hMenu, uiPrevID, MF_BYCOMMAND | MF_POPUP, reinterpret_cast<UINT_PTR>(hSubMenu->GetMenu()), lpSubMenuText) != FALSE);
}

bool raMenu::SetRadioChecked(UINT uiIDFirst, UINT uiIDLast, UINT uiIDCheck)
{
	return (::CheckMenuRadioItem(m_hMenu, uiIDFirst, uiIDLast, uiIDCheck, MF_BYCOMMAND) != FALSE);
}

bool raMenu::Create()
{
	m_hMenu = ::CreateMenu();
	return (m_hMenu != NULL);
}

bool raMenu::SetItemChecked(UINT uiID, bool bChecked)
{
	UINT uiChecked = MF_UNCHECKED;
	if (bChecked) uiChecked = MF_CHECKED;
	return (CheckMenuItem(m_hMenu, uiID, MF_BYCOMMAND | uiChecked) != -1);
}

bool raMenu::SetItemEnabled(UINT uiID, bool bEnabled)
{
	UINT uiEnabled = MF_GRAYED;
	if (bEnabled) uiEnabled = MF_ENABLED;
	return (::EnableMenuItem(m_hMenu, uiID, MF_BYCOMMAND | uiEnabled) != -1);
}
}
}