//#include <windows.h>

#include "..\raSystem\include\raMain.h"
#include "raApplication.h"

#include <new.h>

using namespace System;

int NewHandler (size_t size)
{
	RERROR ( "Out of memory" );
	MessageBox(NULL, "Error: OUTOFMEM\n Anwendung schlie�t sicht nun", "NewHandler", MB_OK);
	raApplication::Instance().Exit();

	return 0;
}

// Normaler einstiegspunkt
int main( int argc, char **argv )
{
	// ladet alle wichtigen Komponetten f�r die Engine bzw. mittlerweile System
	System::raLoadEngine();

	// Setzt die Start Komponenten also �bergibt an die raApplication Klasse die HINSTANCE
	raApplication::Instance().SetStartUpParameter(GetModuleHandle (0)) ;

	// EIn neuen new handler f�r Speicher Kritische Software ( empfohlen )
	_set_new_handler (&NewHandler);

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	raArray<raString> *m_args = new raArray<raString>();
	for(int i = 1; i < argc; i++)
	{
		m_args->Add(argv[i]);
	}
	raCmdParser *cmdparser = new raCmdParser();
	cmdparser->CommandLine = m_args;
	cmdparser->DefaultCommand = new raBasicArg();
	cmdparser->ErrorCommand = new raErrorArg();

	// Eigener Einstiegspunkt f�r die Software die diese Libery verwendet
	raMain(cmdparser);

	return raApplication::Instance().RunApp();
}