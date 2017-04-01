#pragma once

//RSAPI
namespace Sound
{
	enum RACHANNELMODE
	{
		RA_SOUND_MONO,
		RA_SOUND_STEREO,
		RA_SOUND_QUAD,
		RA_SOUND_SURROUND,
		RA_SOUND_51,
		RA_SOUND_71,
		RA_SOUND_PROLOGIC,
		RA_SOUND_MYEARS
	};
	enum RASOUNDFORMAT
	{
		RA_PCM16_44100,
		RA_PCM16_48000,
		RA_PCM16_96000,

		RA_PCM24_44100,
		RA_PCM24_48000,
		RA_PCM24_96000,
		RA_PCM24_192000,

		RA_PCM32_44100,
		RA_PCM32_48000,
		RA_PCM32_96000,
		RA_PCM32_192000,

		RA_PCMFLOAT_44100,
		RA_PCMFLOAT_48000,
		RA_PCMFLOAT_96000,
		RA_PCMFLOAT_192000
	};
	enum RASOUNDAUSGABE
	{
		RA_WINMM,
		RA_DSOUND,
		RA_WASAPI,
		RA_ASIO
	};
	#define DISTANCEFACTOR 1.0f
	class RSAPI raSystem
	{
	public:
		raSystem(void);
		~raSystem(void);

		virtual bool Create(raSmartPointer<raCamera> Cam, raConfig* Config);
		virtual bool Create(raSmartPointer<raCamera> Cam, int Soundkarte = 0,  RACHANNELMODE Mode = RA_SOUND_STEREO,
							RASOUNDFORMAT PCMFORMAT = RA_PCM16_44100, RASOUNDAUSGABE ausgabe = RA_WINMM);
		virtual void Destroy();

		virtual bool Update();

		bool ERRCHECK(FMOD_RESULT result, char* Func = "");

		READONLY_PROPERTY(FMOD::System*, FSystem);
		GET(FSystem)
		{
			return m_system;
		}
		PROPERTY(unsigned int, Buffer);
		GET(Buffer) { unsigned int buf = 0; m_system->getStreamBufferSize(&buf, NULL); return buf; }
		SET(Buffer) { FMOD_RESULT result = m_system->setStreamBufferSize(value, FMOD_TIMEUNIT_RAWBYTES); ERRCHECK (result);}
	protected:
		RACHANNELMODE GetChannelFromConfig(int Channels);
		RASOUNDFORMAT GetFormatFromConfig(int Bit, int Samplerate);
		RASOUNDAUSGABE GetDriverFromConfig(int Driver);

	private:
		raSmartPointer<raCamera>    m_cam;
		raSystem::raCriticalSection m_sec;
		FMOD::System*				m_system;
	};
}