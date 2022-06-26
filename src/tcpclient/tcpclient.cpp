#include "XTcp.h"
int main()
{
	XTcp client;
	client.CreateSocket();
	client.SetBlock(false);
	client.Connect("127.0.0.1", 8080, 3000);
	client.Send("client", 6);
	char buf[1024] = { 0 };
	client.Recv(buf, sizeof(buf));
	printf("%s\n", buf);
	getchar();
	return 0;
}
