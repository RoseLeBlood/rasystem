#pragma once

#define PLAYER_WINDOW		21
#define PLAYER_PLAY			100
#define PLAYER_STOP			23
#define PLAYER_PAUSE		24
#define PLAYER_LIST			25
#define PLAYER_SLIDER		26
#define PLAYER_TIMER		27
#define PLAYER_VOR			29
#define PLAYER_ZURUCK		30
#define PLAYER_VOL			31

#define MENU_DATEI_OPEM		601
#define MENU_DATEI_LOAD		602
#define MENU_DATEI_SAVE		603
#define MENU_DATEI_EXIT		604

#define MENU_CONTROLL_VISO	699
#define MENU_CONTROLL_PLAY	PLAYER_PLAY
#define MENU_CONTROLL_STOP	PLAYER_STOP
#define MENU_CONTROLL_PAUSE	PLAYER_PAUSE
#define MENU_CONTROLL_VOR	PLAYER_VOR
#define MENU_CONTROLL_PREV	PLAYER_ZURUCK
#define MENU_CONTROLL_PLAYLISTCLEAR 700
#define MENU_CONTROLL_SET	701

#define MENU_ABOUT_WEBSITE	801
#define MENU_ABOUT_INFO		802

// Das Controll Fenster aka Main Window ist ein Nicht größen änderbares Fenster
// Abgeleitet von der Klasse System::GUI::raFixedWindow -> Fixed Window ist die Basis aller nnicht gröen
// änderbaren Fenster
class raMainWindow : public raFixedWindow
{
public:
	raMainWindow(raInstance* Inst);
	~raMainWindow(void);

	// Überschreiben der Funktion Create dient zur erstellen des Fensters und die dazugehörigen
	// Elemente WM_CREATE ist nicht erreichbar wegen Sub Window Proc
	virtual bool Create (DWORD dwSytle,char *pTitle,
						 raWindowClass *WindowClass,
						raVector2 Groeße,
						 unsigned long ID);

	//Get Die Playliste
	READONLY_PROPERTY(System::GUI::raList*, Playliste);
	GET(Playliste)
	{
		return m_playlist;
	}
	//System::GUI::raList*	GetPlayListe()			{ Return m_playlist; }

	// Überschreiben der Message Funktion
	virtual long OnMessage(Event e, EventParam ep)
	{
		switch(e.smsg)
		{
		case WM_DESTROY:
			OnDestroy(e,ep);
			break;
		case WM_QUIT:
			OnQuit(e,ep);
			break;
		case WM_PAINT:			// zum neuzeichen
			OnPaint(e, ep);
			break;
		case WM_COMMAND:		// Ein Element gedrückt
			OnCommand(e, ep);
			break;
		case WM_TIMER:
			OnTimer(e, ep);		// aufruf des Timers -> wenn Zeit abgelaufen ist
			break;
		case WM_DROPFILES:		// Beim drop einer Datei in die Liste
			OnDrop(e, ep);
			break;
		case WM_HSCROLL:
		case WM_VSCROLL:
				OnHScroll(e, ep);	// Slider scroll
			break;
		case WM_VKEYTOITEM:
			OnVKeyToItem(e, ep);
			break;
		};
		//UpdateWindow(this->GetWnd());
		return raFixedWindow::OnMessage(e, ep);
	}
	virtual void Idle()
	{
		rapStream::Instance().Idle();
	}
protected:
	// Die Dazugehörigen Funktionen der Messages funktion
	void OnDestroy(Event e, EventParam ep);
	void OnQuit(Event e, EventParam ep);
	void OnPaint(Event e, EventParam ep);
	void OnCommand(Event e, EventParam ep);
	void OnTimer(Event e, EventParam ep);
	long OnDrop(Event e, EventParam ep);
	void OnHScroll(Event e, EventParam ep);
	void OnVKeyToItem(Event e, EventParam ep);
	void OnOpen();
	void OnLoadPlaylist();
	void OnSavePlaylist();
	void OnMenuInfo();

	// Helper für OnHScroll -> fügt nur Datein aus Ordnern und unter verzeichnisse hinzu
	void AddFiles(raString str);

	// WM_COMMAND funktionen
	void OnPlay(bool ex = false);
	void OnStop();
	void OnPause();
	void OnVor();
	void OnZurück();
	void OnList(EventParam ep);

	void PlaylistStart();
	void PlaylistClose();
private:
	raBitmapButton*					Play,*Pause,*Stop,*vor, *zurück;	// Die einzelen Tasten
	System::GUI::raList*					m_playlist;							// Die Playliste
	raSlider*					volume;
	raElapsedSlider*					Slider;								// Der Slieder
	HINSTANCE						m_instance;							// hInstance des PProgrammes
	unsigned int					m_playitem;							// Das gerade gespielte Item
	bool							m_ifseek;
	DWORD							m_seekstart;
};