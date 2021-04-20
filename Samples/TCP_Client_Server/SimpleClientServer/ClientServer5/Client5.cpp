// Client5.cpp : Tao client gui file den server
//

#include <stdio.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>

#pragma comment(lib, "ws2_32")

int main()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(8000);

    system("pause");

    int ret = connect(client, (SOCKADDR*)&addr, sizeof(addr));
    if (ret == SOCKET_ERROR)
    {
        ret = WSAGetLastError();
        printf("Connect failed: %d\n", ret);
        return 1;
    }

    char buf[2048];

    // Doc file va gui sang server

    FILE* f = fopen("c:\\test_client\\test.mp3", "rb");
    while (1)
    {
        ret = fread(buf, 1, sizeof(buf), f);
        if (ret <= 0)
            break;
        send(client, buf, ret, 0);
        printf("Sent: %d bytes\n", ret);
    }
    fclose(f);

    closesocket(client);
    WSACleanup();
}
