#include "XTcp.h"
int main()
{
	XTcp client;
	client.Connect("127.0.0.1", 8080);
	client.Send("client", 6);
	char buf[1024] = { 0 };
	client.Recv(buf, sizeof(buf));
	printf("%s\n", buf);
	return 0;
}
