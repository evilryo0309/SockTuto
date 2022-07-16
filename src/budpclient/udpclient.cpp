#include <iostream>
using namespace std;
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

int main(int argc, char* argv[])
{
	unsigned short port = 8080;
	if (argc > 1)
	{
		port = atoi(argv[1]);
	}
#ifdef WIN32
	WSADATA ws;
	WSAStartup(MAKEWORD(2, 2), &ws);
#endif
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock <= 0)
	{
		cout << "create socket failed!" << endl;
		return -1;
	}

	// �]�m���s������
	int opt = 1;
	setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&opt, sizeof(opt));

	sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = INADDR_BROADCAST; // inet_addr("127.0.0.1");//htonl(0);

	int len = sendto(sock, "12345", 6, 0, (sockaddr*)&saddr, sizeof(saddr));
	cout << "sento size is " << len << endl;
	char buf[1024] = { 0 };
	recvfrom(sock, buf, sizeof(buf) - 1, 0, 0, 0);
	cout << buf << endl;
	getchar();


}