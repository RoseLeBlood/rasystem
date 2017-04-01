#include "..\raSystem\include\raMain.h"
#include "..\raApplication\raApplication.h"

using namespace System;
using namespace std;

class Server;
class Client;

typedef raEvent<Server, raString> ServerEvent;
typedef raDelegate<Client, Server, raString> ClientDelegate;

class Server
{
public:
    ServerEvent MessageSent;

    void SendMessage(IRADelegateParam<raString>* p_msg)
    {
        MessageSent(this, p_msg);
    }
};

class Client
{
    static int s_id;
    int m_id;
    void MessageSentHandler(const Server* p_sender, IRADelegateParam<raString>* pParameter)
    {
        raConsole::WriteLine("Client %d\n\tParameter:", m_id);

		for(int i=0; i < pParameter->Count; i++)
		{
			System::raSmartPointer<raString> *parm = pParameter->GetAt(i);
			raConsole::WriteLine("\t\t%s", parm->Data->c_str());
		}
    }
public:
    Client(Server* p_server)
    {
        m_id = ++s_id;
        p_server->MessageSent += new ClientDelegate(this, &Client::MessageSentHandler);
    }
};
int Client::s_id = 0;

// Einstiegspunkt
void raMain(class System::raCmdParser *)
{
	raConsole::Clear();

	Server server;
    Client client1(&server), client2(&server), client3(&server);

	IRADelegateParam<raString> param;
	param += raSmartPointer<raString>(new raString("Hallo Welt"));
	param += raSmartPointer<raString>(new raString("Wie geht's?"));
	param += raSmartPointer<raString>(new raString("Ach ganz gut"));

    server.SendMessage(&param);

	_getch();
};