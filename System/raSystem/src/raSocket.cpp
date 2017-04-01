#include "..\include\raMain.h"
#include <ws2def.h>

namespace System
{
	FD_SET	raSocket::fdSetSend;
	FD_SET  raSocket::fdSetRecv;

	raSocket::raSocket()
	{
		DWORD ipv6only = 0;

		m_SocketAddress.sin_family = RA_INET;
		/*m_SocketAddress.sin_port = 1234;
		m_SocketAddress.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");*/

		int err = WSAStartup(MAKEWORD( 2, 2 ), &m_WSAData);
		if ( err != 0 )
		{
			char buf[64];
			sprintf(buf, "WinSock 2.2 kann nicht gestartet werden, Fehler Code %d", err);
			RERROR(buf);
		}
		m_Socket = ::socket(RA_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_Socket == INVALID_SOCKET)
		{
			char buf[64];
			sprintf(buf, "Create Socket, Fehler Code %d", err);
			RERROR(buf);
			return;
		}
	}

	raSocket::raSocket(SOCKET socket)
	{
		int err =  WSAStartup(MAKEWORD(2, 2), &m_WSAData);
		if ( err != 0 )
		{
			char buf[64];
			sprintf(buf, "WinSock kann nicht gestartet werden, Fahler Code", err);
			RERROR(buf);
		}
		m_Socket = socket;
	}
	raSocket::~raSocket()
	{
		closesocket(m_Socket);
		WSACleanup();
	}
	void raSocket::fdzero(bool write)
	{
		if(write)
			FD_ZERO(&fdSetSend);
		else
			FD_ZERO(&fdSetRecv);
	}
	void raSocket::fdset(bool write)
	{
		if(m_Socket != INVALID_SOCKET)
		{
			if(write)
				FD_SET(m_Socket, &fdSetSend);
			else
				FD_SET(m_Socket, &fdSetRecv);
		}
	}
	void raSocket::fdclr(bool write)
	{
		if(m_Socket != INVALID_SOCKET)
		{
			if(write)
				FD_CLR(m_Socket, &fdSetSend);
			else
				FD_CLR(m_Socket, &fdSetRecv);
		}
	}
	void raSocket::select(bool write)
	{
		struct timeval t = {0, 5};
		if(write)
			::select(0, NULL, &fdSetSend, NULL, &t);
		else
			::select(0, &fdSetRecv, NULL, NULL, &t);
	}
	int raSocket::Receive(void)
	{
		if(FD_ISSET(m_Socket, &fdSetRecv))
		{
			return ::recv(m_Socket, m_recvbuf, RECVBUFLEN, 0);
		}
		return -1;
	}
	bool raSocket::Send(LPCSTR Data, int Size)
	{
		if(FD_ISSET(m_Socket, &fdSetSend))
		{
			int iRet = ::send(m_Socket, Data, Size, 0);
			if(iRet == SOCKET_ERROR)
			{
				printf("Socket nicht vorhanden");
				return false;
			}
		}
		return true;
	}
};