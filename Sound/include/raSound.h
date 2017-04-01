#ifdef RASOUND_EXPORTS
#define RSAPI __declspec(dllexport)
#include <raMain.h>
#else
#define RSAPI __declspec(dllimport)
#endif

#include "..\FMOD\inc\fmod.hpp"
#include "..\FMOD\inc\fmod_errors.h"
#include "..\FMOD\inc\fmod_dsp.h"

/*
	Project: raSound
	Use:  Non-Commercial only

	System::Sound

	Programer: Philipp Schroeck philsch@unitybox.de
*/
namespace System
{
	#include "raSystem.h"
	#include "raStream.h"
};