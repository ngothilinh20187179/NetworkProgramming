// UDPsender.cpp: truyen dl sd UDP
// gui xau toi dia dia chi 127.0.0.1


#include <stdio.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#pragma comment(lib, "ws2_32")


int main()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    // tao socket sender de gui tin di
    SOCKET sender = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(8080);

    char buf[256] = "Hello World";
    sendto(sender, buf, strlen(buf), 0, (SOCKADDR*)&addr, sizeof(buf));
    WSACleanup();
}
