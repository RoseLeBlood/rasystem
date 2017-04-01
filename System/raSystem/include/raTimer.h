#pragma once

	class RAPI raTimer
	{
	public:
		raTimer(void);
		~raTimer(void);

		void Start(void);
		void Stop(void);
		void UpDate(void);
		void Reset(void);

		float GetTime(void)				{ return m_runningTime; }
		float GetElapsed(void)			{ return m_timeElapsed; }

		READONLY_PROPERTY(float, fRunTime);
		GET(fRunTime) { return m_runningTime; }

		READONLY_PROPERTY(float, fTime);
		GET(fTime) { return m_timeElapsed; }

		READONLY_PROPERTY(__int64, CurTime);
		GET(CurTime) { return m_currentTime; }

		READONLY_PROPERTY(__int64, TicksPerSecond);
		GET(TicksPerSecond) { return m_ticksPerSecond; }

		READONLY_PROPERTY(__int64, LastTime);
		GET(LastTime) { return m_lastTime; }

		READONLY_PROPERTY(bool, IsStopped);
		GET(IsStopped) { return m_timerStopped; }

		READONLY_PROPERTY(float, FPS);
		GET(FPS) { return 1.0f / m_timeElapsed; }
	protected:
		raTimer(const GUID& raguid);

	private:
		__int64 m_ticksPerSecond;
		__int64 m_currentTime;
		__int64 m_lastTime;
		float m_timeElapsed;
		float m_runningTime;
		bool m_timerStopped;
	};