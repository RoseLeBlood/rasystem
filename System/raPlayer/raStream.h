#pragma once

// Diese Klasse kümmert sich um die Wiedergabe einer Datei bzw. eines Streams
class rapStream : public IRASingleton<rapStream>
{
	friend class raMainWindow;		// friend classen Controll Window,
	friend class raVISThread;		// der Thread für die Visulation
	friend class PlayerServer;

	friend class IRASingleton<rapStream>;

public:
	~rapStream(void);

	bool Create();
	bool Idle();
	void Destroy();

	int GetStreamLength();
	int GetStreamPosition();
	bool SetStreamPosition(unsigned int pos);

	raSmartPointer<IRATag> GetTag(raString Name, int Index)			{ return m_str->GetTag(Name, Index); }
protected:
	rapStream() : IRASingleton() { }

	bool PlayStream(raString Stream);
	void StopStream();
	void PauseStream();

	READONLY_PROPERTY(raSmartPointer<Sound::raSystem>, System);
	GET(System)
	{
		return m_sys;
	}
	READONLY_PROPERTY(raSmartPointer<raStream>, Stream);
	GET(Stream)
	{
		return m_str;
	}
private:
	raSmartPointer<Sound::raSystem>		m_sys;
	raSmartPointer<raStream>		m_str;
	raSmartPointer<raCamera>		m_cam;
	raTimer							m_timer;
};