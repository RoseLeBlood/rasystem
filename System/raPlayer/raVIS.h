#pragma once
class raVIS;

// Der Thread für die Visualation vererbung aus der Basis Thread Klasse aus raSystem
class raVISThread : public System::raThread
{
public:
	raVISThread();
	~raVISThread();

	void Ende()		{ m_run = false; }

	PROPERTY(std::wstring, Text);
	GET(Text)
	{
		return m_anzeigetext;
	}
	SET(Text)
	{
		m_anzeigetext = value;
	}

protected:
	// Klasse für eine Linie ( Channel 1 & 2 )
	class line
	{
	public:
		// b = abweichung
		// Col = Farbe der linie
		line(int b, raColor Col)
			{ m_b = b; m_col = Col; }

		int			m_b;
		raColor		m_col;
	};
	// Array für die Linien
	raArray<raSmartPointer<line>>		m_plus;

	bool ERRCHECK(FMOD_RESULT result, char* Func)
	{
		if (result != FMOD_OK)
		{
			char    errstring[1024];

			sprintf(errstring, "FMOD (%d) %s [%s]", result, FMOD_ErrorString(result), Func);

			RERROR(errstring);
			return false;
		}
		return true;
	}
protected:
	// Überschreibung der Run Funktion -> Hier steht der Code des Threads -> das was ausgefürht werden soll
	virtual DWORD Run( LPVOID  arg  );
	// wird beim einstieg des Threads aufgerufen
	virtual void ThreadCtor();
	// Beim verlassend des Threads
	virtual void ThreadDtor();

	// Visualation zeichnen
	void drawSpectrum();

	// IDTAG in Visualation Zeichen
	void DrawIDTag();

protected:
	void Draw(int x);
private:
	raDrawing										m_draw;				// Das Draw element
	raText											m_text;				// DirectWrite object
	std::wstring									m_anzeigetext;		// Der zu anzuzeigener Text
	raSmartPointer<raVIS>							m_window;			// Das Fenster in dem gezeichnet werden soll
	bool											m_run;

	int										spectrumsize;		// spectrumsize
	float*									spectrum1;			// Die Wave Daten Channel0
	float*									spectrum2;			// Die Wave Daten Channel1

	System::raTimer		m_timer;			// der timer aus raSystem zur berechnung der FPS
	FMOD::System* system;
};

// Das eigentliche Fenster in der Der Thread ( s. o.) zeichnen soll
// Ist ein Überlappendes Fenster
class raVIS : public raOverlappedWindow
{
public:
	raVIS(raInstance* Inst);
	~raVIS(void);

	// überschreibend er Create Funktion
	virtual bool Create (DWORD dwSytle, char *pTitle,
						 raWindowClass *WindowClass,
						 raVector2 Groeße,
						 unsigned long ID);

	virtual long OnMessage(Event e, EventParam ep)
	{
		if(this->Wnd != e.shwnd)
			return 0;
		switch(e.smsg)
		{
			// So kann das Fenster nicht mehr geschlossen werden.
		case WM_DESTROY:
			break;
		case WM_QUIT:
			break;
		case WM_CLOSE:
			HideMe();
			break;
		case WM_SIZE:
			break;
		default:
			// jegliche andere Nachricht
			return raWindow::OnMessage(e, ep);
		};
		return 0;
	}
	READONLY_PROPERTY(raVISThread*, VISThread);
	GET(VISThread)
	{
		return &m_thread;
	}
	void HideMe()
	{
		ShowWindow((!m_hide) ? SW_HIDE : SW_SHOW);
		m_hide = !m_hide;
	}
private:
	raVISThread				m_thread;		// Die Thread Klasse siehe oben
	raVector2				m_Groeße;		// Speichern der Größe
	bool					m_hide;
};