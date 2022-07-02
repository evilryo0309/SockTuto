#include <stdlib.h>
#include "XTcp.h"
#include <thread>
#include <string.h>
#include <string>
#include <regex>
using namespace std;
class HttpThread
{
public:
	void Main()
	{
		char buf[10000] = { 0 };
		for (;;)
		{
			//接受 http 客戶端請求
			int recvLen = client.Recv(buf, sizeof(buf) - 1);
			if (recvLen <= 0)
			{
				Close();
				return;
			}
			buf[recvLen] = '\0';
			printf("========recv========\n%s====================\n", buf);

			//GET /index.html HTTP/1.1
			//Host: 172.21.104.140:1234
			//Connection: keep-alive
			//User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/103.0.0.0 Safari/537.36
			//Accept: image/avif,image/webp,image/apng,image/svg+xml,image/*,*/*;q=0.8
			//Referer: http://172.21.104.140:1234/
			//Accept-Encoding: gzip, deflate
			//Accept-Language: zh-TW,zh;q=0.9,en-US;q=0.8,en;q=0.7
			string src = buf;
			string pattern = "^([A-Z]+) (.+) HTTP/1";
			regex r(pattern);
			smatch mas;
			regex_search(src, mas, r);
			if (mas.size() == 0)
			{
				printf("%s failed!\n", pattern.c_str());
				Close();
				return;
			}
			string type = mas[1];
			string path = mas[2];
			if (type != "GET")
			{
				Close();
				return;
			}
			string filename = path;
			if (path == "/")
			{
				filename = "/index.html";
			}

			string filepath = "www";
			filepath += filename;
			FILE* fp = fopen(filepath.c_str(), "rb");
			if (fp == NULL)
			{
				Close();
				return;
			}
			//獲取文件大小
			fseek(fp, 0, SEEK_END);
			int filesize = ftell(fp);
			fseek(fp, 0, 0);
			printf("file size is %d\n", filesize);

			// 回應 http GET 請求
			// 消息頭
			string rmsg = "";
			rmsg = "HTTP/1.1 200 OK\r\n";
			rmsg += "Server: XHttp\r\n";
			rmsg += "Content-Type: text/html\r\n";
			rmsg += "Content-Length: ";
			char bsize[128] = { 0 };
			sprintf(bsize, "%d", filesize);
			rmsg += bsize;
			//rmsg += "10\r\n";
			rmsg += "\r\n\r\n";
			//rmsg += "0123456789";
			//發送消息頭
			int sendSize = client.Send(rmsg.c_str(), rmsg.size());
			printf("sendsize = %d\n", sendSize);
			printf("=======send=======\n%s\n================\n", rmsg.c_str());

			//發送正文
			for (;;)
			{
				int len = fread(buf, 1, sizeof(buf), fp);
				if (len <= 0)break;
				int re = client.Send(buf, len);
				if (re <= 0)break;
			}
		}
		Close();
	}
	void Close()
	{
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
		HttpThread* th = new HttpThread();
		th->client = client;
		std::thread sth(&HttpThread::Main, th);
		sth.detach();
	}
	server.Close();

	getchar();
	return 0;
}
