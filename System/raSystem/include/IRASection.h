#pragma once

	class RAPI IRASection
	{
	public:
		IRASection()
		{
			m_IsCreated = false;
		}

		virtual void Create(void) = 0;
		virtual bool Create(DWORD SpinCount) = 0;
		virtual void Destroy(void) = 0;

		virtual void Enter( void ) = 0;
		virtual void Leave( void ) = 0;

		virtual bool IsCreated()			{ return m_IsCreated; }
	protected:
		virtual void SetCreated(bool crt)	{ m_IsCreated = crt; }
	protected:
		bool	m_IsCreated;
	};