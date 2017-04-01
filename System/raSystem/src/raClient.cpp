#include "..\include\raMain.h"

namespace System
{
	// ----------------------------------------
	raClientSocket::raClientSocket() : raSocket()
	{
	}

	bool raClientSocket::Connect(const  raString IP, int Port)
	{
		m_SocketAddress.sin_family = RA_INET;
		m_SocketAddress.sin_port = Port;
		m_SocketAddress.sin_addr.S_un.S_addr = inet_addr(IP.c_str());

		int err;
		err = ::connect(m_Socket, (SOCKADDR*) &m_SocketAddress, sizeof(m_SocketAddress));
		if(err != 0)
		{
			char buf[64];
			sprintf(buf, "ClientSocket::Connect, Fehler Code %d", err);
			RERROR(buf);
		}

		return true;
	}
	//------------------------------------------------------------------------------------------
	raClient::raClient(void)
	{
	}
	raClient::~raClient(void)
	{
	}
	bool raClient::Connect(const raString IP, int Port)
	{
		m_Socket.Connect(IP, Port);
		OnConnect(IP, Port);
		return true;
	}
	int raClient::Receive()
	{
		raSocket::fdzero(false);
		m_Socket.fdset(false);

		raSocket::select(false);
		m_BytesToRead = m_Socket.Receive();
		m_BufferPtr = m_Socket.GetBuffer();

		if(m_BytesToRead == 0)
		{
			OnConnectionLost();
		}

		OnReceive(m_BytesToRead);

		return m_BytesToRead;
	}
	bool raClient::Send(LPCSTR Data, int Size)
	{
		raSocket::fdzero(true);
		m_Socket.fdset(true);

		raSocket::select(true);

		OnSend(Data, Size);
		return m_Socket.Send(Data, Size);
	}
	int raClient::GetNextPacketID()
	{
		if(m_BufferPtr.c_str())
			return *((int*) m_BufferPtr.c_str());
		else return -1;
	}
	bool raClient::SendString( raString Text)
	{
		int* pPID = (int*)m_sendbuf;
		*pPID = PID_TEXT;
		*(pPID + 1) = (int)strlen(Text.c_str());
		strncpy(m_sendbuf + 8, Text.c_str(), raSocket::RECVBUFLEN - 8);

		return Send(m_sendbuf,  (int)sizeof(raPacketID) + (int)(sizeof(int) + strlen(Text.c_str()) + 1));
	}

	bool raClient::SendVector3(raVector3& Vec)
	{
		int* pPID = (int*)m_sendbuf;
		*pPID = PID_VECTOR3;
		float* px = (float*) pPID + 1;
		float* py = px + 1;
		float* pz = py + 1;
		*px = Vec.x;
		*py = Vec.y;
		*pz = Vec.z;
		return Send(m_sendbuf, sizeof(raPacketID) + 3 * sizeof(float));
	}
	int raClient::ReadString( raString Buffer)
	{
		int* pPID = (int*) m_BufferPtr.c_str();
		int laenge = *(pPID + 1);

		//strcpy(Buffer.c_str(), ( char*)(pPID + 2));
		Buffer = raString((char*)(pPID + 2));

		int nBytesConsumed = 2 * sizeof(int) + laenge + 1;
		m_BytesToRead -= nBytesConsumed;
		if(m_BytesToRead > 0)
			m_BufferPtr += nBytesConsumed;
		else
			m_BufferPtr = raString(NULL);
		return laenge;
	}

