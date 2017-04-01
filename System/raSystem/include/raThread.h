	class RAPI raThread
	{
	private:
			class raThreadContext
			{
			public:
				raThreadContext()
				{
					memset(this, 0, sizeof(this));
				}
			public:
				HANDLE m_hThread;
				DWORD  m_dwTID;
				LPVOID m_pUserData;
				LPVOID m_pParent;
				DWORD  m_dwExitCode;
			};
	public:
			raThread();
			raThread(LPTHREAD_START_ROUTINE lpExternalRoutine);
			~raThread();

			DWORD Start(void* arg = NULL);
			DWORD Stop (bool bForceKill = false);

			DWORD GetExitCode() const;

			void Attach(LPTHREAD_START_ROUTINE lpThreadStartRoutine );
			void Detach(void);

	protected:
			static DWORD WINAPI EntryPoint( LPVOID pArg);

			virtual DWORD Run( LPVOID  arg  );
			virtual void ThreadCtor(){	}
			virtual void ThreadDtor(){	}

	protected:
			raThreadContext					m_ThreadCtx;
			LPTHREAD_START_ROUTINE			m_pThreadStartRoutine;
		};