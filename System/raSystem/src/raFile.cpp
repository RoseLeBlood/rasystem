#include "..\include\raMain.h"

namespace raSystem
{
	struct raZipHeader
	{
		DWORD	dwSignature;
		WORD	wVersionNeeded;
		WORD	wGeneralPurpose;
		WORD	wCompressionMethod;
		WORD	wLastModTime;
		WORD	wLastModDate;
		DWORD	dwCRC32;
		DWORD	dwCompressedSize;
		DWORD	dwUncompressedSize;
		WORD	wFilenameLength;
		WORD	wExtraLength;
	};
	//#pragma pack(pop)

	raFile::raFile(void)
	{
	}


	raFile::~raFile(void)
	{
	}
	

	

	DWORD	raCRC32Table[256];		// CRC32-Tabelle für die Entschlüsselung
	BOOL	raCRC32TableFilled = FALSE;	// Ist die Tabelle gefüllt?

	DWORD raCRC32(DWORD a, BYTE b)
	{
		// Liefert den CRC32-Wert zweier Zahlen zurück
		return raCRC32Table[(a ^ b) & 0xFF] ^ (a >> 8);
	}
	bool raFile::FillCRC32Table()
	{
		DWORD c;

		if(raCRC32TableFilled) 
			return true;

		// Tabelle mit speziellem Verfahren ausfüllen
		for(DWORD a = 0; a < 256; a++)
		{
			c = a;
			for(int b = 0; b < 8; b++)
			{
				c = c & 1 ? 0xEDB88320 ^ (c >> 1) : c >> 1;
			}

			// Eintrag speichern
			raCRC32Table[a] = c;
		}

		// Tabelle gefüllt
		raCRC32TableFilled = TRUE;
		RINFO("CRC32-Tabelle wurde generiert!");

		return true;
	}
	bool raFile::UpdateZipKeys(DWORD* pdwKey, BYTE c)
	{
		if(!pdwKey) 
			return false;
		pdwKey[0] = raCRC32(pdwKey[0], c);
		pdwKey[1] += pdwKey[0] & 0x000000FF;
		pdwKey[1] = pdwKey[1] * 134775813 + 1;
		pdwKey[2] = raCRC32(pdwKey[2], (BYTE)(pdwKey[1] >> 24));

		return true;
	}

	BYTE DecryptZipByte(DWORD* pdwKey)
	{
		WORD wTemp;
		wTemp = (WORD)(pdwKey[2] | 2);
		return (((wTemp * (wTemp ^ 1)) >> 8) & 0xFF);
	}
	bool raFile::DecryptZipObject(DWORD dwObjectCRC32, BYTE* pEncryptionHeader, BYTE* pData,
								   raString pcPassword)
	{
		DWORD	adwKey[3];	// Die drei Schlüssel
		BYTE	Temp;		// Temporäre Variable
		int		iSize;		// Größe des Objekts


		if(!pData)		{ RERROR_NULLPOINTER("pData"); return false; }


		// Wenn die CRC32-Tabelle noch nicht ausgefüllt wurde, dann wird
		// das jetzt getan.
		if(!raCRC32TableFilled) 
			FillCRC32Table();

		// Die drei Schlüssel initialisieren
		adwKey[0] = 305419896;
		adwKey[1] = 591751049;
		adwKey[2] = 878082192;

		// Schlüssel auf Passwort einstellen
		for(int a = 0; a < (int)(strlen(pcPassword.c_str())); a++) 
			UpdateZipKeys(adwKey, (BYTE)(pcPassword[a]));

		// Verschlüsselungsheader entschlüsseln
		for(int a = 0; a < 12; a++)
		{
			Temp = *((BYTE*)(pEncryptionHeader) + a) ^ DecryptZipByte(adwKey);
			pEncryptionHeader[a] = Temp;
			UpdateZipKeys(adwKey, Temp);
		}

		// Testen, ob das Passwort in Ordnung war
		if((BYTE)((dwObjectCRC32 & 0xFF000000) >> 24) != pEncryptionHeader[11]) return false;

		// Nun werden die Daten entschlüsselt.
		iSize = raSystem::raMemManager::Instance().MemGetSize(pData);
		for(int a = 0; a < iSize; a++)
		{
			Temp = pData[a] ^ DecryptZipByte(adwKey);
			pData[a] = Temp;
			UpdateZipKeys(adwKey, Temp);
		}

		return true;
	}

