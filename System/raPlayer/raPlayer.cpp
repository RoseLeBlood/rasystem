#include "raPlayer.h"

// Einstiegspunkt
void raMain(System::raCmdParser *cmdParser)
{
	System::raConsole::Close();

	RINFO("Ramona Audio Player RAMP 1.6-Leica\n");
	// GUI Manager (Singleton Klasse) Funktion Create Aufrufen um SIe zu erstellen
	// Param 0: Die HINSTANCE aus der Applikation Klasse auch Singleton
	raGuiManager::Instance().Create(new raInstance( raApplication::Instance().GetInstance()) );

	// Es ist kein Spiel, verwende nicht Peek
	raApplication::Instance().IsGame(false);
	// Style setzen
	raApplication::Instance().SetVisualStyle();
	// Setzen der Programm Klasse
	raPlayerPrg *prg = new raPlayerPrg();

	raApplication::Instance().SetProgramm(prg);
}

namespace tools
{
	raString RemoveBlanks(raString input)
	{
		for(size_t i=input.length()-2; i>0 ;i--) // String von hinten durch gehen von 2. letzer steller
		{
			if((input.at(i)==' ') && (input.at(i-1) ==' ') )	// Ist die Stelle ein ' ' und die Dannach auch?
				input.erase((size_t)i, (size_t)1);				// Dann lösche diese Stelle
		}
		return input;											// Rückgabe des Veränderten strings
	}
}