// 4.3Server1tr154.cpp
// Server chap nhan nhieu ket noi va xly du lieu

#include <iostream>
#include <stdio.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32")

int main()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY); // htonl - 4byte (little -> big)
	addr.sin_port = htons(9000);// htons (2byte little -> big)

	SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	bind(listener, (SOCKADDR*)&addr, sizeof(addr));
	listen(listener, 5);


	fd_set fdread;
	int ret;

	SOCKET clients[64];
	int numClient = 0;
	char buf[256];

	while (1) {

		FD_ZERO(&fdread);

		FD_SET(listener, &fdread);

		for(int i=0;i<numClient;i++)
			FD_SET(clients[i], &fdread);

		ret = select(0, &fdread, 0, 0, 0);

		if (ret < 0) {
			break;
		}
		if (FD_ISSET(listener, &fdread)) // ktr xem xra sk ko
		{
			SOCKET client = accept(listener, 0, 0);
			const char* msg = "Hello client\n";
			send(client, msg, strlen(msg), 0);
			// khi socket dc acc thi them vao mang
			printf("New client connect %d\n", client);
			clients[numClient] = client;
			numClient++;
		}

		for (int i = 0; i < numClient; i++)
		{
			if (FD_ISSET(clients[i], &fdread))
			{
				ret = recv(clients[i], buf, sizeof(buf));
				if (ret <= 0)
				{
					// ket noi bi huy
					// xoa client[i] ra khoi mang
					continue; // ktr tung client
				}
				buf[ret] = 0;
				printf("Data client %d: %s\n",clients[i], buf);
			}
		}
	}
}
