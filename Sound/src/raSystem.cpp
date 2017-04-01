#include "..\include\raSound.h"

namespace System
{
	namespace Sound
	{
		bool raSystem::ERRCHECK(FMOD_RESULT result, char* Func)
		{
			if (result != FMOD_OK)
			{
				char    errstring[1024];

				sprintf(errstring, "FMOD (%d) %s [%s]", result, FMOD_ErrorString(result), Func);

				RERROR(errstring);
				return false;
			}
			return true;
		}
		raSystem::raSystem(void)
		{
		}

		raSystem::~raSystem(void)
		{
		}

		bool raSystem::Create(raSmartPointer<raCamera> Cam, raConfig* Config)
		{
			RACHANNELMODE mode = GetChannelFromConfig(Config->Channels);
			RASOUNDFORMAT format = GetFormatFromConfig(Config->Bit, Config->SampleRate);
			RASOUNDAUSGABE ausgabe = GetDriverFromConfig(Config->Driver);

			return Create(Cam, Config->Soundkarte, mode, format, ausgabe);
		}
		bool raSystem::Create(raSmartPointer<raCamera> Cam, int Soundkarte, RACHANNELMODE Mode,
								RASOUNDFORMAT PCMFORMAT, RASOUNDAUSGABE ausgabe)
		{
			m_cam = Cam;
			unsigned int version = 0;
			int samplerate = 44100;
			FMOD_SOUND_FORMAT m_format;

			RINFO("[Init Sound FMOD-System ... ]");

			{
				FMOD_RESULT result = FMOD::System_Create(&m_system);
				if(!ERRCHECK(result)) return false;

				result = m_system->getVersion(&version);
				if(!ERRCHECK(result)) return false;

				if (version < FMOD_VERSION)
				{
					MessageBox(GetForegroundWindow(), "INCORRECT DLL VERSION!!", "FMOD ERROR", MB_OK);
					return false;
				}
				// Set Plugins
				result = m_system->setPluginPath("raPlugins/FMOD");
				ERRCHECK(result);

				// Set Soundkarte
				result = m_system->setDriver(Soundkarte);
				ERRCHECK(result);

				switch(ausgabe)
				{
				case RA_WINMM:
					m_system->setOutput(FMOD_OUTPUTTYPE_WINMM);
					break;
				case RA_DSOUND:
					m_system->setOutput(FMOD_OUTPUTTYPE_DSOUND);
					break;
				case RA_WASAPI:
					m_system->setOutput(FMOD_OUTPUTTYPE_WASAPI);
					break;
				case RA_ASIO:
					m_system->setOutput(FMOD_OUTPUTTYPE_ASIO);
					break;
				};
				// FMOD Software Format setzen
				switch(PCMFORMAT)
				{
				case RA_PCM16_44100:
					samplerate = 44100;
					m_format = FMOD_SOUND_FORMAT_PCM16;
					break;
				case RA_PCM16_48000:
					samplerate = 48000;
					m_format = FMOD_SOUND_FORMAT_PCM16;
					break;
				case RA_PCM16_96000:
					samplerate = 96000;
					m_format = FMOD_SOUND_FORMAT_PCM16;
					break;
				case RA_PCM24_44100:
					samplerate = 44100;
					m_format = FMOD_SOUND_FORMAT_PCM24;
					break;
				case RA_PCM24_48000:
					samplerate = 48000;
					m_format = FMOD_SOUND_FORMAT_PCM24;
					break;
				case RA_PCM24_96000:
					samplerate = 96000;
					m_format = FMOD_SOUND_FORMAT_PCM24;
					break;
				case RA_PCM24_192000:
					samplerate = 192000;
					m_format = FMOD_SOUND_FORMAT_PCM24;
					break;
				case RA_PCMFLOAT_44100:
					samplerate = 44100;
					m_format = FMOD_SOUND_FORMAT_PCMFLOAT;
					break;
				case RA_PCMFLOAT_48000:
					samplerate = 48000;
					m_format = FMOD_SOUND_FORMAT_PCMFLOAT;
					break;
				case RA_PCMFLOAT_96000:
					samplerate = 96000;
					m_format = FMOD_SOUND_FORMAT_PCMFLOAT;
					break;
				case RA_PCMFLOAT_192000:
					samplerate = 192000;
					m_format = FMOD_SOUND_FORMAT_PCMFLOAT;
					break;
				case RA_PCM32_44100:
					samplerate = 44100;
					m_format = FMOD_SOUND_FORMAT_PCM32;
					break;
				case RA_PCM32_48000:
					samplerate = 48000;
					m_format = FMOD_SOUND_FORMAT_PCM32;
					break;
				case RA_PCM32_96000:
					samplerate = 96000;
					m_format = FMOD_SOUND_FORMAT_PCM32;
					break;
				case RA_PCM32_192000:
					samplerate = 192000;
					m_format = FMOD_SOUND_FORMAT_PCM32;
					break;
				}

				m_system->setSoftwareFormat(samplerate, m_format, 0, 0, FMOD_DSP_RESAMPLER_LINEAR);

				switch(Mode)
				{
				case RA_SOUND_MONO:
					result = m_system->setSpeakerMode(FMOD_SPEAKERMODE_MONO);
					break;
				case RA_SOUND_STEREO:
					result = m_system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
					break;
				case RA_SOUND_QUAD:
					result = m_system->setSpeakerMode(FMOD_SPEAKERMODE_QUAD);
					break;
				case RA_SOUND_51:
					result = m_system->setSpeakerMode(FMOD_SPEAKERMODE_5POINT1);
					break;
				case RA_SOUND_71:
					result = m_system->setSpeakerMode(FMOD_SPEAKERMODE_7POINT1);
					break;
				case RA_SOUND_SURROUND:
					result = m_system->setSpeakerMode(FMOD_SPEAKERMODE_SURROUND);
					break;
				case RA_SOUND_PROLOGIC:
					result = m_system->setSpeakerMode(FMOD_SPEAKERMODE_SRS5_1_MATRIX);
					break;
				case RA_SOUND_MYEARS:
					result = m_system->setSpeakerMode(FMOD_SPEAKERMODE_MYEARS);
					break;
				};
				// FMOD Initialisieren
				result = m_system->init(64, FMOD_INIT_NORMAL, 0);
				if(!ERRCHECK(result)) return false;

				result = m_system->setStreamBufferSize(32768, FMOD_TIMEUNIT_RAWBYTES);
				ERRCHECK(result);

				m_system->set3DSettings(1.0, DISTANCEFACTOR, 1.0f);
				this->Update();

				// Alle Plug-Ins Laden
				{
					HANDLE fHandle;
					WIN32_FIND_DATA wfd;

					fHandle=FindFirstFile("./raPlugins/FMOD/*",&wfd);

					unsigned int pre = 2500;
					do
					{
						if (!( (wfd.cFileName[0]=='.') && ( (wfd.cFileName[1]=='.' && wfd.cFileName[2]==0) || wfd.cFileName[1]==0 ) ))
						{
							if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
							{
							}
							else
							{
								m_system->loadPlugin(wfd.cFileName, &pre);
							}
						}
					}
					while (FindNextFile(fHandle,&wfd));
					FindClose(fHandle);
				}
			}
			ROK("\tFMOD OK");
			return true;
		}
		void raSystem::Destroy()
		{
			m_system->close();
			m_system->release();
		}