	// ******************************************************************
	// Passwort entschlüsseln
	bool raFile::DecryptPassword( raString pcPassword)
	{
		// Schlüsselwerte, die zum Entschlüsseln des Passworts benötigt werden
		int aiKey[50] = {0x02, 0x03, 0x05, 0x07, 0x11, 0x13, 0x17, 0x19, 0x23, 0x29,
						 0xA2, 0xB3, 0xC5, 0xD7, 0xE1, 0xF3, 0xA7, 0xB9, 0xC3, 0xD9,
						 0x93, 0xA4, 0xB6, 0xC8, 0xD2, 0xE4, 0x98, 0xA8, 0xB4, 0xC8,
						 0x46, 0x58, 0x63, 0x67, 0x74, 0x78, 0x57, 0x57, 0x68, 0x67,
						 0xA9, 0xBC, 0xC9, 0xDF, 0xF6, 0x0C, 0xBF, 0xCF, 0xFC, 0xFF};

		int iHexToDec[256];		// Umrechungstabelle
		char acPassword[256];	// Neues Passwort


		// Hexadezimaltabelle initialisieren
		ZeroMemory(iHexToDec, 256 * sizeof(int));
		iHexToDec['0'] = 0;
		iHexToDec['1'] = 1;
		iHexToDec['2'] = 2;
		iHexToDec['3'] = 3;
		iHexToDec['4'] = 4;
		iHexToDec['5'] = 5;
		iHexToDec['6'] = 6;
		iHexToDec['7'] = 7;
		iHexToDec['8'] = 8;
		iHexToDec['9'] = 9;
		iHexToDec['A'] = iHexToDec['a'] = 10;
		iHexToDec['B'] = iHexToDec['b'] = 11;
		iHexToDec['C'] = iHexToDec['c'] = 12;
		iHexToDec['D'] = iHexToDec['d'] = 13;
		iHexToDec['E'] = iHexToDec['e'] = 14;
		iHexToDec['F'] = iHexToDec['f'] = 15;

		// Das Passwort liegt im Hexadezimalformat vor. Dieses wandeln
		// wir jetzt in Bytefolgen um.
		ZeroMemory(acPassword, 256 * sizeof(char));
		for(int a = 0; a < (int)(strlen(pcPassword.c_str())); a += 2)
		{
			acPassword[a / 2] = iHexToDec[pcPassword[a]] * 16 +
								iHexToDec[pcPassword[a + 1]];
		}

	
		// Das Passwort wird entschlüsselt.
		// Zuerst wird seine Länge verwendet, um den Zufallszahlen-
		// generator zu initialisieren. Das Initialisieren passiert
		// mehrfach, um eine höhere Sicherheit zu gewährleisten.
#ifndef WIN64
		srand(strlen(acPassword) * 17);
#else
		size_t len = strlen(acPassword);
		srand(UINT(len * 17));
#endif
		for(int a = 0; a < (int)(strlen(acPassword)); a++)
		{
			// Zufallszahlengenerator initialisieren
#ifndef WIN64
			srand(rand() + strlen(acPassword) * a);
#else
			len = strlen(acPassword);
			srand(UINT(rand() + len * a));
#endif
			srand((rand() % (aiKey[a % 50])) + (rand() % (aiKey[(a * 23) % 50])));
			srand((rand() % (aiKey[(a + 2305) % 50])) + (rand() % (aiKey[(17 + a * 133) % 50])) * 177);

			// Entschlüsselung des Passworts vornehmen
			pcPassword[a] = acPassword[a] - (rand() % 256);

			// Zufallszahlengenerator erneut initialisieren
			srand(((BYTE)(acPassword[a]) + 1) * (a + 23) + (rand() % 1381));
		}

		return true;
	}

