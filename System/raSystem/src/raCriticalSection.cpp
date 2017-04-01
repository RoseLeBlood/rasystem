#include "..\include\raMain.h"

namespace System
{
	raCriticalSection::raCriticalSection(void) : IRASection()
	{
	}

	raCriticalSection::~raCriticalSection(void)
	{
	}
	void raCriticalSection::Create(void)
	{
		InitializeCriticalSection( &m_Crit );
		SetCreated(true);
	}
	bool raCriticalSection::Create(DWORD SpinCount)
	{
		if(InitializeCriticalSectionAndSpinCount( &m_Crit, SpinCount))
			SetCreated(true);
		else
			SetCreated(false);

		return IsCreated();
	}
	void raCriticalSection::Destroy(void)
	{
		DeleteCriticalSection(&m_Crit);
		SetCreated(false);
	}
	void raCriticalSection::Enter( void )
	{
		if(IsCreated()) EnterCriticalSection( &m_Crit );
	}
	void raCriticalSection::Leave( void )
	{
		if(IsCreated()) LeaveCriticalSection( &m_Crit );
	}
}