// ChatServerBtnv.cpp: Sd mo hinh da luong
// - nhan ket noi tu client, vao vong lap hoi ten (client_id) cho toi khi client gui dung cu phap
// - vao vong lap nhan dl tu 1 client, gui dl do toi cac client khac
//   + abc: xin chao
//   + 2021/04/13 10:14:00PM abc: xin chao

#include <stdio.h>
#include <WinSock2.h>
#include <string.h>
#pragma comment(lib, "ws2_32")
#define _CRT_SECURE_NO_WARNINGS

DWORD WINAPI ClientThread(LPVOID lpParam);
SOCKET clients[64];
int numClient = 0;

int main()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);//htonl function takes a 32-bit number in host byte order and returns a 32-bit number in the network byte
	addr.sin_port = htons(9000);//htons function takes a 16-bit number in host byte order and returns a 16-bit number in network byte 

	SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	bind(listener, (SOCKADDR*)&addr, sizeof(addr));
	listen(listener, 5);

	numClient = 0;
	while (1)
	{
		SOCKET client = accept(listener, 0, 0);
		printf("New client connected: %d\n", client);
		CreateThread(0, 0, ClientThread, &client, 0, 0);
	}
}
DWORD WINAPI ClientThread(LPVOID lpParam)
{
	SOCKET client = *(SOCKET*)lpParam;

    //server hoi id cua client
    char askId[] = "Nhap ten dung cu phap sau 'client_id: xxxx':   ";
    send(client, askId, strlen(askId), 0); /* gui cau hoi id toi client moi khi co client ket noi */
    char buf[256]; /* luu cau trl id cua client*/
    int ret;

    char cmd[32], id[32], tmp[32]; /*cmd (luu 'client_id:'), id (luu 'xxxx'), tmp luu ki tu sau xxxx*/

    // vao vong lap hoi id cua client cho toi khi dung cu phap
    while (1)
    {
        ret = recv(client, buf, sizeof(buf), 0); /*server nhan thong tin id cua client*/
        if (ret <= 0)
        {
            closesocket(client);
            return 1;
        }
        buf[ret] = 0;
        int n = sscanf(buf, "%s %s %s", cmd, id, tmp); /*tach cau trl ve id cua client lam 3*/
        // n = 2 -> dung so tu
        if (n == 2)
        {
            // cmd == "client_id:" -> dung cu phap, them client vao mang clients[]
            if (strcmp(cmd, "client_id:") == 0)
            {
                const char* msg = "Dung cu phap. Nhap tin nhan de chuyen tiep\n";
                send(client, msg, strlen(msg), 0);
                clients[numClient] = client;
                numClient++;
                break;   
            }
            // cmd != "client_id: xxxx" -> sai cu phap
            else
            {
                const char* msg = "Sai cu phap. Nhap lai!\n";
                send(client, msg, strlen(msg), 0);
            }
        }
        // n != 2 -> sai cu phap 'client_id: xxxx'
        else
        {
            const char* msg = "Sai cu phap. Nhap lai!\n";
            send(client, msg, strlen(msg), 0);
        }
    }

    // vao vong lap nhan dl tu 1 client va gui toi cac client khac
    char msg[] = "Nhap du lieu: \n";
    while (1)
    {
        // server nhan dl tu client
        ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
        {
            closesocket(client);
            return 1;
        }

        buf[ret] = 0;
        char sendbuf[256];
        sprintf(sendbuf, "%s: %s", id, buf);

        for (int i = 0; i < numClient; i++)
        {
            if (client != clients[i])
            {
                send(clients[i], sendbuf, strlen(sendbuf), 0);
            }
        }
    }
}
