#pragma once
#pragma pointers_to_members( full_generality, virtual_inheritance )

#include "Propertie.h"
#include "IRAProgramm.h"
#include "raConsole.h"
#include "raException.h"
#include <GdiPlus.h>
#include "raArg.h"
#include "raErrorArg.h"
#include "raBasicArg.h"

using namespace Gdiplus;

//gdiplus.lib
#pragma comment( lib, "gdiplus.lib" )

// Comctl32.lib
#pragma comment( lib, "Comctl32.lib" )

//Die Application Klasse der dreh und angel punkt eines raApplications Programmes
class raApplication
{
	friend int main( int argc, char **argv );
public:
	~raApplication(void);

	void			SetVisualStyle();
	void			IsGame(bool game)					{ m_peek = game; }
	void			SetProgramm(IRAProgramm* prg)
	{
		m_prg = prg;
	}
	IRAProgramm*	GetProgramm()						{ return m_prg; }
	HINSTANCE		GetInstance()						{ return m_inst; }

	void			SetStartUpParameter(HINSTANCE hInstance);

	void			Exit()		{ m_while = false; }

	static raApplication& Instance();
protected:
	int				RunApp();
private:
	static raApplication *m_instance;
	static void CreateInstance();
	static void UnInstance();
private:
	int WindowLoop(bool Peek);
private:
	IRAProgramm								*m_prg;
	HINSTANCE								m_inst;
	bool									m_while;
	bool									m_peek;
private:
	raApplication(void);
};

	//--------------------------------------------------------------
	extern void raMain(System::raCmdParser *cmdParser);