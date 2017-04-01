#include "..\include\raMain.h"

namespace raSystem
{
	bool ERRCHECK(FMOD_RESULT result)
	{
		if (result != FMOD_OK)
		{
			char    errstring[1024];

			sprintf(errstring, "FMOD (%d) %s", result, FMOD_ErrorString(result));

			RERROR(errstring);
			return false;
		}
		return true;
	}
	raFMODSystem::raFMODSystem(raCamera *Cam) : raSoundDriver(Cam)
	{
		m_system = NULL;
		m_Created = false;
		m_format = FMOD_SOUND_FORMAT_PCM16;
		m_Config = NULL;
		
	}

	bool raFMODSystem::Create(raConfig* altConfig, int toUseMemory)
	{
		unsigned int version = 0;
		FMOD_RESULT result;
	#ifdef _WIN64
		INT_PTR retval = 0;
	#else
		int retval = 0;
	#endif

		if(altConfig)
		{
			RINFO("Verwende alternative Sound Konfiguration");
			m_Config = altConfig;
		}
		else
		{
			m_Config = new raConfig();
			m_Config->Sound.Bit = raConfigLoader::Instance().GetConfig().Sound.Bit;
			m_Config->Sound.b3D = raConfigLoader::Instance().GetConfig().Sound.b3D;
			m_Config->Sound.Channels = raConfigLoader::Instance().GetConfig().Sound.Channels;
			m_Config->Sound.Driver = raConfigLoader::Instance().GetConfig().Sound.Driver;
			m_Config->Sound.Hardware = raConfigLoader::Instance().GetConfig().Sound.Hardware;
			m_Config->Sound.SampleRate = raConfigLoader::Instance().GetConfig().Sound.SampleRate;
			m_Config->Sound.Soundkarte = raConfigLoader::Instance().GetConfig().Sound.Soundkarte;
		}
		/*
			Initialise FMOD
		*/
		result = FMOD::Memory_Initialize(raSystem::raMemManager::Instance().MemAlloc(toUseMemory), toUseMemory, 0, 0, 0);
		if(!ERRCHECK(result)) return false;

		result = FMOD::System_Create(&m_system);
		if(!ERRCHECK(result)) return false;

		result = m_system->getVersion(&version);
		if(!ERRCHECK(result)) return false;

		if (version < FMOD_VERSION)
		{
			MessageBox(GetForegroundWindow(), "INCORRECT DLL VERSION!!", "FMOD ERROR", MB_OK);
		}
	#ifdef _WIN64
		result = m_system->setPluginPath("raPlugins/x64");
	#else
		result = m_system->setPluginPath("raPlugins/x86");
	#endif    
		if(!ERRCHECK(result)) return false;

		switch (m_Config->Sound.Driver)
		{
			case 0: 
			{
				m_system->setOutput(FMOD_OUTPUTTYPE_ASIO);
				RINFO("SetOutput: ASIO");
				break;
			}
			case 1: 
			{
				m_system->setOutput(FMOD_OUTPUTTYPE_WINMM);
				RINFO("SetOutput: WinMM");
				break;
			}
			case 2: 
			{
				m_system->setOutput(FMOD_OUTPUTTYPE_DSOUND);
				RINFO("SetOutput: DSOUND");
				break;
			}
			case 3:
			{
				m_system->setOutput(FMOD_OUTPUTTYPE_WASAPI);
				RINFO("SetOutput: WASAPI");
				break;
			}
		}
	

		// FMOD PCM Format wählen
		switch(m_Config->Sound.Bit)
		{
		case 0:
			m_format = FMOD_SOUND_FORMAT_PCMFLOAT;
			RINFO("SetSoundFormat: PCM_FLOAT");
			break;
		case 16:
			m_format = FMOD_SOUND_FORMAT_PCM16;
			RINFO("SetSoundFormat: PCM_16");
			break;
		case 24:
			m_format = FMOD_SOUND_FORMAT_PCM24;
			RINFO("SetSoundFormat: PCM_24");
			break;
		case 32:
			m_format = FMOD_SOUND_FORMAT_PCM32;
			RINFO("SetSoundFormat: PCM_32");
			break;
		};

		// FMOD Software Format setzen
		result = m_system->setSoftwareFormat(m_Config->Sound.SampleRate,
											 m_format, 0, 0, FMOD_DSP_RESAMPLER_LINEAR);
		if(!ERRCHECK(result)) return false;

		switch(m_Config->Sound.Channels)
		{
		case 1:
			m_speakermode = FMOD_SPEAKERMODE_MONO;
			RINFO("SetChannelMode: Mono");
			break;
		case 2:
			m_speakermode = FMOD_SPEAKERMODE_STEREO;
			RINFO("SetChannelMode: Stereo");
			break;
		case 4:
			m_speakermode = FMOD_SPEAKERMODE_QUAD;
			RINFO("SetChannelMode: 4.0");
			break;
		case 6:
			m_speakermode = FMOD_SPEAKERMODE_5POINT1;
			RINFO("SetChannelMode: 5.1");
			break;
		case 8:
			m_speakermode = FMOD_SPEAKERMODE_7POINT1;
			RINFO("SetChannelMode: 7.1");
			break;
		};
		// FMOD Speakermode
		result = m_system->setSpeakerMode(m_speakermode);
		if(!ERRCHECK(result)) return false;

		result = m_system->setDriver(m_Config->Sound.Soundkarte);
		if(!ERRCHECK(result)) return false;

		// FMOD Initialisieren
		result = m_system->init(32, FMOD_INIT_NORMAL, 0);
		if(!ERRCHECK(result)) return false;

		if(m_Config->Sound.b3D)
		{
			m_system->set3DSettings(1.0, DISTANCEFACTOR, 1.0f);
		}

		ROK("SoundEnginnee Erstellt");
		return true;
	}
	void raFMODSystem::Destroy()
	{
		m_system->close();
		m_system->release();

		SAFE_DELETE(m_system);
		SAFE_DELETE(m_Config);


	}
	bool raFMODSystem::Update()
	{
		if(!m_Cam) return false;

		if(m_Config->Sound.b3D)
		{
			FMOD_RESULT	result;

			result = m_system->set3DListenerAttributes(0, (FMOD_VECTOR*)m_Cam->GetEyePt(), 
													  (FMOD_VECTOR*)m_Cam->GetVelocity(), 
													  (FMOD_VECTOR*)m_Cam->GetWorldAhead(), 
													  (FMOD_VECTOR*)m_Cam->GetWorldUp());
			
			m_system->update();
		}
		return true;
	}
};