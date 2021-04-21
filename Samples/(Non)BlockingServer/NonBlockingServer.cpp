// NonBlockingServer 
//  Tao Server chap nhan nhieu ket noi, nhan dl tu cac client
// Socket hd che do non-blocking, cac ham vao ra tra ve kq ngay lap tuc, kp cho doi lan nhau
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
	addr.sin_addr.s_addr = htonl(INADDR_ANY); // htonl (4byte little->big)
	addr.sin_port = htons(9000); // htons (2byte little -> big)

	SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	// chuyen socket sang che do bat dong bo
	unsigned long ul = 1;
	int result = ioctlsocket(listener, FIONBIO, (unsigned long*)&ul);
	if (result == SOCKET_ERROR)
	{
		return 1;
	}
	bind(listener, (SOCKADDR*)&addr, sizeof(addr));
	listen(listener, 5);

	char buffer[256];
	SOCKET clients[64];
	int numClients = 0;

	while (1)
	{
		SOCKET client = accept(listener, NULL, NULL);
		if (client != INVALID_SOCKET) {
			printf("Chap nhan ket noi %d\n", client);
			clients[numClients] = client;
			numClients++;
		}
		else
		{
			result = WSAGetLastError();
		}
		for (int i = 0; i < numClients; i++)
		{
			// nhan dl
			result = recv(clients[i], buffer, sizeof(buffer), 0);
			if (result > 0)
			{
				buffer[result] = 0;
				printf("Data from client %d: %s\n", clients[i], buffer);
			}
			else if(result == SOCKET_ERROR)
			{
				result = WSAGetLastError();
				if (result == WSAEWOULDBLOCK) {
					// bo qua loi
				}
				else
				{
					//xly loi
					// ket noi bi dong
				}
			}
			else // result == 0
			{
				// ket noi bi dong
			}

		}
	}
}