	void raClient::ReadVector3(raVector3& Vec)
	{
		int* pPID = (int*) m_BufferPtr.c_str();

		float* px = (float*) pPID + 1;
		float* py = px + 1;
		float* pz = py + 1;

		Vec.x = *px;
		Vec.y = *py;
		Vec.z = *pz;

		int nBytesConsumed = sizeof(int) + 3 * sizeof(float);
		m_BytesToRead -= nBytesConsumed;
		if(m_BytesToRead > 0)
			m_BufferPtr += nBytesConsumed;
		else
			m_BufferPtr = raString(NULL);
	}
	void raClient::ReadVector2(raVector2& Vec)
	{
		int* pPID = (int*) m_BufferPtr.c_str();

		float* px = (float*)pPID + 1;
		float* py = px + 1;

		Vec.x = *px;
		Vec.y = *py;

		int nBytesConsumed = sizeof(int) + 2 * sizeof(float);
		m_BytesToRead -= nBytesConsumed;
		if(m_BytesToRead > 0)
			m_BufferPtr += nBytesConsumed;
		else
			m_BufferPtr = raString(NULL);
	}
	void raClient::ReadMatrix(raMatrix& Mat)
	{
		int* pPID = (int*) m_BufferPtr.c_str();

		float *m11, *m12, *m13, *m14,
			  *m21, *m22, *m23, *m24,
			  *m31, *m32, *m33, *m34,
			  *m41, *m42, *m43, *m44;

		m11 = (float*)pPID + 1;
		m12 = m11 + 1;
		m13 = m12 + 1;
		m14 = m13 + 1;
		m21 = m14 + 1;
		m22 = m21 + 1;
		m23 = m22 + 1;
		m24 = m23 + 1;
		m31 = m24 + 1;
		m32 = m31 + 1;
		m33 = m32 + 1;
		m34 = m33 + 1;
		m41 = m34 + 1;
		m42 = m41 + 1;
		m43 = m42 + 1;
		m44 = m43 + 1;

		Mat = raMatrix(*m11, *m12, *m13, *m14, *m21, *m22, *m23, *m24, *m31, *m32, *m33, *m34,
			*m41, *m42, *m43, *m44);

		int nBytesConsumed = sizeof(int) + 16 * sizeof(float);
		m_BytesToRead -= nBytesConsumed;
		if(m_BytesToRead > 0)
			m_BufferPtr += nBytesConsumed;
		else
			m_BufferPtr = raString(NULL);
	}
	void raClient::ReadQuaternion(raQuaternion &Non)
	{
		int* pPID = (int*) m_BufferPtr.c_str();

		float *x = (float*)pPID + 1;
		float *y = x + 1;
		float *z = y + 1;
		float *s = z + 1;

		Non.v.x = *x;
		Non.v.y = *y;
		Non.v.z = *z;
		Non.s = *s;

		int nBytesConsumed = sizeof(int) + 16 * sizeof(float);
		m_BytesToRead -= nBytesConsumed;
		if(m_BytesToRead > 0)
			m_BufferPtr += nBytesConsumed;
		else
			m_BufferPtr = raString(NULL);
	}
	void raClient::ReadRect(raRECT& Rec)
	{
		int *pPID = (int*)m_BufferPtr.c_str();

		long *x1, *x2;
		long *y1, *y2;

		x1 = (long*)pPID + 1;
		x2 = x1 + 1;
		y1 = x2 + 1;
		y2 = y1 + 1;

		Rec.x1 = *x1;
		Rec.x2 = *x2;
		Rec.y1 = *y1;
		Rec.y2 = *y2;

		int nBytesConsumed = sizeof(int) + 16 * sizeof(float);
		m_BytesToRead -= nBytesConsumed;
		if(m_BytesToRead > 0)
			m_BufferPtr += nBytesConsumed;
		else
			m_BufferPtr = raString(NULL);
	}
	void raClient::ReadColor(raColor& Col)
	{
		int *pPID = (int*)m_BufferPtr.c_str();

		float *r, *g, *b, *a;

		r = (float*)pPID + 1;
		g = r + 1;
		b = g + 1;
		a = b + 1;

		Col.r = *r;
		Col.g = *g;
		Col.b = *b;
		Col.a = *a;

		int nBytesConsumed = sizeof(int) + 16 * sizeof(float);
		m_BytesToRead -= nBytesConsumed;
		if(m_BytesToRead > 0)
			m_BufferPtr += nBytesConsumed;
		else
			m_BufferPtr = raString(NULL);
	}
	void raClient::ReadPlane(raPlane& Plan)
	{
		int *pPID = (int*)m_BufferPtr.c_str();

		float *a, *b, *c, *d;

		a = (float*)pPID + 1;
		b = a + 1;
		c = b + 1;
		d = c + 1;

		Plan.a = *a;
		Plan.b = *b;
		Plan.c = *c;
		Plan.d = *d;

		int nBytesConsumed = sizeof(int) + 16 * sizeof(float);
		m_BytesToRead -= nBytesConsumed;
		if(m_BytesToRead > 0)
			m_BufferPtr += nBytesConsumed;
		else
			m_BufferPtr = raString(NULL);
	}

	void raClient::Disconnect()
	{
		OnDisconnect();

		m_Socket.Send(NULL, 0);
		m_Socket.Disconnect();
	}
};