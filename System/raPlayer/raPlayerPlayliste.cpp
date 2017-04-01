#include "raPlayer.h"

using namespace std;

raPlayerPlayliste::raPlayerPlayliste(void)
{
}


raPlayerPlayliste::~raPlayerPlayliste(void)
{
}
// �ffnen einer Playlist
bool raPlayerPlayliste::Open(raString FileName)
{
	char line[256];
	ifstream infile (FileName, ios::in);	// Datei FileName �ffnen
  
	if (!infile) return false;				// ge�ffnet? nein: Dann ByeBye
    
	// Solange bis alle Zeilen der Datei durch gegangen sind
	while (infile.getline (line, 256))	
	{
		if(line[0] == '#')		// fengt die Zeile mit ein '#' an?
		{
			// Ja:
			// zwischen Speichern der Summary und deklaration + Zuweisung der Variable Title
			raString Summary = raString(line), Title = "";

			// N�chste Zeile bekommen?
			if(infile.getline (line, 256))
			{
				// Ja:
				if(line[0] != '#')									// f�ngt die Zeile nicht mit ein '#' an?
				{
					Title = line;									// Title zwischen speichern

					Summary = Summary.erase(0,1);					// Aus dem Summary String die '#' l�schen			
					this->AddItem(PlaylistItem(Title, Summary));	// Item zur PLaylist hinzuf�gen
				}
			}
		}
	}
	infile.close();	// Inifile schlie�en

	return true;
}
// Speichern der Playlist in eine Datei
bool raPlayerPlayliste::Save(raString Filename)
{
	fstream file_op;
	raString summary, title;

	try																		// versuche
	{
		file_op.open(Filename, ios::out);									// Datei zum schreiben �ffnen

		for(int i = 0; i < m_items->GetSize(); i++)							// Solange bis alle Items durchgegangen sind
		{
			if(m_items->GetAt(i).m_Summary.c_str()[0] != '#')				// Hat die Summary kein '#' zeichen		
				summary = "#" +  m_items->GetAt(i).m_Summary + "\n";		// Dann eins hinzuf�gen und ein "\n" zecihen
			else
				summary = m_items->GetAt(i).m_Summary + "\n";				// Wenn nicht dann nur '\n' zecihen

			title = m_items->GetAt(i).m_Item + "\n";						// Dem Titel string auch ein "\n" hinzuf�gen

			//Summary
			file_op.write(summary.c_str(), summary.length());
			//Path
			file_op.write(title.c_str(), title.length());

		}
		file_op.close();													// Schlie�en der Datei

	}
	catch(exception e)														// Bei ein Fehler
	{
		RERROR(e.what());													// Fehler in log datei ausgeben
		return false;														// return false
	}
 
	return true;															// alles ok return true
}