		bool raSystem::Update()
		{
			{
				if(!m_cam.Data) return false;

				FMOD_VECTOR eyept, velocty, worldahead, worldup;
				raVector3* ceyept, *cvelocty, *cworldahead, *cworldup;

				ceyept = m_cam->GetEyePt();
				cvelocty = (m_cam->GetVelocity());

				cworldahead = (m_cam->GetWorldAhead());
				cworldup = (m_cam->GetWorldUp());

				eyept.x = ceyept->x;
				eyept.y = ceyept->y;
				eyept.z = ceyept->z;

				velocty.x = cvelocty->x;
				velocty.y = cvelocty->y;
				velocty.z = cvelocty->z;

				worldahead.x = cworldahead->x;
				worldahead.y = cworldahead->y;
				worldahead.z = cworldahead->z;

				worldup.x = cworldup->x;
				worldup.y = cworldup->y;
				worldup.z = cworldup->z;

				m_system->set3DListenerAttributes(0, &eyept,
													 &velocty,
													 &worldahead,
													 &worldup);

				m_system->update();
			}
			return true;
		}
		//-------------------
		RACHANNELMODE raSystem::GetChannelFromConfig(int Channels)
		{
			RACHANNELMODE mode = RA_SOUND_STEREO;
			switch(Channels)
			{
			case 1:
				mode = RA_SOUND_MONO;
				break;
			case 2:
				mode = RA_SOUND_STEREO;
				break;
			case 4:
				mode = RA_SOUND_QUAD;
				break;
			case 6:
				mode = RA_SOUND_51;
				break;
			case 8:
				mode = RA_SOUND_71;
				break;
			case 9:
				mode = RA_SOUND_SURROUND;
				break;
			case 10:
				mode = RA_SOUND_PROLOGIC;
				break;
			case 11:
				mode = RA_SOUND_MYEARS;
			};
			return mode;
		}
		RASOUNDFORMAT raSystem::GetFormatFromConfig(int Bit, int Samplerate)
		{
			RASOUNDFORMAT format = RA_PCM16_44100;
			switch(Bit)
			{
			case 16:
				switch(Samplerate)
				{
				case 44100:
					format = RA_PCM16_44100;
					break;
				case 48000:
					format = RA_PCM16_48000;
					break;
				};
				break;
			case 24:
				switch(Samplerate)
				{
				case 44100:
					format = RA_PCM24_44100;
					break;
				case 48000:
					format = RA_PCM24_48000;
					break;
				case 96000:
					format = RA_PCM24_96000;
					break;
				case 192000:
					format = RA_PCM24_192000;
					break;
				};
				break;
			case 32:
				switch(Samplerate)
				{
				case 44100:
					format = RA_PCM32_44100;
					break;
				case 48000:
					format = RA_PCM32_48000;
					break;
				case 96000:
					format = RA_PCM32_96000;
					break;
				case 192000:
					format = RA_PCM32_192000;
					break;
				default:
					format = RA_PCM32_44100;
				};
				break;
			case 0:
				switch(Samplerate)
				{
				case 44100:
					format = RA_PCMFLOAT_44100;
					break;
				case 48000:
					format = RA_PCMFLOAT_48000;
					break;
				case 96000:
					format = RA_PCMFLOAT_96000;
					break;
				case 192000:
					format = RA_PCMFLOAT_192000;
					break;
				default:
					format = RA_PCMFLOAT_44100;
				};
				break;
			};
			return format;
		}
		RASOUNDAUSGABE raSystem::GetDriverFromConfig(int Driver)
		{
			RASOUNDAUSGABE ausgabe;
			switch(Driver)
			{
			case 0:
				ausgabe = RA_WINMM;
				break;
			case 1:
				ausgabe = RA_DSOUND;
				break;
			case 2:
				ausgabe = RA_WASAPI;
				break;
			default:
				ausgabe = RA_WASAPI;
			};
			return ausgabe;
		}
		//-------------------
	}
}