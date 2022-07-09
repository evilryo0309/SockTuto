#pragma once
#include "XTcp.h"
#include "XHttpResponse.h"
class XHttpClient
{
public:
	XHttpClient();
	~XHttpClient();
	bool Start(XTcp client);
	void Main();
	XTcp client;
	XHttpResponse res;
};

