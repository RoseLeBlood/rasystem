#pragma once

	class RAPI raClientSocket : public raSocket
	{
	public:
		raClientSocket();

		bool Connect(const  raString IP, int Port);
		void Disconnect()
		{
			::closesocket(m_Socket);
		}
	protected:
		raClientSocket(const GUID& raguid);
	};

	class RAPI raClient
	{
	public:
		raClient(void);
		~raClient(void);

		void	        Create(HWND hWnd)	{ m_hWnd = hWnd; }
		void			Destroy() { }

		virtual bool	Connect(const  raString IP, int Port = 28015);
		virtual void	Disconnect();
		virtual int		Receive(void);
		virtual bool	Send(LPCSTR Data, int Size);
		virtual int     GetNextPacketID();

		 raString GetBuffer()		{ return m_Socket.GetBuffer(); }

		bool SendString( raString text);
		bool SendVector2(raVector2& Vec);
		bool SendVector3(raVector3& Vec);
		bool SendMatrix(raMatrix& Mat);
		bool SendQuaternion(raQuaternion& Non);
		bool SendRect(raRECT& Rec);
		bool SendColor(raColor& Col);
		bool SendPlane(raPlane& Plan);

		int	 ReadString( raString buffer);
		void ReadVector2(raVector2& Vec);
		void ReadVector3(raVector3& Vec);
		void ReadMatrix(raMatrix& Mat);
		void ReadQuaternion(raQuaternion& Non);
		void ReadRect(raRECT& Rec);
		void ReadColor(raColor& Col);
		void ReadPlane(raPlane& Plan);

	protected:
		raClient(const GUID& raguid);
	protected:
		virtual void OnDisconnect() {};
		virtual void OnConnect(const  raString IP, int Port) {};
		virtual void OnConnectionLost() {};
		virtual void OnReceive(int BytesToRead) {};
		virtual void OnSend(LPCSTR Data, int Size) {};
	private:
		raClientSocket	m_Socket;
		int				m_BytesToRead;

		char m_sendbuf[raSocket::RECVBUFLEN];
		 raString m_BufferPtr;
		HWND m_hWnd;
	};