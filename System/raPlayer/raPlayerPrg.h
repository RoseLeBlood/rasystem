#pragma once

// Main Klasse des Programmes. Von hier aus werden die Fenster erstellt bzw. regestriert
class raPlayerPrg : public IRAProgramm
{
public:
	raPlayerPrg();
	~raPlayerPrg(void);

	virtual bool Create();	// Wird beim erstellen des Programmes aufgerufen
	virtual bool Idle();	// F�r spiele sehr interresant -> wird ausgef�hrt wenn keine Nachrichten vorliegen
	virtual void Destroy();	// Wird aufgerufen wenn das Programm "zerst�rt wird"


private:
	HINSTANCE						m_instance;		// hinstance (mutex) des Programmes

};