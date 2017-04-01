#include "raPlayer.h"
#include "Resource.h"

raPlayerPrg::raPlayerPrg() : IRAProgramm()
{
	// Hier darf nichts passieren -> speichermanager
}

raPlayerPrg::~raPlayerPrg(void)
{
	// Hier darf nichts passieren -> speichermanager
}
bool raPlayerPrg::Create()
{
	// Instance für die Klasse raWindowClass erstellen
	raSmartPointer<raWindowClass> wndClass = raSmartPointer<raWindowClass>(new raWindowClass("raPTRClass"));

	// und die Fenster klasse regestrieren
	wndClass->hIcon = LoadIcon(raApplication::Instance().GetInstance(), MAKEINTRESOURCE(raQuad));
	wndClass->hIconSm = LoadIcon(raApplication::Instance().GetInstance(), MAKEINTRESOURCE(raQuad));
	wndClass->Register(raApplication::Instance().GetInstance());

	rapStream::Instance().Create();

	// Fenster hinzufügen

	raGuiManager::Instance().AddWindow<raVIS>(0, " ", wndClass.get(), raVector2(500,300), 200);						// Fenster der Visualisierung
	raGuiManager::Instance().AddWindow<raMainWindow>(0, RAPLAYER_VERSION_STRING, wndClass.get(), raVector2(500,180),PLAYER_WINDOW);	// Controll Fenster
	raGuiManager::Instance().AddWindow<raInfoWindow>(0, "", wndClass.get(), 0.0f, PLAYER_INFO_WINDOW);

	return true;
}
void raPlayerPrg::Destroy()
{
	return;
}
bool raPlayerPrg::Idle()
{
	//rapStream::Instance().Idle();
	return true;
}