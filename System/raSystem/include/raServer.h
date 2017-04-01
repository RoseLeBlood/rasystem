#pragma once

	class RAPI raServerSocket : public raSocket
	{
	public:
		raServerSocket(void);

		void bind(int Port);
		void listen(void);
		void Stop(void);

		raSocket* accept(void);
	};

	class RAPI raServer
	{
	public:
		raServer(void);
		~raServer(void);

		void	Setup(int Port = 28015, int MaxClients = 10);
		int		Loop(void);
		void	Stop(void);

		raServerSocket		GetServerSocket()				{ return m_ListenSocket; }
		int					GetMaxClients()					{ return m_MaxClients; }
		raSocket*			GetClientSocket(int index)		{ return m_ClientSockets[index]; }

		bool				SetNewClient(raSocket* Client, int index);
	protected:
		raServer(const GUID& raguid);

	protected:
		virtual bool OnSetup(int Port, int MaxClients) { return true; };
		virtual bool OnClientConnect(raSocket* newClient)  { return true; };
		virtual bool OnClientDisconnect(raSocket* Client, int ClientNum)  { return true; };
		virtual int  OnRun( raString Buffer, int Size, int FromClient)	 { return 1; };
	private:
		raServerSocket		m_ListenSocket;
		int					m_MaxClients;
		raSocket			**m_ClientSockets;
	};