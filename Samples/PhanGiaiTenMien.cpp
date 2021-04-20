// Phan giai ten mien

#include <stdio.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

int main()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    // Phan giai ten mien
    addrinfo* info;
    int result = getaddrinfo("hust.edu.vn", "http", NULL, &info);
    if (result == 0)
    {
        SOCKADDR_IN addr;
        memcpy(&addr, info->ai_addr, info->ai_addrlen);
        printf("Dia chi IP: %s", inet_ntoa(addr.sin_addr));
    }
    else
    {
        printf("Khong phan giai duoc ten mien");
    }

    WSACleanup();
}
