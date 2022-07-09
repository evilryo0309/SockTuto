#pragma once
#include "XTcp.h"

class XHttpServer
{
public:
	bool Start(unsigned short port);
	XHttpServer();
	~XHttpServer();
	XTcp server;
	void Main();
	void Stop();
	bool isexit = false;
};
