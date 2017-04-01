#include "raPlayer.h"

raText g_text;
std::wstring m_text;

#define GRAPHICWINDOW_HEIGHT  100		// Draw Bereich hoehe

raVISThread::raVISThread() : raThread()
{
}
raVISThread::~raVISThread()
{
}
void raVISThread::ThreadCtor()
{
	m_run = true;

	m_text.Create(&m_draw, L"Times New Roman", 12, 0);
	m_text.Color = COLOR_XRGB(0,255,125);
	m_text.Opacity = 0.5f;
	system = NULL;
}
void raVISThread::ThreadDtor()
{
	SAFE_MEMFREE(spectrum1);
	SAFE_MEMFREE(spectrum2);
	m_plus.RemoveAll();
}

DWORD raVISThread::Run( LPVOID  arg  )
{
	if(arg)			// Ist die Variabele arg nor null
	{
		// Setzen der Variablen
		m_window = raSmartPointer<raVIS>((raVIS*)arg);

		spectrumsize = m_window->GetWindowSize().x;

		// Speicher alloc
		// In der Variable spectrum werden die Spectrum Daten gespeichert
		spectrum1 = new float[spectrumsize];
		spectrum2 = new float[spectrumsize];

		m_draw.Create( new System::GUI::raWND(m_window->Wnd), 22);				// Das draw (direct2d) element erstellen

		// Stereo VIS Channel:0, 1
		m_plus.Add(( raSmartPointer<line>(new line(20, COLOR_XRGB(0,0,255))))); // Channel 0
		m_plus.Add(( raSmartPointer<line>(new line(180, COLOR_XRGB(255,0,0))))); // Channel 1

		raCriticalSection m_cs;
		m_cs.Create();

		system = rapStream::Instance().System->FSystem;
		{

				while(m_run)												// Sollange run = true ist ...
				{
					if(!::IsIconic(m_window->Wnd))
					{
						m_draw.Begin(COLOR_XRGB(0,0,0));					// Zeichen beginnen backbuffer löschen
							drawSpectrum();									// Visualatioen zeichen

							DrawIDTag();

							m_text.RenderText((WCHAR*)m_anzeigetext.c_str(), VECTOR2_2F(2,0), VECTOR2_2F(spectrumsize,20),
								TA_LEFT, 2);

						m_draw.End();										// Backbuffer anzeigen
					}
					::Sleep(1);
				}
		}
	}
	return 0;
}
void raVISThread::DrawIDTag()
{
	raSmartPointer<IRATag> ArtistTag, AlbumTag, TitleTag;
	raString Title = "";

	ArtistTag = rapStream::Instance().GetTag("ARTIST", -1);
	AlbumTag = rapStream::Instance().GetTag("ALBUM", -1);
	TitleTag = rapStream::Instance().GetTag("TITLE", -1);

	if(ArtistTag.Data)
	{
		if(ArtistTag->Value != "") Title += " " + ArtistTag->Value;
	}
	if(TitleTag.Data)
	{
		if(TitleTag->Value != "") Title += "::" + TitleTag->Value;
	}
	if(AlbumTag.Data)
	{
		if(AlbumTag->Value != "") Title += "::" + AlbumTag->Value;
	}

	if(Title != "")
	{
		Title = tools::RemoveBlanks(Title);
		m_anzeigetext = std::wstring(Title.begin(), Title.end());
	}
}

void raVISThread::Draw(int x)
{
	float y1, y2;
	float ynext1, ynext2;

	if(m_plus.GetSize() != 2) this->Stop(true);

	// Channel1 && Channel2

	{
		// Brechnung des 1. Punktes (y Koordinate) startpunkt der Linie
		y1 = (spectrum1[x] + 1) / 2.0f * GRAPHICWINDOW_HEIGHT;
		// Berechnung des 2. Punktes (y Koordinate) endpunkt der Linie
		ynext1 = (spectrum1[x+1] + 1) / 2.0f * GRAPHICWINDOW_HEIGHT;

		// Brechnung des 1. Punktes (y Koordinate) startpunkt der Linie
		y2 = (spectrum2[x] + 1) / 2.0f * GRAPHICWINDOW_HEIGHT;
		// Berechnung des 2 Punktes (y Koordinate) endpunkt der Linie
		ynext2 = (spectrum2[x+1] + 1) / 2.0f * GRAPHICWINDOW_HEIGHT;

		// Linie 1 Zeichen y1 bzw y1next
		m_draw.DrawLine(VECTOR2_2F(x+1, y1+m_plus[0]->m_b),				// Startpunkt der Linie
						VECTOR2_2F(x+2, ynext1+m_plus[0]->m_b),			// Endpunkt der Linie
						m_plus[0]->m_col);								// Farbe der Linie

		m_draw.DrawLine(VECTOR2_2F(x+1, y2+m_plus[1]->m_b),
						VECTOR2_2F(x+2, ynext2+m_plus[1]->m_b),
						m_plus[1]->m_col);
	}
}

void raVISThread::drawSpectrum()
{
	int count2  = 0;

	{
		FMOD_RESULT result = system->getWaveData(spectrum1, spectrumsize, 0);		// Wave Daten bekommen Channel0
		if(!ERRCHECK(result, "getWaveData 0")) return;

		result =  system->getWaveData(spectrum2, spectrumsize, 1);		// Wave Daten bekommen Channel1
		if(!ERRCHECK(result, "getWaveData 1")) return;

		for (count2 = 0; count2 < spectrumsize-1; count2++)	// Zeichnen aller Punkte
		{
				Draw(count2);
		}
	}
}

//-------------------------------------------------
raVIS::raVIS(System::GUI::raInstance* Inst) : System::GUI::raOverlappedWindow(Inst)
{
}

raVIS::~raVIS(void)
{
}
bool raVIS::Create (DWORD dwSytle,char *pTitle, raWindowClass *WindowClass,  System::raVector2 Groeße,
					unsigned long ID)
{
	m_WindowClass = WindowClass;
	m_Groeße = Groeße;
	m_ID = ID;

	// Erstellen des Fensters
	if(!raOverlappedWindow::Create(dwSytle, pTitle, WindowClass, Groeße, ID))
			return false; // Beim Fehler return false

	ShowWindow(SW_NORMAL);	// Show Fenster normal
	::Sleep(100);			// Warte 100 ms bis das Fenster auch wirklich sichtbar ist
	m_thread.Start(this);	// nun den Thread starten

	return true;
}