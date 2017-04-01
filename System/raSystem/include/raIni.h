#pragma once

/*
 * Copyright (C) Jefferson 2010 <web@jeffersongta.de> / <remis80@gmx.net>
 *
 http://creativecommons.org/licenses/by/3.0/de/deed.en_US

  You are free:

    * to Share — to copy, distribute and transmit the work
    * to Remix — to adapt the work

	Angepasst an raSystem von Philipp Schröck
*/

#define INI_DEFAULT_SECTION "rasystem"

#define INI_MAX_LENGTH 300
#define INI_MAX_LINES_PER_SEC 50
#define INI_MAX_SECTIONS 20
#define INI_LINE_DONTSAVE "rasystem-notsave"

	class RAPI raIni : public IConfigFile
	{
	public:
		raIni(void) : IConfigFile() { }

		virtual ~raIni(void) { }

		virtual bool Open(const char filename[]);
		virtual void Clear();
		virtual void Close();
		virtual bool Good();

		virtual int GetInt(const char section[], const char key[]);
		virtual long GetLong(const char section[], const char key[]);
		virtual bool GetBool(const char section[], const char key[]);
		virtual raString GetString(const char section[], const char key[]);
		virtual raString Get(const char section[], const char key[]);

		virtual void SetInt(const char section[],const char key[], int newval);
		virtual void SetLong(const char section[], const char key[], long newval);
		virtual void SetBool(const char section[], const char key[], bool newval);
		virtual void SetString(const char section[], const char key[], raString newval);
		virtual void Set(const char section[], const char key[], const char newval[]);

		virtual void CreateSection(const char section[]);
		virtual void RemoveSection(const char section[]);
		virtual void RemoveEntry(const char section[], const char key[]);
		virtual bool ValidSection(const char section[]);
		virtual bool ValidEntry(const char section[], const char key[]);
		virtual int CountSections();
		virtual int CountEntries();

	private:
		// internal functions
		raString getKeyFromString(raString mystring);
		raString getValueFromString(raString mystring);

	private:
		template <class T>
		raString numtostring(T num);
		template <class T>
		T stringtonum(raString mystring);

		raString filename;
		std::vector<raString> sectionnames;
		std::vector<std::vector<raString> > buffer;
		bool InitGood;
		bool Closed;
		bool withoutSections;
		int sections;
		int entries;
	};