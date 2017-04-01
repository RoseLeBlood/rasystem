#pragma once

#ifndef AF_INET6
#define AF_INET6	23
#endif

#ifndef RA_INET
#define RA_INET		AF_INET
#endif

	class RAPI raSocket
	{
	public:
		raSocket(void);
		raSocket(SOCKET Socket);
		~raSocket(void);

		static void select(bool write);
		static void fdzero(bool write);
		void		fdset(bool write);
		void		fdclr(bool write);
		int			Receive(void);
		bool		Send(LPCSTR Data, int Size);

		 raString		GetBuffer()			{ return m_recvbuf; }

		static const int RECVBUFLEN = 512;

	protected:
		SOCKET			m_Socket;
		sockaddr_in		m_SocketAddress;

		static	FD_SET  fdSetSend;
		static  FD_SET  fdSetRecv;
		char			m_recvbuf[RECVBUFLEN];
	private:
		WSAData			m_WSAData;
	};