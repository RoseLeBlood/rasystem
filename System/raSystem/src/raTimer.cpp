#include "..\include\raMain.h"

namespace System
{
raTimer::raTimer(void)
{
	QueryPerformanceFrequency( (LARGE_INTEGER *)&m_ticksPerSecond );

    m_currentTime = m_lastTime  = 0;
    m_runningTime = 0.0f;
    m_timerStopped = true;
	m_timeElapsed = 0.0f;
}
raTimer::raTimer(const GUID& raguid)
{
	QueryPerformanceFrequency( (LARGE_INTEGER *)&m_ticksPerSecond );

    m_currentTime = m_lastTime  = 0;
    m_runningTime = 0.0f;
    m_timerStopped = true;
	m_timeElapsed = 0.0f;
}
raTimer::~raTimer(void)
{
	Stop();
}
void raTimer::Start(void)
{
	if (!m_timerStopped)
		return;
	QueryPerformanceCounter( (LARGE_INTEGER *)&m_lastTime );
	m_timerStopped = false;
}
void raTimer::Stop(void)
{
	if(m_timerStopped)
		return;
	INT64 stopTime = 0;
    QueryPerformanceCounter( (LARGE_INTEGER *)&stopTime );
    m_runningTime += (float)(stopTime - m_lastTime) / (float)m_ticksPerSecond;
    m_timerStopped = true;
}
void raTimer::UpDate(void)
{
	if (m_timerStopped)
        return;
    QueryPerformanceCounter( (LARGE_INTEGER *)&m_currentTime );

    m_timeElapsed = (float)(m_currentTime - m_lastTime) / (float)m_ticksPerSecond;
    m_runningTime += m_timeElapsed;

	m_lastTime = m_currentTime;
}
void raTimer::Reset(void)
{
	m_runningTime = 0.0f;
	m_lastTime = 0;
}
};