	// ******************************************************************
	// Eine Datei einlesen (alle Typen)
	 bool raFile::ReadFile( raString pcFilename, void** ppOut)
	{
		char	acFilename[256];	// Kopie des Dateinamens (zum Basteln)
		 raString	*pcAt;				// Position des "@"-Zeichens im Dateinamen
		 raString	*pcLeft;				// Linker Teil des Dateinamen
		 raString	*pcRight;			// Linker Teil des Dateinamen
		 raString	*pcPassword;			// Passwort


		// Dateiname kopieren
		strcpy(acFilename, pcFilename.c_str());
		pcFilename = acFilename;

		// Nach einem "@"-Zeichen im Dateinamen suchen.
		// Falls eines gefunden wird, wird der Dateiname an dieser Stelle
		// geteilt - der vordere Teil ist der Objektname und der hintere
		// der Dateiname der enthaltenden Datei.
		pcAt = new raString(strrchr(pcFilename.c_str(), '@'));
		if(!pcFilename.find('@'))
		{
			// Kein "@"-Zeichen gefunden! Die Datei wird ganz normal
			// eingelesen. Dazu rufen wir ReadRawFile auf.
			return ReadRawFile(pcFilename, ppOut);
		}

		// Es ist also ein "@"-Zeichen im Dateinamen.
		// Wir kennen seine Position, also können wir den Dateinamen in
		// zwei Strings aufteilen. Das "@"-Zeichen ersetzen wir einfach
		// durch eine binäre null, damit dort der erste String endet.
		*pcAt = raString(0);
		pcLeft = new raString(pcFilename);
		pcRight = pcAt + 1;

		// Falls versucht wird, eine verschlüsselte Zip-Datei zu lesen,
		// muss zusätzlich ein Passwort angegeben werden. Es beginnt nach
		// dem ">"-Zeichen, was wir jetzt suchen.
		pcPassword = new raString(strrchr(pcRight->c_str(), '>'));
		if(pcPassword->c_str())
		{
			// Das ">"-Zeichen wurde gefunden.
			// Es wird durch eine binäre Null ersetzt, damit der String
			// des Dateinamen der Zip-Datei dort endet.
			*pcPassword = raString(0);
			pcPassword++;

			// Wenn das Passwort verschlüsselt ist, wird es jetzt entschlüsselt.
			if(*pcPassword->c_str() == '?') 
				DecryptPassword(*(++pcPassword));
		}

		// Nun können wir die Zip-Dateifunktion aufrufen.
		return ReadZipFileObject(*pcRight, *pcLeft, *pcPassword, ppOut);
	}

	// ******************************************************************
	// Zip-Objekt lesen
	 bool raFile::ReadZipFileObject( raString pcZipFilename,  raString pcObjectName,
									 raString pcPassword, void** ppOut)
	{
		FILE*		pZipFile;				// Handle der geöffneten Zip-Datei
		raZipHeader	ZipHeader;				// Header der Zip-Datei
		char		acObjectName[256];		// Name des aktuellen Objekts
		BOOL		bFinished;				// Fertig mit dem Lesen?
		BOOL		bEncrypted;				// Sind die Daten verschlüsselt?
		BYTE		aEncryptionHeader[12];	// 12 Bytes großer Verschlüsselungsheader
		void*		pData;					// Daten des Objekts
		int			iSize;					// Größe des Objekts




		// Datei binär zum Lesen öffnen
		pZipFile = fopen(pcZipFilename.c_str(), "rb");
		if(!pZipFile) 
		{
			RERROR_FILE(pcZipFilename);
			return false;
		}

		// Die Zip-Datei ist geöffnet.
		// Wir lesen jetzt so lange Objekte ein, bis wir das passende
		// gefunden haben.
		bFinished = FALSE;
		bEncrypted = FALSE;
		while(!bFinished)
		{
			// Den Header einlesen
			if(fread(&ZipHeader, sizeof(raZipHeader), 1, pZipFile) != 1)
			{
				fclose(pZipFile);
				RERROR("Fehler beim Lesen des Headers!");
				return false;
			}

			// Prüfen, ob die Signatur stimmt. Falls nicht, sind wohl
			// alle Dateien gelesen worden.
			if(ZipHeader.dwSignature != 0x04034B50)
			{
				fclose(pZipFile);
				RERROR("Das Objekt wurde nicht gefunden!");
				return false;
			}

			// Falls Bit Nr. 1 des wGeneralPurpose-Elements gesetzt ist,
			// sind die Daten dieses Objekts verschlüsselt.
			if(ZipHeader.wGeneralPurpose & 0x0001) bEncrypted = TRUE;

			// Dateiname (Objektname) einlesen
			ZeroMemory(acObjectName, 256 * sizeof(char));
			if(fread(acObjectName, ZipHeader.wFilenameLength, 1, pZipFile) != 1)
			{
				fclose(pZipFile);
				RERROR("Fehler beim Lesen des Dateinamens!");
				return false;
			}

			// Vergleichen
			if(!_stricmp(pcObjectName.c_str(), acObjectName))
			{
				// Das richtige Objekt wurde gefunden!
				bFinished = TRUE;
			}

			// Die Extradaten überspringen
			fseek(pZipFile, ZipHeader.wExtraLength, SEEK_CUR);

			if(!bFinished)
			{
				// Die Daten des Objekts überspringen, falls es sich nicht
				// um das gesuchte Objekt handelt.
				fseek(pZipFile, ZipHeader.dwCompressedSize, SEEK_CUR);

				// Falls Bit Nr. 4 des wGeneralPurpose-Elements des Headers
				// gesetzt ist, folgt ein weiterer Datenbereich, den wir aber
				// nicht brauchen und einfach überspringen können.
				if(ZipHeader.wGeneralPurpose & 0x0008) 
					fseek(pZipFile, 12, SEEK_CUR);
			}
		}


		// Das Objekt wurde gefunden!
		// Es muss unkomprimiert sein, um gelesen zu werden.
		if(ZipHeader.wCompressionMethod != 0)
		{
			fclose(pZipFile);
			RERROR("Komprimiertes Objekt gefunden! Dekomprimierung wird nicht unterstützt!");
			return false;
		}

		// Falls das Objekt verschlüsselt ist, wird der 12 Bytes große
		// Verschlüsselungsheader, der sich vor den eigentlichen Daten
		// befindet, eingelesen werden.
		if(bEncrypted)
		{
			if(fread(aEncryptionHeader, 12, 1, pZipFile) != 1)
			{
				fclose(pZipFile);
				RERROR("Fehler beim Lesen des Verschlüsselungs-Headers!");
				return false;
			}
		}
	
		// Speicher wird für die Daten reserviert.
		pData = raSystem::raMemManager::Instance().MemAlloc(ZipHeader.dwCompressedSize - (bEncrypted ? 12 : 0));
		if(!pData) 
		{
			RERROR_OUTOFMEM();
			return false;
		}
		// Daten einlesen
		iSize = raSystem::raMemManager::Instance().MemGetSize(pData);
		if(fread(pData, iSize, 1, pZipFile) != 1)
		{
			raSystem::raMemManager::Instance().MemFree(pData);
			fclose(pZipFile);
			RERROR("Fehler beim Lesen der Objektdaten!");
			return false;
		}

		// Entschlüsseln, falls nötig
		if(bEncrypted)
		{
			if(DecryptZipObject(ZipHeader.dwCRC32, aEncryptionHeader,
								  (BYTE*)(pData), pcPassword))
			{
				raSystem::raMemManager::Instance().MemFree(pData);
				fclose(pZipFile);
				RERROR("Entschlüsselung fehlgeschlagen!");
				return false;
			}
		}

		// Datei schließen und Pointer zurückliefern
		fclose(pZipFile);

		// Zeiger liefern
		*ppOut = pData;

		return true;
	}

