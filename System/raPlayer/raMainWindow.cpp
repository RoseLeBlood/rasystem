#include "raPlayer.h"
HWND raf ;
bool next = false;

raMainWindow::raMainWindow(raInstance* Inst) : raFixedWindow(Inst)
{
}

raMainWindow::~raMainWindow(void)
{
}
bool raMainWindow::Create (DWORD dwSytle,char *pTitle,
						 raWindowClass *WindowClass,
						 raVector2 Groeße,
						 unsigned long ID)
{
	m_playitem = 0;
	m_ID = ID;
	m_ifseek = false;
	m_seekstart = true;

	// Die Basis Funktioen Create aufrufen um das Fenster zuerstellen
	if(raFixedWindow::Create(0, pTitle, WindowClass, Groeße, ID))
	{
		// Menu Items
		raMenu Main, Date, Controll, About;
		// Config datei des Players laden
		System::raIni *iniConfig = new System::raIni();

		// Config datei laden muss vorhanden sein sonst ende ""
		if(!iniConfig->Open("./raConfig/player.ini") ) return false;

		// Das Main Menu erstellen (1. Knotenpunkt )
		if(Main.Create())
		{
			Controll.Create();		// Das Controll Menu erstellen
			About.Create();			// Das About Menu erstellen

			if(Date.Create())		// Das Date Manu erstellen
			{
				// Menu Unterpunkte im Menu Date hinzufügen. Name der Items aus Config datei laden
				Date.AppendItem(MENU_DATEI_OPEM, iniConfig->GetString("Lang", "Open").c_str());			// Open
				Date.AppendItem(0, "", MF_SEPARATOR);													// Seperator
				Date.AppendItem(MENU_DATEI_LOAD, iniConfig->GetString("Lang", "LoadPlaylist").c_str());	// Load Playlist
				Date.AppendItem(MENU_DATEI_SAVE, iniConfig->GetString("Lang", "SavePlaylist").c_str());	// Save Playlist
				Date.AppendItem(0, "", MF_SEPARATOR);													// Seperator
				Date.AppendItem(MENU_DATEI_EXIT, iniConfig->GetString("Lang", "Exit").c_str());			// Exit

				// Menu Date zum Main Menu hinzufügen
				Main.AppendSubMenu(&Date, iniConfig->GetString("Lang", "Datei").c_str());
			}
			if(Controll.Create())
			{
				// Menu Unterpunkte im Menu Controll hinzufügen. Name der Items aus Config datei laden
				Controll.AppendItem(MENU_CONTROLL_VISO, iniConfig->GetString("Lang", "Viso").c_str());		// Play
				Controll.AppendItem(0, "", MF_SEPARATOR);
				Controll.AppendItem(MENU_CONTROLL_PLAY, iniConfig->GetString("Lang", "Play").c_str());		// Play
				Controll.AppendItem(MENU_CONTROLL_PAUSE, iniConfig->GetString("Lang", "Pause").c_str());	// Pause
				Controll.AppendItem(MENU_CONTROLL_STOP, iniConfig->GetString("Lang", "Stop").c_str());		// Stop
				Controll.AppendItem(0, "", MF_SEPARATOR);													// Seperator
				Controll.AppendItem(MENU_CONTROLL_PREV, iniConfig->GetString("Lang", "Prev").c_str());		// Zurück
				Controll.AppendItem(MENU_CONTROLL_VOR, iniConfig->GetString("Lang", "Next").c_str());		// Vor
				Controll.AppendItem(0, "", MF_SEPARATOR);
				Controll.AppendItem(MENU_CONTROLL_PLAYLISTCLEAR, iniConfig->GetString("Lang", "PlaylistClear").c_str()); // Clear der Playliste

				// Menu Controll zum Main Menu hinzufügen
				Main.AppendSubMenu(&Controll, iniConfig->GetString("Lang", "Controll").c_str());
			}
			if(About.Create())
			{
				About.AppendItem(MENU_ABOUT_WEBSITE, "Website");		// TODO
				About.AppendItem(MENU_ABOUT_INFO, "Info");				// TODO

				// Menu About zum Main Menu hinzufügen
				Main.AppendSubMenu(&About, "&?");
			}
			ShowWindow(SW_SHOW);
			SetMenu(&Main);
		}
		this->SetTransparenz(iniConfig->GetInt("sytle", "transparent"));	// Fenster auf (100 - "transparent") % Transparentz setzen :

		raVIS *wind = raGuiManager::Instance().GetWindowAs<raVIS>(200);			// Fenster der Visualation bekommen
		if(wind)																// Fenster object not null?
			wind->SetTransparenz(iniConfig->GetInt("sytle", "transparent"));	// Dann die Transparentz setzen wie main window

		// Slider hinzufügen
		Slider = AddElapsedSlider(0, 100, raVector2(43.0f, 3.0f), raVector2(415.0f, 20.0f), PLAYER_SLIDER);
		// ListBox Hinzufügen
		m_playlist = AddListBox(raVector2(3.0f, 23.0f), raVector2(494.0f, 115.0f), PLAYER_LIST);

		// Theme string aus Config laden und Path bauen
		raString ThemePath = raString("./raStyle/") + iniConfig->GetString("sytle", "theme") + raString("/");

		// Ini Config schließen und löschen
		iniConfig->Close();
		SAFE_DELETE(iniConfig);

		// BitmapsButtons hinzufügen mit den ICONS aus der Theme datei		   		Position			Größe					ID
		Play = this->AddBitmapButton(ThemePath + raString("player_play.png"), VECTOR2_2F(3.0f, 130),VECTOR2_2F(50.0f, 25.0f), PLAYER_PLAY);
		Pause = this->AddBitmapButton(ThemePath + raString("player_pause.png"), VECTOR2_2F(55.0f, 130), VECTOR2_2F(50.0f, 25.0f), PLAYER_PAUSE);
		Stop = this->AddBitmapButton(ThemePath + raString("player_stop.png"), VECTOR2_2F(107.0f, 130), VECTOR2_2F(50.0f, 25.0f), PLAYER_STOP);
		zurück = this->AddBitmapButton(ThemePath + raString("player_back.png"), VECTOR2_2F(159, 130), VECTOR2_2F(50.0f, 25.0f), PLAYER_ZURUCK);
		vor = this->AddBitmapButton(ThemePath + raString("player_next.png"), VECTOR2_2F(211, 130), VECTOR2_2F(50.0f, 25.0f), PLAYER_VOR);

		// Slider erstellen     Size				Position				  ID         3D
		volume = AddSlider(VECTOR2_2F(265, 130), VECTOR2_2F(100.0f, 20.0f), PLAYER_VOL,false);
		// Slieder aus der ElementenSamplung bekommen Position setzen
		((raSlider*)(GetElement(PLAYER_VOL)))->SetPosition(1000);

		// Instance der ListBox in "m_playlist" kopieren
		m_playlist = ((System::GUI::raList*)(GetElement(PLAYER_LIST)));
		// Drop erlauben
		DragAcceptFiles(m_playlist->GetHandle()->GetWnd(),true);

		// Timer erstellen
		//SetTimer(Wnd, PLAYER_TIMER, 100, NULL);

		SetClassLong(Wnd, -10, (LONG)RGB(255, 0, 0));

		// Aufruf der Funktion PlaylistStart zum laden der last.m3u
		PlaylistStart();

		return true;
	}
	return false;
}
void raMainWindow::OnDestroy(Event e, EventParam ep)
{
	// Stream Stoppen
	rapStream::Instance().StopStream();
	PostQuitMessage(0);	// Sende Quit Message 0 ( kein Fehler)
}
void raMainWindow::OnQuit(Event e, EventParam ep)
{
		// PLaylist last.m3u speichern
		PlaylistClose();
		::DestroyWindow(e.shwnd);	// Fenster zerstören
}
void raMainWindow::OnPaint(Event e, EventParam ep)
{
	// Fenster zeichenen
	PAINTSTRUCT ps;
	BeginPaint(e.shwnd, &ps);		// Beginnen

	EndPaint(e.shwnd, &ps);			// Ende
}
void raMainWindow::OnPlay(bool ex)
{
	raSmartPointer<raStream> ms = (rapStream::Instance().Stream);
	raString UserData;	//Deklaration der Varibale UserDate
	// Wird gerade ein stream wiedergegeben?
	if(ms->IsPlaying)
		return; // Wenn ja -> verlasse die Methode

	m_playitem = Playliste->GetSelectedIndex();	// Aus der Playliste von dem Selctierten Item den Index in m_playitem speichern

	// Die Daten des Selectierten Item bekommen -> ist das ok?
	if(Playliste->GetDate<raString>(m_playitem, &UserData))
	{
		// wenn ja:
		// Den Stream mit den Daten aus dem Item starten
		// Beim Fehler?
		if(!rapStream::Instance().PlayStream(UserData))
		{
			OnVor();	// Eins vor
		}

		// Alles ok? dann müste der Player spielen
		if(ms->IsPlaying)
		{
			// Die Länge des Streams bekommen und dem Slider als End Zeit übergeben (max)
			((raElapsedSlider*)(GetElement(PLAYER_SLIDER)))->SetTimeLength(rapStream::Instance().GetStreamLength());
			// timer starten
			SetTimer(Wnd, PLAYER_TIMER, 600, (TIMERPROC) NULL);
		}
	}
}
void raMainWindow::OnStop()
{
	raSmartPointer<raStream> ms = (rapStream::Instance().Stream);
	if(!ms.Data) return;

	// Beim Drücken des Buttons []
	// Wird überhaupt was abgespielt?
	if(ms->IsPlaying)
	{
		// Ja.
		// Timer zerstören "anhalten"
		KillTimer(Wnd, PLAYER_TIMER);
		// Stream Stoppen
		rapStream::Instance().StopStream();
		// Setze die Spielzeit auf 0 und die Position auf 0
		((raElapsedSlider*)(GetElement(PLAYER_SLIDER)))->SetTimeLength(0);
		((raElapsedSlider*)(GetElement(PLAYER_SLIDER)))->SetTimePosition(0);

		// Angezeigter String der Visualation wieder auf ""  (nichts) stellen
		raVIS *window = raGuiManager::Instance().GetWindowAs<raVIS>(200);	// Thread Objekt aus Fenster ID 200 bekommen
		if(window)	// Ist thread?
			window->VISThread->Text = L"";	// Ja Text ändern

		Sleep(10);
	}
}
void raMainWindow::OnPause()
{
	// Beim Drücken des Buttons ||
	// Wird überhaupt was abgespielt?
	if(rapStream::Instance().Stream->IsPlaying)
	{
		// Wenn ja Pause
		rapStream::Instance().PauseStream();
	}
}
void raMainWindow::OnVor()
{
	// Gibt es eine nächstes Item
	if(Playliste->SetSelectedIndex(m_playitem+1))
	{
		// Dann stopp den Stream und starte ein neuen
		OnStop();
		OnPlay();
	}
}
void raMainWindow::OnZurück()
{
	// Beim Drücken des Buttons |<
	// Wird überhaupt was abgespielt?
	if(Playliste->SetSelectedIndex(m_playitem-1))
	{
		OnStop();
		OnPlay();
	}
}
void raMainWindow::OnList(EventParam ep)
{
	switch(LOWORD(ep.swParam))
        {
            case PLAYER_LIST:
               // Unsere Listbox?
                switch(HIWORD(ep.swParam))
                {
                    case LBN_DBLCLK:	// Beim Doppelklick
						OnStop();
                        OnPlay();		// Stream wiedergeben
						break;
                }
            break;
        }
}
void raMainWindow::OnVKeyToItem(Event e, EventParam ep)
{
	if(LOWORD(ep.swParam) == RKEY_RETURN)		// Hit Return
		OnPlay();
	else if(LOWORD(ep.swParam) == RKEY_DELETE)	// Hit Delete
	{
		// Speichern der ID vom Selectierten Item
		int select = Playliste->GetSelectedIndex();

		// Ist der Selectierte Item auch der gerade gespielter oder ist der Selectierte Item vorm gerade abgespielten?
		if(select <= m_playitem) m_playitem--;	// Ja dann verringer den Index für den gerade gespielten Item

		// Selectierten Item löschen
		Playliste->DelSelectedEntry();
		// Denn davor Selectierten makieren
		Playliste->SetSelectedIndex(select--);
	}
	else if(LOWORD(ep.swParam) == RKEY_F1)	// Hit F1
	{
		Playliste->Clear();	// Playliste löschen
		m_playitem = 0;				// Reset
	}
}
void raMainWindow::OnCommand(Event e, EventParam ep)
{
	// welcher Button würde gedrückt

	switch LOWORD(ep.swParam)
	{
		case WM_CREATE:
			KillTimer(Wnd, PLAYER_TIMER);
			break;
		case MENU_DATEI_EXIT:							// Menu Datei Exit
			PlaylistClose();							// Playliste speichern last
			::SendMessageA(Wnd, WM_DESTROY, 0,0);	// Message zum Beenden des Programmes senden
			break;
		case WM_QUIT:
		case WM_DESTROY:
			PlaylistClose();
			break;
		case PLAYER_PLAY:								// Taste Play
			OnPlay();									// Aufruf: OnPlay
			break;
		case PLAYER_STOP:								// Taste Stop
			OnStop();									// Aufruf: OnStop
			break;
		case PLAYER_PAUSE:								// Taste Pause
			OnPause();									// Aufruf OnPause
			break;
		case PLAYER_VOR:								// Taste Vor
			OnVor();									// Aufruf: OnVor
			break;
		case PLAYER_ZURUCK:								// Taste Zurück
			OnZurück();									// Aufruf: OnZurück
			break;
		case PLAYER_LIST:								// Ereigniss Listbox
			OnList(ep);									// Aufruf: OnList
			break;
		case MENU_DATEI_SAVE:							// Menu Datei Playlist save
			OnSavePlaylist();							// Aufruf: OnSavePlaylist
			break;
		case MENU_DATEI_LOAD:							// Menu Datei Playlist load
			OnLoadPlaylist();							// OnLoadPlaylist
			break;
		case MENU_DATEI_OPEM:							// Menu Datei öffnen
			OnOpen();									// Aufruf: OnOpen
			break;
		case MENU_CONTROLL_PLAYLISTCLEAR:				// Menu Controll Clear
			Playliste->Clear();					// Playliste löschen
			m_playitem = 0;								// Reset
			break;
		case MENU_ABOUT_INFO:
			raGuiManager::Instance().GetWindowAs<raInfoWindow>(PLAYER_INFO_WINDOW)->HideMe();
			break;
		case MENU_CONTROLL_VISO:
			raGuiManager::Instance().GetWindowAs<raVIS>(200)->HideMe();
			break;
	}
}
void raMainWindow::OnMenuInfo()
{
}
// Beim Drücken auf öffnen
void raMainWindow::OnOpen()
{
	raFileDialog* filedialog = (new raFileDialog());	// FileDialog object erstellen

	// Filter hinzufügen
	filedialog->Filter("MPEG Audio (mp3)\0*.mp3\0" \
					   "Free Lossless Audio Codec (FLAC)\0*.flac\0" \
					   "Xiph.Org Foundation (ogg)\0*.ogg\0" \
					   "MPEG Audio (mp2)\0*.mp2\0" \
					   "Windows Media Audio (wma)\0*.wma\0" \
					   "RIFF-WAVE (wav)\0*.wav\0" \
					   "Audio Interchange File Format (aiff)\0*.aif\0" \
					   "Advanced Streaming Format Audio (asf)\0*.asf\0" \
					   "Advanced Stream Redirecting  (wvf)\0*.wvf\0" \
					   "DLS format (dls)\0*.dsf\0" \
					   "Alle Dateien *.*\0*.*\0");

	// Filedialog öffnen und Ausgewählte datei in raString buffer speichern
	raString buffer = filedialog->Open(this);

	// Hat der User den Filedialog nicht abgebrochen?
	if(buffer != USER_FILE_ABORT)
	{
		// Neun dann adde den Eintrag mit Hintergrung daten in die PLaylist
		UINT index = m_playlist->AddEntry(Util::raFile::raRemoveDir(buffer));
		m_playlist->SetData<raString>(index, buffer);

		Playliste->SetSelectedIndex(index);	// Den gerade hinzugefügten makieren
		OnStop();									// Stream Stopen
		OnPlay();									// Stream starten
	}
	SAFE_MEMFREE(filedialog);
}
// Die letzte Wiedergabe lieste wiederherstellen
void raMainWindow::PlaylistStart()
{
	// Playlist object erstellen (laden von m3u datein)
	IPlayliste* playliste = (new raPlayerPlayliste());

	// Exestiert die Datei?
	if(System::Util::raFile::raFileExists("last.m3u"))
	{
		// Ja:
		playliste->Create();						// Playliste erstellen
		playliste->Open("last.m3u");					// Playliste mit den Inhalt aus der Datei "last.m3u" füllen
		playliste->WriteItemsInListBox(m_playlist);	// Items der Playliste in die Playliste des Players schreiben
	}
	SAFE_DELETE(playliste);							// Playlist object zerstören
}
// Menu Datei Load Playlist
void raMainWindow::OnLoadPlaylist()
{
	raFileDialog filedialog = raFileDialog();				// FileDialog object erstellen

	filedialog.AddExtension("Playlist (m3u)", "*.m3u");		// Filter des FileDialogs setzen

	raString buffer = filedialog.Open(this);					// FileDialog anzeigen ergebnis in buffer speichern

	if(buffer != USER_FILE_ABORT)								// User nicht abgebrochen?
	{
		raSmartPointer<IPlayliste>  playliste = raSmartPointer<IPlayliste> (new raPlayerPlayliste());			// playlist Object erstellen
		playliste->Create();									// Playlist erstellen
		playliste->Open(buffer);								// Playlist mit dem Ihnhalt aus der gewälten Datei füllen
		playliste->WriteItemsInListBox(m_playlist);				// Playlist des Players mit inhalt der Playlist füllen
	}
}
// Save Playlist bein beendigung des Programmes
void raMainWindow::PlaylistClose()
{
	if(m_playlist->GetNumEntrys() == 0) return;					// Wenn nichts drinne ist muss nichts gespeichert werden

	raSmartPointer<IPlayliste> playliste = raSmartPointer<IPlayliste>(new raPlayerPlayliste());			// Playlist object erstellen

	playliste->Create(m_playlist);								// Playlist mit Ihnhalt der spieler Playlist füllen
	playliste->Save("last.m3u");									// Playlist speichern
}
// Menu Datei Save Playlist
void raMainWindow::OnSavePlaylist()
{
	raFileDialog filedialog = raFileDialog();
	raSmartPointer<IPlayliste> playliste = raSmartPointer<IPlayliste>(new raPlayerPlayliste());

	filedialog.AddExtension("Playliste m3u", "*.m3u");
	raString buffer = filedialog.Save(this);

	if(buffer != USER_FILE_ABORT)
	{
		playliste->Create(m_playlist);
		playliste->Save(buffer);
	}
}

