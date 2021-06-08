// HttpServer1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <stdio.h>

#include <winsock2.h>

#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)


DWORD WINAPI ClientThread(LPVOID);

int main()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(9000);

	SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	bind(listener, (SOCKADDR*)&addr, sizeof(addr));
	listen(listener, 5);

	while (1)
	{
		SOCKET client = accept(listener, NULL, NULL);
		CreateThread(0, 0, ClientThread, &client, 0, 0);
	}
}

DWORD WINAPI ClientThread(LPVOID lpParam)
{
	SOCKET client = *(SOCKET*)lpParam;

	char req[2048];
	int ret = recv(client, req, sizeof(req), 0);
	req[ret] = 0;
	printf("%s\r\n", req);

	char method[8], uri[256];
	sscanf(req, "%s%s", method, uri);

	if (strcmp(uri, "/test_page") == 0)
	{
		// Send header part
		char header[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
		send(client, header, strlen(header), 0);

		// Send body part
		FILE* f = fopen("c:\\test\\test.html", "rb");
		char fbuf[2048];
		while (!feof(f))
		{
			ret = fread(fbuf, 1, sizeof(fbuf), f);
			send(client, fbuf, ret, 0);
		}
	}
	else if (strcmp(uri, "/test_photo") == 0)
	{
		// Send header part
		char header[] = "HTTP/1.1 200 OK\r\nContent-Type: image/jpeg\r\n\r\n";
		send(client, header, strlen(header), 0);

		// Send body part
		FILE* f = fopen("c:\\test\\test.jpg", "rb");
		char fbuf[2048];
		while (!feof(f))
		{
			ret = fread(fbuf, 1, sizeof(fbuf), f);
			send(client, fbuf, ret, 0);
		}
	}
	else if (strcmp(uri, "/test_audio") == 0)
	{
		// Send header part
		char header[] = "HTTP/1.1 200 OK\r\nContent-Type: audio/mp3\r\n\r\n";
		send(client, header, strlen(header), 0);

		// Send body part
		FILE* f = fopen("c:\\test\\test.mp3", "rb");
		char fbuf[2048];
		while (!feof(f))
		{
			ret = fread(fbuf, 1, sizeof(fbuf), f);
			send(client, fbuf, ret, 0);
		}
	}
	else
	{
		//char res[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><head><title>Test Page</title></head><body><h1>Hello World</h1><h3><a href='/test_page'>Test Page</a></h3><h3><a href='/test_photo'>Test Photo</a></h3><h3><a href='/test_audio'>Test Audio</a></h3></body></html>";
		char res[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body>3</body></html>";
		send(client, res, strlen(res), 0);
	}
	
	closesocket(client);
	return 0;
}
