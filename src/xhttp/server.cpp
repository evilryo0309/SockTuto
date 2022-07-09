#include <stdlib.h>
#include "XTcp.h"
#include "XHttpServer.h"
using namespace std;
#include <signal.h>
int main(int argc, char* argv[])
{
	//signal(SIGPIPE, SIG_IGN);

	unsigned short port = 8080;
	if (argc > 1)
	{
		port = atoi(argv[1]);
	}
	XHttpServer server;
	server.Start(port);
	getchar();
	return 0;
}