	// ******************************************************************
	// "Rohe" Datei lesen
	 bool raFile::ReadRawFile( raString pcFilename, void** ppOut)
	{
		struct stat FileStats;	// Statistiken der Datei
		void*		pData;		// Daten aus der Datei
		FILE*		pFile;		// Dateihandle

		// Statistiken der Datei abfragen (wir brauchen nur die Größe)
		ZeroMemory(&FileStats, sizeof(struct stat));
		stat(pcFilename.c_str(), &FileStats);
		if(FileStats.st_size <= 0) { RERROR_FILE(pcFilename); return false; }

		// Speicherbereich reservieren, der groß genug ist
		pData = raSystem::raMemManager::Instance().MemAlloc(FileStats.st_size);
		if(!pData) 
		{
			RERROR_OUTOFMEM();
			return false;
		}

		// Datei binär zum Lesen öffnen
		pFile = fopen(pcFilename.c_str(), "rb");
		if(!pFile)
		{
			raSystem::raMemManager::Instance().MemFree(pData);
			RERROR_FILE(pcFilename);
			return false;
		}

		// Daten lesen
		if(fread(pData, FileStats.st_size, 1, pFile) != 1)
		{
			raSystem::raMemManager::Instance().MemFree(pData);
			fclose(pFile);
			RERROR_FILE(pcFilename);
			return false;
		}

		// Datei schließen und Pointer zurückliefern
		fclose(pFile);

		// Zeiger liefern
		*ppOut = pData;

		return true;
	}

	// ******************************************************************
	// Daten in eine Datei schreiben
	 bool raFile::WriteFile( raString pcFilename, void* pData, int iNumBytes)
	{
		FILE* pFile; // Dateihandle

		// Parameter prüfen
		if(!pData)			{ RERROR_NULLPOINTER("pData"); return false; }
		if(iNumBytes <= 0)	{ RERROR_INVALIDVALUE("iNumBytes");return false; }


		// Datei zum Schreiben im Binärmodus öffnen
		pFile = fopen(pcFilename.c_str(), "wb");
		if(!pFile) 
		{
			RERROR_FILE(pcFilename);
			return false;
		}

		// Daten schreiben
		if(fwrite(pData, iNumBytes, 1, pFile) != 1)
		{
			fclose(pFile);
			RERROR_FILE(pcFilename);
			return false;
		}

		// Datei schließen und fertig
		fclose(pFile);

		return true;
	}
};
