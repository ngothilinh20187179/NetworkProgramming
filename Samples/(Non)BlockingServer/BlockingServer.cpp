// Blocking sever
// Tao server chap nhan nhieu ket noi, nhan dl tu cac client
// (Cac Socket hd o che do blocking, nen cac ham vao ra phai cho lan nhau
// Run (client) cd netcat -> nc64.exe -v 127.0.0.1 9000

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

	SOCKET clients[64];
	int numClients = 0;
	int result;
	char buffer[256];
	while (1)
	{
		printf("Dang cho ket noi....\n");
		SOCKET client = accept(listener, NULL, NULL);
		printf("Chap nhan ket noi >_< : %d\n", client);

		// luu socket vua accept vao mang
		clients[numClients] = client;
		numClients++;

		// Nhan du lieu tu cac client trong mang
		for (int i = 0; i < numClients; i++)
		{
			printf("Dang cho dl tu client %d...\n", clients[i]);
			result = recv(clients[i], buffer, sizeof(buffer), 0);
			if (result > 0)
			{
				buffer[result] = 0;
				printf("Data from client %d: %s\n", clients[i], buffer);
			}
		}
	}
}
