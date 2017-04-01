#pragma once

namespace raSystem
{
	

	class RAPI raFMODStream : public raStream
	{
	public:
		raFMODStream(raSoundDriver *System) : raStream(System)
		{
			m_Sound = NULL;
			m_Channel = NULL;
			paused = false;
			Position = raVector3(0.0f);
			Velocity = raVector3(0.0f);
			minDistance = 0.5f * DISTANCEFACTOR;
			maxDistance = 5000.0f * DISTANCEFACTOR;
			StreamBufferSize = 64*1024;
		}
		~raFMODStream();

		virtual bool LoadStream(void);
		virtual bool CloseStream(void);
		virtual bool PlayStream(void);
		virtual bool PauseStream(void);
		virtual bool StopStream(void);

		bool IsPaused(void)
		{
			bool ispaused = false;;
			if (m_Channel)
				m_Channel->getPaused(&ispaused);
			return ispaused;
		}
		bool IsPlayed(void)
		{
			bool isplaying = false;;
			if (m_Channel)
				m_Channel->isPlaying(&isplaying);
			return isplaying;
		}
		virtual int	 GetPosition(void);
		virtual void SetPosition(int ms);
		virtual int  GetStreamLänge(void);

		// 0.0 = silent, 1.0 = full volume 
		virtual bool SetVolume(float Volume = 1.0f);
		// -1.0 = Full left, 0.0 = center, 1.0 = full right
		virtual bool SetPan(float Pan = 0.0f);
		virtual bool Set3DMinMaxDistance(float min, float max);
		virtual bool SetMute(void);

		virtual bool GetMute(void);
		virtual float GetPan();
		virtual float GetVolume();

		virtual bool SetStreamPosition(raVector3& vec);
		virtual raVector3 GetStreamPosition(void);
		virtual bool SetSctreamVelocity(raVector3& vec);

		virtual raTag *GetTag(void);
	private:
		FMOD::Sound	*m_Sound;
		FMOD::Channel *m_Channel;
	};
};