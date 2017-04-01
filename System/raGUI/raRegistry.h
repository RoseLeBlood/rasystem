#pragma once

namespace GUI
{
	class RGAPI raRegistry : public raElement<raRegistry>
	{
	public:
		bool Create (raWND* ParentWindow,
					 unsigned long ID)
		{
			raElement<raRegistry>::m_Parent = ParentWindow;
			raElement<raRegistry>::m_ID = ID;
			raElement<raRegistry>::m_Type = WIN_REGESTRY;

			hKey = NULL;
			return true;
		}

		void Destroy() { }

		bool OpenKey(HKEY hRoot, LPCTSTR lpSubKey, REGSAM samDesired = KEY_ALL_ACCESS, bool bCreate = false);
		void CloseKey();
		bool DeleteKey(HKEY hRoot, LPCTSTR lpSubKey);
		bool DeleteValue(LPCTSTR lpField);
		bool GetValue(LPCTSTR lpField, LPVOID lpValue, DWORD dwSize);
		bool SetValue(LPCTSTR lpField, LPVOID lpValue, DWORD dwSize, DWORD dwType);
	private:
		HKEY hKey;
	};
}