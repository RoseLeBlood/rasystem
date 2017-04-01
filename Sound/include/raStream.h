#pragma once

namespace Sound
{
	class RSAPI IRATag
	{
	public:
		IRATag(raString data, raString name) : m_data(data), m_name(name) { }
		~IRATag() { }

		PROPERTY(raString, Value);
		GET(Value) { return m_data; }
		SET(Value) { m_data = value; }

		PROPERTY(raString, Name);
		GET(Name) { return m_name; }
		SET(Name) { m_name = value; }
	private:
		raString	m_data;
		raString	m_name;
	};
	class RSAPI raStream
	{
	public:
		raStream(void);
		~raStream(void);

		virtual bool Create(raSmartPointer<Sound::raSystem> AudioSystem);
		virtual void Destroy();

		virtual void Set3DMinMaxDistance(float min, float max);
		virtual bool SetVelocity(raVector3& vec);
		virtual bool Set3DPosition(raVector3& vec);

		virtual int	 GetPosition(void);
		virtual bool SetPosition(unsigned int pos);
		virtual int  GetLenght(void);

		virtual bool SetChannelMix(float fleft, float fright, float rleft, float rright,
			float sleft, float sright, float center, float bass);

		virtual void SetVolume(float vol);

		virtual bool LoadStream(raString FileName, bool b3D = false);
		virtual bool CloseStream();
		virtual bool PlayStream(raVector3 Position = 0.0f, float minDistance = 1.0f,
			float maxDistance = 100.0f, raVector3 Velocity = 0.0f);
		virtual bool PauseStream();
		virtual bool StopStream();

		virtual raSmartPointer<IRATag> GetTag(raString Name, int Index);

		READONLY_PROPERTY(FMOD::Sound*, Sound);
		GET(Sound) { return m_sound; }

		READONLY_PROPERTY(FMOD::Channel*, Channel);
		GET(Channel) { return m_channel; }

		PROPERTY(raSmartPointer<Sound::raSystem>, System);
		GET(System) { return m_system; }

		READONLY_PROPERTY(bool, IsPlaying);
		GET(IsPlaying) { if(!m_channel) return false; bool playing = false; m_channel->isPlaying(&playing); return playing;  }

		PROPERTY(unsigned int, Position);
		GET(Position) { return GetPosition(); }
		SET(Position) { SetPosition(value); }

		WRITEONLY_PROPERTY(raVector3, Position3D);
		SET(Position3D) { Set3DPosition(raVector3(value)); }
	protected:
		bool ERRCHECK(FMOD_RESULT result, char* Func = "");
	protected:
		FMOD::Sound							*m_sound;
		FMOD::Channel						*m_channel;
		raSmartPointer<Sound::raSystem>			m_system;
	};
}