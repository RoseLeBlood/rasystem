#include "Game.h"

using namespace System;

void raMain(System::raCmdParser *cmdParser)
{
	raSmartPointer<raPong>(new raPong())->Run();
}