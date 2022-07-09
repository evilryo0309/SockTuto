#include "XHttpServer.h"
#include "XHttpClient.h"
#include <thread>
using namespace std;
void XHttpServer::Stop()
{
	isexit = true;
}
void XHttpServer::Main()
{
	while (!isexit)
	{
		XTcp client = server.Accept();
		if (client.sock <= 0) continue;
		XHttpClient* th = new XHttpClient();
		th->Start(client);
	}
}
bool XHttpServer::Start(unsigned short port)
{
	isexit = false;
	server.CreateSocket();
	if (!server.Bind(port)) return false;
	thread sth(&XHttpServer::Main, this);
	sth.detach();
	return true;
}
XHttpServer::XHttpServer()
{
}

XHttpServer::~XHttpServer()
{
}
