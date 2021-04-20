// client ket noi webserver
// 2. Tao client gui lenh GET den website sd http va hien thi kq tra ve (Ket noi den web server, gui yeu cau, nhan du lieu va in ra man hinh)

#include <stdio.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment (lib, "ws2_32")

int main()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
     

    // ko bt dia chi ip cua web server -> phan giai ten mien va luu vao bien addr
    addrinfo* info;
    SOCKADDR_IN addr; 
    int result = getaddrinfo("lebavui.github.io", "http", NULL, &info);
    if (result != 0) // ko phan giai dc ten mien
    {
        printf("Khong phan giai duoc ten mien");
        return 1;
    }
    memcpy(&addr, info->ai_addr, info->ai_addrlen); // phan giai thanh cong luu kq vao addr
    printf("Dia chi IP: %s\n", inet_ntoa(addr.sin_addr));


    // connect len server
    int ret = connect(client, (SOCKADDR*)&addr, sizeof(addr));
    if (ret == SOCKET_ERROR) 
    {
        ret = WSAGetLastError();
        printf("Connect failed: %d\n", ret);
        return 1;
    }

    // gui sau chao len web server
    char buf[256] = "GET /network_programming/ HTTP/1.1\nHost: lebavui.github.io\n\n";
    send(client, buf, strlen(buf), 0);
    while (1)
    {
        ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0) {
            break;
        }
        if (ret < sizeof(buf))
        {
            buf[ret] = 0;
        }
        printf("Received: %s\n", buf);
    }
    closesocket(client);
    WSACleanup();
}
