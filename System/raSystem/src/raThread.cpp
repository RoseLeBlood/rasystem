#include "raMain.h"

namespace System
{
	raThread::raThread()
	{
		m_pThreadStartRoutine = raThread::EntryPoint;
	}
	raThread::raThread(LPTHREAD_START_ROUTINE lpExternalRoutine)
	{
		Attach(lpExternalRoutine);
	}
	raThread::~raThread()
	{
		if ( m_ThreadCtx.m_hThread )
			Stop(true);
	}
	DWORD raThread::Start( void* arg)
	{
		m_ThreadCtx.m_pUserData = arg;
		m_ThreadCtx.m_hThread = CreateThread(NULL, 0, m_pThreadStartRoutine, this, 0, &m_ThreadCtx.m_dwTID);
		m_ThreadCtx.m_dwExitCode = (DWORD)-1;

		return GetLastError();
	}
	DWORD raThread::Stop ( bool bForceKill)
	{
		if ( m_ThreadCtx.m_hThread )
		{
			GetExitCodeThread(m_ThreadCtx.m_hThread, &m_ThreadCtx.m_dwExitCode);

			if ( m_ThreadCtx.m_dwExitCode == STILL_ACTIVE && bForceKill )
				TerminateThread(m_ThreadCtx.m_hThread, DWORD(-1));

			m_ThreadCtx.m_hThread = NULL;
		}

		return m_ThreadCtx.m_dwExitCode;
	}
	DWORD raThread::GetExitCode() const
	{
		if ( m_ThreadCtx.m_hThread )
			GetExitCodeThread(m_ThreadCtx.m_hThread, (LPDWORD)&m_ThreadCtx.m_dwExitCode);
		return m_ThreadCtx.m_dwExitCode;
	}
	void raThread::Attach( LPTHREAD_START_ROUTINE lpThreadStartRoutine )
	{
		m_pThreadStartRoutine = lpThreadStartRoutine;
	}
	void  raThread::Detach( void )
	{
		m_pThreadStartRoutine = raThread::EntryPoint;
	}
	DWORD WINAPI raThread::EntryPoint( LPVOID pArg)
	{
		raThread *pParent = reinterpret_cast<raThread*>(pArg);
		if(!pParent) return STILL_ACTIVE;

			pParent->ThreadCtor();

			pParent->Run( pParent->m_ThreadCtx.m_pUserData );

			pParent->ThreadDtor();

		return STILL_ACTIVE;
	}
	DWORD raThread::Run( LPVOID  arg  )
	{
		return 0;
	}
}