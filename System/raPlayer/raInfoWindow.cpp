#include "raPlayer.h"

#define COPYRIGHT "Audio Engine: FMOD Sound System, Copyright © Firelight Technologies Pty, Ltd., 1994-2011\nGrafik Engine: raSystem, Copyright Philipp Schröck, 2012 GUI: raGUI, Copyright Philipp Schröck, 2012. philsch@unitybox.de "

raInfoWindow::raInfoWindow(System::GUI::raInstance* Inst) : System::GUI::raFixedWindow(Inst)
{
}
raInfoWindow::~raInfoWindow()
{
}
using namespace std;

bool raInfoWindow::Create (DWORD dwSytle, char *,
						 raWindowClass *WindowClass,
						 System::raVector2,
						 unsigned long ID)
{
	m_WindowClass = WindowClass;
	m_ID = ID;

	// Erstellen des Fensters
	if(!raFixedWindow::Create(dwSytle, RAPLAYER_VERSION_STRING, WindowClass, PLAYER_INFO_SIZE, ID))
			return false; // Beim Fehler return false

	m_pLblCoppyright = AddLabel(COPYRIGHT, VECTOR2_2F(10, 265), VECTOR2_2F(500.0f, 25.0f), PLAYER_INFO_FMODLABEL);
	m_pLblCoppyright->SetFont(10, "Calibri Standard",false, false, false, false);

	ifstream in("License.txt");
	in.seekg (0, ios::end);
	unsigned int length = in.tellg();
	in.seekg (0, ios::beg);

	char* buffer = new char [length];
	in.read (buffer,length);
	in.close();

	m_pTextBox = AddRichTextBox(buffer, VECTOR2_2F(10, 10), VECTOR2_2F(480.0f, 250.0f), 9003, true, true, false);

	m_hide = false;
	HideMe();

	return true;
}