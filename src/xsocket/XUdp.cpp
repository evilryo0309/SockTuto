#include "XUdp.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef WIN32
#include <windows.h>
#define socklen_t int
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#define closesocket close
#endif

#include <thread>
using namespace std;

XUdp::XUdp()
{
#ifdef WIN32
	static bool first = true;
	if (first)
	{
		first = false;
		WSADATA ws;
		WSAStartup(MAKEWORD(2, 2), &ws);
	}
#endif

	memset(ip, 0, sizeof(ip));
}


int XUdp::CreateSocket()
{
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == -1)
	{
		printf("create socket failed!\n");
	}
	return sock;
}

void XUdp::Close()
{
	if (sock <= 0) return;
	closesocket(sock);
	if (addr)
	{
		delete addr;
	}
	addr = 0;
	sock = 0;
}

bool XUdp::Bind(unsigned short port)
{
	if (sock <= 0)
		CreateSocket();
	sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = htons(0);
	if (::bind(sock, (sockaddr*)&saddr, sizeof(saddr)) != 0)
	{
		printf("bind port %d failed!\n", port);
		return false;
	}
	printf("bind port %d success!\n", port);
	listen(sock, 10);
	return true;
}
int XUdp::Recv(char* buf, int bufsize)
{
	if (sock <= 0) return 0;
	if (addr == 0)
	{
		addr = new sockaddr_in();
	}
	socklen_t len = sizeof(sockaddr_in);
	int re = recvfrom(sock, buf, bufsize, 0, (sockaddr*)addr, &len);
	return re;
}
XUdp::~XUdp()
{

}