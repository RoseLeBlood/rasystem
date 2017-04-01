#include "raPlayer.h"

static bool canupdate = false;

rapStream::~rapStream(void)
{
}
bool rapStream::Create()
{
	m_cam = raSmartPointer<raCamera>(new raCameraFP());
	m_cam->SetViewParams(new VECTOR3_3F(0,0,-1.5f),new VECTOR3_3F(0,0,0));
	m_cam->FrameMove(1,1);

	// Sound System mit der Kamera und der Config erstellen
	// Config ist die ini aus raConfig/config.ini
	m_sys = raSmartPointer<raSystem>(new raSystem());
	{
		m_sys->Create(m_cam, &raConfigLoader::GetConfig());
		// Sound System source updaten
		m_sys->Update();
		canupdate = true;
	}
	m_str = raSmartPointer<raStream>(new raStream());
	{
		// Sound Stream erstellen
		m_str->Create(m_sys.get());
	}

	return true;
}
bool rapStream::SetStreamPosition(unsigned int pos)
{
	// Setzen der Stream Position
	return m_str->SetPosition(pos);
}
void rapStream::PauseStream()
{
	// Pausieren den Stream
	m_str->PauseStream();
}
bool rapStream::Idle()
{
	if(canupdate) m_sys->Update();
	return true;
}
void rapStream::Destroy()
{
	// Effect stoppen
	//m_normalize.Stop();
}
bool rapStream::PlayStream(raString Stream)
{
	// laden des Streams wenn ok in 3D Modus
	if(m_str->LoadStream(Stream, true))
		// stream wiedergeben
		return m_str->PlayStream();
	return false;
}
void rapStream::StopStream()
{
	m_str->StopStream();		// Stream stoppen
}
int rapStream::GetStreamLength()
{
	return m_str->GetLenght();		// Stream länge bekommen
}
int rapStream::GetStreamPosition()
{
	return m_str->GetPosition();		// Stream position bekommen
}