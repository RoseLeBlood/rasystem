#ifndef __RA_EXCEPTION__
#define __RA_EXCEPTION__

namespace System
{
	class raException
	{
	public:
		raException (char* msg) :  m_err (GetLastError()), m_msg(msg)  {};

		DWORD GetError() const			{ return m_err; }
		char* GetMessage () const		{ return m_msg; }
	private:
		DWORD	m_err;
		char*	m_msg;
	};
}
#endif