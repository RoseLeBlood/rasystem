#include "..\raSystem\include\raMain.h"
#include "raApplication.h"

using namespace System;

	///-----------------------APPLICATION CLASS ----------------------------------
	raApplication* raApplication::m_instance = NULL;	// statische Instance der Klasse

	raApplication::raApplication(void)
	{
	}

	raApplication::~raApplication(void)
	{
	}
	void raApplication::SetVisualStyle()
	{
		static bool bInitialized = false;
		if (!bInitialized)
		{
			INITCOMMONCONTROLSEX pInit;
			pInit.dwSize = sizeof(INITCOMMONCONTROLSEX);
			pInit.dwICC  = ICC_ANIMATE_CLASS      | ICC_BAR_CLASSES        | ICC_COOL_CLASSES       |
						   ICC_DATE_CLASSES       | ICC_HOTKEY_CLASS       | ICC_INTERNET_CLASSES   |
						   ICC_LISTVIEW_CLASSES   | ICC_NATIVEFNTCTL_CLASS | ICC_PAGESCROLLER_CLASS |
						   ICC_PROGRESS_CLASS     | ICC_TAB_CLASSES        | ICC_TREEVIEW_CLASSES   |
						   ICC_UPDOWN_CLASS       | ICC_USEREX_CLASSES     | ICC_WIN95_CLASSES;
			InitCommonControlsEx(&pInit);

			#if (_WIN32_WINNT >= 0x501)
			pInit.dwICC  = ICC_STANDARD_CLASSES   | ICC_LINK_CLASS;
			InitCommonControlsEx(&pInit);
			#endif

			bInitialized = true;
		}
	}
	raApplication& raApplication::Instance()
	{
		if(!m_instance)	// Ist m_instance == null
		{
			// Ja
			CreateInstance();				// Instance erstellen
			m_instance->m_prg = NULL;		// Es ist noch kein Programm vorhanden
			m_instance->m_while = true;
		}
		return *m_instance;					// Instance zurückgeben
	}
	void raApplication::SetStartUpParameter(HINSTANCE hInstance)
	{
		m_inst = hInstance;
	}
	void raApplication::CreateInstance()
	{
		// Instance pointer erstellen ( speicher alloc )
		m_instance = new raApplication();
	}
	void raApplication::UnInstance()
	{
		// Wenn m_instance != null ist dann delete[] m_instance & m_instance = null
		SAFE_DELETE(m_instance);
	}
	int raApplication::RunApp()
	{
		DWORD retval;
		m_while = true;
		try	// Versuche WindowLoop auszuführen
		{
			retval = WindowLoop(m_peek);
		}
		catch ( raException e )	// Beim Fehler -> raException Klasse
		{
			// Fehler ausgeben in Konsole und Log
			char buf [50];
			sprintf_s ( buf, "%s, Error %d", e.GetMessage (), e.GetError ());
			RERROR (buf);
			// In retval den Error Code der Exception speichern
			retval = e.GetError();
		}
		catch (...)	// Bei sonstige Exception
		{
			// Fehler Unbekannt in Konsole und Log ausgeben
			RERROR("Unknown - Exception");
			// In retval -1 für unbekannt speichern
			retval = -1;
		}
		// Engine bzw. System schließen
		System::raCloseEngine();
		// Error Code zurück geben
		return retval;
	}
	int raApplication::WindowLoop(bool Peek)
	{
		MSG msg;
		if(!m_prg) return 0;						// Ohne Programm Klasse startet hier nichts

		if(!m_prg->Create()) return -1;							// Programm erstellen

		if(!Peek)									//Ist Peek == true
		{
			// Nein also false dann verwende ein Message Loop der nur
			// reagiert wenn eine Nachricht vorliegt

			// Sol lange bis keine Nachrichten mehr vorliegen

			while (GetMessage (&msg, NULL, 0, 0))
			{
				TranslateMessage(&msg);	// Message übersetzen
				DispatchMessage(&msg);	// Message an Message Proc senden

				if(!m_prg->Idle())
					return -1;
			}
		}
		else
		{
			// Peek ist true
			// Dann verwenden wir ein Message Loop der nicht auf Nachrichten wartet sondern
			// immer nur fragt ob eine Nachricht da ist -> sie behandelt und weiter geht's
			// kein Message lock

			while(m_while)
			{
				// Nachricht vorhanden?
				if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					// Ja dann Nachricht übersetzen und an Message Pro senden
					TranslateMessage(&msg);
					DispatchMessage(&msg);

					// Ist die Nachricht WM_QUIT dann ...
					if(msg.message == WM_QUIT)
						Exit();
				}
				else
				{
					// Nein Keine Nachricht dann ruf m_prg->idle auf
					if(!m_prg->Idle())
						return -1;
				}
			}
		}

		// Programm zerstören
		m_prg->Destroy();
		SAFE_DELETE(m_prg);

		return (int)msg.wParam;
	}