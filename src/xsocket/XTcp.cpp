#include "XTcp.h"
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

XTcp::XTcp()
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
}
int XTcp::CreateSocket()
{
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
	{
		printf("create socket failed!\n");
	}
	return sock;
}
bool XTcp::Bind(unsigned short port)
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
int XTcp::Recv(char* buf, int bufsize)
{
	return recv(sock, buf, bufsize, 0);
}
int XTcp::Send(const char* buf, int size)
{
	int s = 0;
	while (s != size)
	{
		int len = send(sock, buf + s, size - s, 0);
		if (len <= 0)break;
		s += len;
	}
	return s;
}
void XTcp::Close()
{
	if (sock <= 0) return;
	closesocket(sock);
	sock = 0;
}
XTcp XTcp::Accept()
{
	XTcp tcp;
	sockaddr_in caddr;
	socklen_t len = sizeof(caddr);
	int client = accept(sock, (sockaddr*)&caddr, &len);
	if (client <= 0) return tcp;
	printf("accept client %d\n", client);
	char* ip = inet_ntoa(caddr.sin_addr);
#if WIN32
	strcpy_s(tcp.ip, ip);
#else
	strcpy(tcp.ip, ip);
#endif
	tcp.port = ntohs(caddr.sin_port);
	tcp.sock = client;
	printf("client ip is %s, port is %d\n", tcp.ip, tcp.port);
	return tcp;
}

bool XTcp::Connect(const char* ip, unsigned short port, int timeoutms)
{
	if (sock <= 0) CreateSocket();
	sockaddr_in saddr{};
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = inet_addr(ip);
	SetBlock(false);
	fd_set set;
	if (connect(sock, (sockaddr*)&saddr, sizeof(saddr)) != 0)
	{
		FD_ZERO(&set);
		FD_SET(sock, &set);
		timeval tm;
		tm.tv_sec = 0;
		tm.tv_usec = timeoutms * 1000;
		if (select(sock + 1, 0, &set, 0, &tm) <= 0)
		{
			printf("connect timeout or error!\n");
#if WIN32
		printf("connect %s:%d failed!\n", ip, port /*, strerror(errno) */);
#else
		printf("connect %s:%d failed!:%s\n", ip, port, strerror(errno));
#endif
			return false;
		}
		return true;
	}
	SetBlock(true);
	printf("connect %s:%d success!\n", ip, port);
	return true;
}
XTcp::~XTcp()
{
}

bool XTcp::SetBlock(bool isblock)
{
	if (sock <= 0) return false;
#ifdef WIN32
	unsigned long ul = 0;
	if (!isblock) ul = 1;
	ioctlsocket(sock, FIONBIO, &ul);
#else
	int flags = fcntl(sock, F_GETFL, 0);
	if (flags < 0)
		return false;
	if (isblock)
	{
		flags = flags & ~O_NONBLOCK;
	}
	else
	{
		flags = flags | O_NONBLOCK;
	}
	if (fcntl(sock, F_SETFL, flags) != 0)
		return false;
#endif
	return true;
}
