#include "..\raGUI\raGuiMain.h"
#include "..\raApplication\raApplication.h"
#include "..\..\Sound\include\raSound.h"

using namespace System;			// Namespace der Basis (Engine und mehr)
using namespace System::GUI;	// Namespace der GUI
using namespace System::Sound;	// Namespace der Sound Engine

#define RAPLAYER_VERSION_STRING "RAMP 1.6-Leica"

#include "IPlayliste.h"
#include "raPlayerPlayliste.h"
#include "raStream.h"
#include "raVIS.h"
#include "raMainWindow.h"
#include "raInfoWindow.h"
#include "raPlayerPrg.h"
#include "OpenFileArgument.h"
#include "ViewVIS.h"
#include "ShowConsole.h"

namespace tools
{
	// Remove überschüzige Blanks aus string. Aus "HALLO      DU DA" wird "HALLO DU DA"
	extern  raString RemoveBlanks(raString input);
}