#include <stdlib.h>
#include "XTcp.h"
#include <thread>
#include <string.h>
class TcpThread
{
public:
	void Main()
	{
		char buf[1024] = { 0 };
		for (;;)
		{
			int recvlen = client.Recv(buf, sizeof(buf) - 1);
			if (recvlen <= 0) break;
			buf[recvlen] = '\0';
			if (strstr(buf, "quit") != NULL)
			{
				char re[] = "quit success!\n";
				client.Send(re, strlen(re) + 1);
				break;
			}
			int sendlen = client.Send("ok\n", 4);
			printf("recv %s\n", buf);
		}
		client.Close();
		delete this;
	}
	XTcp client;
};

int main(int argc, char* argv[])
{
	unsigned short port = 8080;
	if (argc > 1)
	{
		port = atoi(argv[1]);
	}
	XTcp server;
	server.CreateSocket();
	server.Bind(port);
	for (;;)
	{
		XTcp client = server.Accept();
		TcpThread* th = new TcpThread();
		th->client = client;
		std::thread sth(&TcpThread::Main, th);
		sth.detach();
	}
	server.Close();

	getchar();
	return 0;
}
