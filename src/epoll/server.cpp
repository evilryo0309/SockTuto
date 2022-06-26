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
#include <sys/epoll.h>
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
	// create epoll
	int epfd = epoll_create(256);

	epoll_event ev;
	ev.data.fd = server.sock;
	ev.events = EPOLLIN | EPOLLET;
	epoll_ctl(epfd, EPOLL_CTL_ADD, server.sock, &ev);
	epoll_event event[20];
	char buf[1024] = { 0 };
	const char* msg = "HTTP/1.1 200 OK\r\nContent-Length:1\r\n\r\nX";
	int size = strlen(msg);
	server.SetBlock(false);
	for (;;)
	{
		int count = epoll_wait(epfd, event, 20, 500);
		if (count <= 0) continue;
		for (int i = 0; i < count; i++)
		{
			if (event[i].data.fd == server.sock)
			{
				for (;;)
				{
					XTcp client = server.Accept();
					if (client.sock <= 0) break;
					ev.data.fd = client.sock;
					ev.events = EPOLLIN | EPOLLET;
					epoll_ctl(epfd, EPOLL_CTL_ADD, client.sock, &ev);
				}
			}
			else
			{
				XTcp client;
				client.sock = event[i].data.fd;
				client.Recv(buf, 1024);
				client.Send(msg, size);
				epoll_ctl(epfd, EPOLL_CTL_DEL, client.sock, &ev);
				client.Close();
			}
		}
		/*XTcp client = server.Accept();
		TcpThread* th = new TcpThread();
		th->client = client;
		std::thread sth(&TcpThread::Main, th);
		sth.detach();*/
	}
	server.Close();

	getchar();
	return 0;
}