void raMainWindow::OnHScroll(Event e, EventParam ep)
{
	FMOD::ChannelGroup  * chGroup; // Deklaration als zeiger
	// Aus der Stream Klasses (Singleton) den FMOD System treiber bekommen und die Master Channel Group ( der Master Channel)
	rapStream::Instance().System->FSystem->getMasterChannelGroup(&chGroup);
	DWORD dwPos = 0;

	if(((raSlider*)(GetElement(PLAYER_VOL)))->GetHandle()->GetWnd() == (HWND)((LPARAM)ep))
	{
		dwPos = ((raSlider*)(GetElement(PLAYER_VOL)))->GetPosition();
		// Laufstärke setzen ( 0 => Mute )
		// Es wird durch 1100.0f geteilt weil: dwpos=1000 / 1000 = 1.97... ergibt
		chGroup->setVolume((float)(dwPos * 0.9) / 1100.0f);
	}
	// Ist es der Slieder zur angebe der Position?
	else if(((raElapsedSlider*)(GetElement(PLAYER_SLIDER)))->GetHandle()->GetWnd() == (HWND)((LPARAM)ep))
	{
		if(!rapStream::Instance().Stream->IsPlaying) return;	// Wird was abgespielt?? nein: return;

		DWORD dwPosition  = ((raElapsedSlider*)(GetElement(PLAYER_SLIDER)))->GetPosition();
		DWORD dwLenght = ((raElapsedSlider*)(GetElement(PLAYER_SLIDER)))->GetTimeLength();
		((raElapsedSlider*)(GetElement(PLAYER_SLIDER)))->SetTimePosition((dwPosition * dwLenght) / 1000);

		m_seekstart = false;
		rapStream::Instance().Stream->SetPosition((dwPosition * dwLenght) / 1000);

		chGroup->setVolume(0);

		if((WPARAM)ep == TB_ENDTRACK)
		{
			m_seekstart = true;
			dwPos = ((raSlider*)(GetElement(PLAYER_VOL)))->GetPosition();
			chGroup->setVolume((float)(dwPos * 0.9) / 1100.0f);
		}
	}
}

