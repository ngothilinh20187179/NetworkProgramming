// 3. Tao client gui xau ky tu den server
// chuot phai vao Solution -> Set Startup Projects -> chon server3 va client 3

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

    system("pause");// khi chay cung luc server 3 va client 3 thi them lenh system-> client dung lai de server khoi tao 

    int ret = connect(client, (SOCKADDR*)&addr, sizeof(addr));
    if (ret == SOCKET_ERROR)
    {
        ret = WSAGetLastError();
        printf("Connect failed: %d\n", ret);
        return 1;
    }

    char buf[256];

    // Nhap du lieu tu ban phim va gui cho server
    while (1)
    {
        printf("Nhap xau ky tu: ");
        gets_s(buf, sizeof(buf));

        send(client, buf, strlen(buf), 0);

        if (strcmp(buf, "exit") == 0)
            break;
    }

    closesocket(client);
    WSACleanup();
}
