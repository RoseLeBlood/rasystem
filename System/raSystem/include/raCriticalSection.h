#pragma once

	class RAPI raCriticalSection : public IRASection
	{
	public:
		raCriticalSection(void);
		~raCriticalSection(void);

		virtual void Create(void);
		virtual bool Create(DWORD SpinCount);
		virtual void Destroy(void);

		virtual void Enter( void );
		virtual void Leave( void );

	private:
		CRITICAL_SECTION m_Crit;
	};

	//--------------------------------
	#define RACSECTION(x) if (System::raCriticalBlock __csc = x)

	class RAPI raCriticalBlock
	{
	public:
		raCriticalBlock(IRASection* rc) :  m_rc(rc)
		{
			m_rc->Enter();
		}
		~raCriticalBlock(void)
		{
			m_rc->Leave();
		}
		operator bool()
		{
			return true;
		}
	private:
		IRASection*		m_rc;
	};