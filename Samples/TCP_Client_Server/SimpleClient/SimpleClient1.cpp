// 1. Tao Client gui cau chao len netcat server, nhan dl tu server va in ra man hinh ====> B1.tr101
// Run: opend cmd -> cd netcat -> nc64.exe -v -l -p 8000
// Build B1 and Debug

#include <stdio.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#pragma comment (lib, "ws2_32")

int main()
{
    // Khoi tao WinSock
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    // Tao socket
    SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Khai bao bien luu dchi server
    SOCKADDR_IN addr;
    // Ho dia chi la IPv4 -> AF_INET
    addr.sin_family = AF_INET; 
    // Chuyen xau dia chi ip 127.0.0.1 sang so 4 byte dang network-byte order, roi gan cho truong sin_addr
    addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 127.0.0.1 goi la dchi loopback (reserved ip)
    // Chuyen doi cong sang dang network-byte order roi gan cho truong sin_port
    addr.sin_port = htons(8000);

    // Connect server
    int r = connect(client, (SOCKADDR*)&addr, sizeof(addr));
    if (r == SOCKET_ERROR)
    {
        r = WSAGetLastError();
        printf("Loi ket noi:");
        system("pause");
        return 1;
    }

    // Gui du lieu len server
    char buf[256] = "Hello!";
    send(client, buf, strlen(buf), 0);

    // Nhan du lieu tu server
    while (1)
    {
        r = recv(client, buf, sizeof(buf), 0);
        if (r < 0)
        {
            r = WSAGetLastError();
            printf("Connection closed: %d", r);
            break;
        }
        if (r < 256) 
        {
            buf[r] = 0;
        }
        printf("Received: %s\n", buf);
    }
    closesocket(client);
    WSACleanup();
    return 0;
}
