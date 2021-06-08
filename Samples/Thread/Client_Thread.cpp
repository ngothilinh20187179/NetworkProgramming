// Client vua nhan dl tu ban phim vua nhan dl tu server va in ra man hinh (server gui nhan dl dong thoi)
// cd netcat
// nc64.exe -v -l -p 9000

#include <stdio.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#pragma comment(lib, "ws2_32")

// nhan dl tu server thong qua socket va in ra man hinh
DWORD WINAPI ReceiverThread(LPVOID lpParameter);
SOCKET _client;

int main()
{
    // Khoi tao WinSock
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    // Tao socket
    _client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(9000);

    // connect server
    connect(_client, (SOCKADDR*)&addr, sizeof(addr));

    CreateThread(0, 0, ReceiverThread, 0, 0, 0);
    char buf[256]; 
    while (true)
    {
        printf("Nhap chuoi kt: ");
        gets_s(buf, sizeof(buf)); // doc buf
        send(_client, buf, strlen(buf), 0); // gui buf len server
    }
   
}
DWORD WINAPI ReceiverThread(LPVOID lpParameter)
{
    char buf[256];
    int ret;
    while (true)
    {
        ret = recv(_client, buf, sizeof(buf), 0);
        if (ret <= 0)
        {
            break;
        }
        buf[ret] = 0;
        printf("Data: %s\n", buf);
    }
    closesocket(_client);
}
