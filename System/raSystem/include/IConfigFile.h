#pragma once

	class RAPI IConfigFile
	{
	public:
		IConfigFile(void)
		{
			m_isloaded = false;
		}
		virtual ~IConfigFile(void)
		{
		}

		virtual bool Open(const char filename[]) { return true; };
		virtual void Clear() { };
		virtual void Close() { };
		virtual bool Good()  { return true; };

		virtual int GetInt(const char section[], const char key[]) { return 0; };
		virtual long GetLong(const char section[], const char key[]) { return 0; };
		virtual bool GetBool(const char section[], const char key[]) { return false; };
		virtual raString GetString(const char section[], const char key[]) { return ""; };
		virtual raString Get(const char section[], const char key[]) { return ""; };

		virtual void SetInt(const char section[],const char key[], int newval) { };
		virtual void SetLong(const char section[], const char key[], long newval) { };
		virtual void SetBool(const char section[], const char key[], bool newval) { };
		virtual void SetString(const char section[], const char key[], raString newval) { };
		virtual void Set(const char section[], const char key[], const char newval[]) { };

		virtual void CreateSection(const char section[]) { };
		virtual void RemoveSection(const char section[]) { };
		virtual void RemoveEntry(const char section[], const char key[]) { };
		virtual bool ValidSection(const char section[]) { return false; };
		virtual bool ValidEntry(const char section[], const char key[]) { return false; };
		virtual int CountSections() { return 0; };
		virtual int CountEntries() { return 0; };

		bool IsLoaded()			{ return m_isloaded; }
	protected:
		bool					m_isloaded;
	};