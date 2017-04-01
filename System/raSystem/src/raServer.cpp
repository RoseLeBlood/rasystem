#include "..\include\raMain.h"

namespace System
{
	int g_port;
	raServerSocket::raServerSocket() : raSocket()
	{
	}

	void raServerSocket::bind(int Port)
	{
		m_SocketAddress.sin_family = RA_INET;
		m_SocketAddress.sin_port = Port;
		m_SocketAddress.sin_addr.s_addr = INADDR_ANY;
		g_port = Port;

		//Blockierender Aufruf!
		::bind(m_Socket, (SOCKADDR*) &m_SocketAddress,
			sizeof(m_SocketAddress));
	}
	void raServerSocket::listen(void)
	{
		::listen(m_Socket, SOMAXCONN);
	}
	void raServerSocket::Stop(void)
	{
		Send(NULL, 0);
		::closesocket(m_Socket);
		RINFO("Server gestoppt");
	}
	raSocket* raServerSocket::accept(void)
	{
		if(FD_ISSET(m_Socket, &fdSetRecv))
		{
			return (new raSocket(::accept(m_Socket, NULL, NULL)));
		}
		return NULL;
	}
	//--------------------------------------------------------
	raServer::raServer(void)
	{
	}

	raServer::~raServer(void)
	{
		free(m_ClientSockets);
	}
	void raServer::Setup(int Port, int MaxClients)
	{
		m_ClientSockets = (raSocket**)malloc(MaxClients*sizeof(m_ClientSockets));
		m_MaxClients = MaxClients;

		for(int i = 0; i < MaxClients; i++)
		{
			m_ClientSockets[i] = NULL;
		}

		m_ListenSocket.bind(Port);
		m_ListenSocket.listen();
		RINFO("Server gestartet.\n");

		OnSetup(Port, MaxClients);
	}
	void raServer::Stop()
	{
		m_ListenSocket.Stop();
	}
	int raServer::Loop(void)
	{
		raSocket::fdzero(false);
		m_ListenSocket.fdset(false);

		for(int i=0; i < m_MaxClients; i++)
		{
			if(m_ClientSockets[i]) m_ClientSockets[i]->fdset(false);
		}
		raSocket::select(false);

		raSocket* pNewClient = m_ListenSocket.accept();
		if(pNewClient != NULL)
		{
			OnClientConnect(pNewClient);
		}

		raSocket::fdzero(true);
		for(int i=0; i < m_MaxClients; i++)
		{
			if(m_ClientSockets[i]) m_ClientSockets[i]->fdset(true);
		}
		raSocket::select(true);

		for(int i=0; i < m_MaxClients; i++)
		{
			if(m_ClientSockets[i])
			{
				int iResult = m_ClientSockets[i]->Receive();

				if(iResult > 0)
				{
					raString Buffer = m_ClientSockets[i]->GetBuffer();

					OnRun(Buffer, iResult, i);
				}
				else if(iResult == 0) //Client beendet die Verbindung
				{
					OnClientDisconnect(m_ClientSockets[i], i);
					SAFE_DELETE(m_ClientSockets[i]);
				}
			}
		}
		return 0;
	}
	bool raServer::SetNewClient(raSocket* Client, int index)
	{
		if(index > m_MaxClients) { RERROR_INVALIDVALUE("index"); return false; }
		if(!Client)	{ RERROR_NULLPOINTER("Client");return false; }

		m_ClientSockets[index] = Client;
		return true;
	}
};