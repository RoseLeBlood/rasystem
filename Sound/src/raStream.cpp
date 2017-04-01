#include "..\include\raSound.h"

namespace System
{
	namespace Sound
	{
		raStream::raStream(void)
		{
		}

		raStream::~raStream(void)
		{
		}
		bool raStream::Create(raSmartPointer<Sound::raSystem> AudioSystem)
		{
			m_sound = NULL;
			m_channel = NULL;
			m_system = NULL;

			m_system = AudioSystem;
			return true;
		}

		void raStream::Destroy()
		{
			FMOD_RESULT result;

			StopStream();
			result = m_sound->release();
			if(!ERRCHECK(result, "m_sound::Release")) return;

			SAFE_DELETE(m_channel);
		}
		bool raStream::LoadStream(raString FileName, bool b3D)
		{
			FMOD_RESULT result;
			FMOD::System* sys = m_system->FSystem;

			if(sys)
			{
				std::cout << FileName << std::endl;
				result = sys->createStream(FileName.c_str(), (b3D) ? FMOD_3D : FMOD_2D , 0, &m_sound);
			}
			else
			{
				RERROR("System nicht vorhanden");
				return false;
			}
			return ERRCHECK(result,"CreateStream");
		}
		bool raStream::PlayStream(raVector3 Position, float minDistance,
				float maxDistance, raVector3 Velocity)
		{
			if(!m_system.Data) { RERROR("Sound System nicht vorhanden"); return false; }

			if(m_channel)
			{
				m_channel->stop();
			}
			FMOD_RESULT result = m_system->FSystem->playSound(FMOD_CHANNEL_FREE, m_sound, true, &m_channel);
			if(!ERRCHECK(result,"playSound" )) return false;
			ROK("Play Stream");

			FMOD_VECTOR position, velocity;
			position.x = Position.x;
			position.y = Position.y;
			position.z = Position.z;

			velocity.x = Velocity.x;
			velocity.y = Velocity.y;
			velocity.z = Velocity.z;

			result = m_channel->set3DAttributes( (FMOD_VECTOR*)&position, (FMOD_VECTOR*)&velocity );
			if(!ERRCHECK(result,"playSound" )) return false;

			Set3DMinMaxDistance(minDistance, maxDistance);

			result = m_channel->setPaused(false);
			if(!ERRCHECK(result, "setPaused")) return false;

			return true;
		}
		bool raStream::PauseStream()
		{
			static bool m_paused;

			if(m_channel )
			{
				FMOD_RESULT result = m_channel->getPaused(&m_paused);
				if(!ERRCHECK(result, "getPaused")) return false;

				result =m_channel->setPaused(!m_paused);
				if(!ERRCHECK(result, "setPaused")) return false;
			}
			return true;
		}
		bool raStream::StopStream()
		{
			if(m_channel)
			{
				m_channel->stop();
				CloseStream();
			}
			return true;
		}
		bool raStream::CloseStream()
		{
			FMOD_RESULT result;
			if(m_sound)
			{
				result = m_sound->release();
				m_channel = NULL;
			}
			return ERRCHECK(result, "raStream::CloseStream");
		}
		void raStream::Set3DMinMaxDistance(float min, float max)
		{
			m_sound->set3DMinMaxDistance(min, max);
		}
		bool raStream::SetVelocity(raVector3& vec)
		{
			if (m_channel)
			{
				FMOD_VECTOR pos;
				FMOD_RESULT result = m_channel->get3DAttributes( (FMOD_VECTOR*)&pos, 0);
				if(!ERRCHECK(result, "SetVelocity::get3DAttributes")) return false;

				pos.x = vec.x;
				pos.y = vec.y;
				pos.z = vec.z;

				result = m_channel->set3DAttributes( (FMOD_VECTOR*)&pos, (FMOD_VECTOR*)&pos );
				if(!ERRCHECK(result, "SetVelocity::set3DAttributes")) return false;

				return true;
			}
			return false;
		}
		bool raStream::Set3DPosition(raVector3& vec)
		{
			if (m_channel)
			{
				FMOD_VECTOR vel;

				FMOD_RESULT result = m_channel->get3DAttributes( 0, (FMOD_VECTOR*)&vel);
				if(!ERRCHECK(result, "Set3DPosition::get3DAttributes")) return false;

				vel.x = vec.x;
				vel.y = vec.y;
				vel.z = vec.z;

				result = m_channel->set3DAttributes( (FMOD_VECTOR*)&vec, (FMOD_VECTOR*)&vel );
				if(!ERRCHECK(result, "Set3DPosition::set3DAttributes")) return false;

				return true;
			}
			return false;
		}

		int	 raStream::GetPosition(void)
		{
			unsigned int currtime = 0;
			if(m_channel)
			{
				m_channel->getPosition(&currtime, FMOD_TIMEUNIT_MS);
			}
			return currtime;
		}
		int  raStream::GetLenght(void)
		{
			unsigned int length = 0;
			if(m_channel)
			{
				m_sound->getLength(&length, FMOD_TIMEUNIT_MS);
			}
			return length;
		}
		bool raStream::SetChannelMix(float fleft, float fright, float rleft, float rright,
				float sleft, float sright, float center, float bass)
		{
			 FMOD_RESULT result = m_channel->setSpeakerMix(fleft, fright, center, bass, rleft, rright, sleft, sright);
			 if(!ERRCHECK(result, "SetChannelMix")) return false;

			 return true;
		}
		bool  raStream::SetPosition(unsigned int pos)
		{
			if(!m_channel) return false;

			FMOD_RESULT result = m_channel->setPosition(pos, FMOD_TIMEUNIT_MS);
			if(!ERRCHECK(result, "SetPosition")) return false;

			 return true;
		}
		void raStream::SetVolume(float vol)
		{
			m_channel->setVolume(vol);
		}
		raSmartPointer<IRATag> raStream::GetTag(raString Name, int Index)
		{
			FMOD_TAG mftag;
			raSmartPointer<IRATag> sITag;

			if (m_sound->getTag(Name.c_str(), Index, &mftag) == FMOD_OK)
			{
				sITag = raSmartPointer<IRATag>(new IRATag((LPCSTR)(mftag.data), (LPCSTR)mftag.name));

				printf("%s %s\n", mftag.name, mftag.data);
			}

			return sITag;
		}
		bool raStream::ERRCHECK(FMOD_RESULT result, char* Func)
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
	}
}