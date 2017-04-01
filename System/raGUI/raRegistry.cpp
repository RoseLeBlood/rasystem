#include "raGuiMain.h"

namespace System
{
namespace GUI
{
	bool raRegistry::OpenKey(HKEY hRoot, LPCTSTR lpSubKey, REGSAM samDesired, bool bCreate)
	{
		CloseKey();
		LONG dwReturn = RegOpenKeyEx(hRoot, lpSubKey, 0, samDesired, &hKey);

		if (dwReturn == ERROR_SUCCESS)
			return true;
		if (bCreate)
			return (RegCreateKeyEx(hRoot, lpSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, samDesired, NULL, &hKey, NULL) == ERROR_SUCCESS);
		else
			return false;
	}

	void raRegistry::CloseKey()
	{
		if (hKey) RegCloseKey(hKey);
	}

	bool raRegistry::DeleteKey(HKEY hRoot, LPCTSTR lpSubKey)
	{
		return (RegDeleteKey(hRoot, lpSubKey) == ERROR_SUCCESS);
	}

	bool raRegistry::DeleteValue(LPCTSTR lpField)
	{
		return (RegDeleteValue(hKey, lpField) == ERROR_SUCCESS);
	}

	bool raRegistry::GetValue(LPCTSTR lpField, LPVOID lpValue, DWORD dwSize)
	{
		DWORD dwTypes = REG_DWORD | REG_BINARY | REG_SZ | REG_MULTI_SZ;
		return (RegQueryValueEx(hKey, lpField, NULL, &dwTypes, reinterpret_cast<LPBYTE>(lpValue), &dwSize) == ERROR_SUCCESS);
	}

	bool raRegistry::SetValue(LPCTSTR lpField, LPVOID lpValue, DWORD dwSize, DWORD dwType)
	{
		return (RegSetValueEx(hKey, lpField, 0, dwType, reinterpret_cast<const BYTE*>(lpValue), dwSize) == ERROR_SUCCESS);
	}
}
}