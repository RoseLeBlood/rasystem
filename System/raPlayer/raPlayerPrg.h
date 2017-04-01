#pragma once

// Main Klasse des Programmes. Von hier aus werden die Fenster erstellt bzw. regestriert
class raPlayerPrg : public IRAProgramm
{
public:
	raPlayerPrg();
	~raPlayerPrg(void);

	virtual bool Create();	// Wird beim erstellen des Programmes aufgerufen
	virtual bool Idle();	// Für spiele sehr interresant -> wird ausgeführt wenn keine Nachrichten vorliegen
	virtual void Destroy();	// Wird aufgerufen wenn das Programm "zerstört wird"


private:
	HINSTANCE						m_instance;		// hinstance (mutex) des Programmes

};