void raMainWindow::OnTimer(Event e, EventParam ep)
{
	if((WPARAM)ep != PLAYER_TIMER) return;

	if(!rapStream::Instance().Stream->IsPlaying)
	{
		KillTimer(Wnd, PLAYER_TIMER);
	}
	if(m_seekstart)
	{
		int ms = rapStream::Instance().GetStreamPosition();
		((raElapsedSlider*)(GetElement(PLAYER_SLIDER)))->SetTimePosition(ms);

		if(rapStream::Instance().GetStreamLength() == -1) return;

		if(rapStream::Instance().GetStreamPosition() >=
			((raElapsedSlider*)(GetElement(PLAYER_SLIDER)))->GetTimeLength() - 600)
		{
			OnVor();
		}
	}
}
long raMainWindow::OnDrop(Event e, EventParam ep)
{
	UINT  uNumFiles;
	char szNextFile [MAX_PATH];
	HDROP hDrop;

    hDrop = (HDROP)ep.swParam; // HDROP aus wParam erhalten
    uNumFiles = DragQueryFile ( hDrop, -1, NULL, 0 );		// Wie viele Datein werden gedropt speichern in uNumFiles
	int uFile = 0;
	for (uFile = 0; uFile < uNumFiles; uFile++ )	// alle Datein die gedropt werden sollen durch gehen
	{
		if ( DragQueryFile ( hDrop, uFile, szNextFile, MAX_PATH ) > 0 )	// Ist die größe der Datei gröer als 0 Byte
		{
			if(Util::raFile::raGetFilenameExtension(szNextFile) == "m3u")	// Ist es eine m3u Datei (Playlist)
			{
				IPlayliste* playliste = new raPlayerPlayliste();			// playlist object erstellen
				playliste->Create();										// Playlist erstellen
				playliste->Open(szNextFile);								// Playlist öffnen (gedropte datei)
				playliste->WriteItemsInListBox(m_playlist);					// Playlist in spieler Playlist schreiben
				SAFE_DELETE(playliste);										// Playlist object zerstören

				continue;	// next
			}
			else if(strcmp(Util::raFile::raRemoveDir(szNextFile).c_str(), "desktop.ini"))	// Alle ausser desktop.ini
				AddFiles(szNextFile); // Datei hinzufügen
		}
	}
    DragFinish(hDrop);

	return 0;
}
void raMainWindow::AddFiles(raString str)
{
	HANDLE fHandle;
	WIN32_FIND_DATA wfd;

	fHandle=FindFirstFile(str.c_str(),&wfd);
	do
	{
		// Eintrag nur behandeln, wenn es nicht . oder .. ist (werden nur bei Unterverzeichnissen mit zurückgeliefert)
		 // hier könnte man z.B. auch mit lstrcmp auf . und .. vergleichen, was allerdings nicht ganz so effizient ist
		 if (!( (wfd.cFileName[0]=='.') && ( (wfd.cFileName[1]=='.' && wfd.cFileName[2]==0) || wfd.cFileName[1]==0 ) ))
		 {
			 // Ist die Datei kein Ordner?
			if (!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
#ifdef _DEBUG
				std::cout << str << "\n";
#endif
				// m_playlist->AddEntry -> Einfach ein String in die Liste hinzufügen; Gibt den Index des neuen Items zurück
				// m_playlist->SetData<raString> -> erwartet in den < > Klammern was für ein Typ Daten hinzugefügt werden sollen
				// weiter erwartet die Funktion den Index und die Daten Index in UINT und die Daten als T =  { Inhalt <> Klammern }
				m_playlist->SetData<raString>(m_playlist->AddEntry(wfd.cFileName), str);
			}
		}
	}
	// Solange weier machen bis keine Daten mehr gefunden werden
	while (FindNextFile(fHandle,&wfd));
	FindClose(fHandle);
}