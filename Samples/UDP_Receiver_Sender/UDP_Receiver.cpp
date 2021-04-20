// UDPreceiver.cpp : nhan du lieu datagram tu cong 8080 va in ra man hinh

#include <stdio.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#pragma comment(lib, "ws2_32")


int main()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    // tao socket UDP
    SOCKET receiver = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(8080);

    // bind socket vao tat ca cac giao dien va cong 8080
    bind(receiver, (SOCKADDR*)&addr, sizeof(addr));

    int ret;
    char buf[256];
    while (1)
    {
        ret = recvfrom(receiver, buf, 10, 0, NULL, NULL);
        if (ret > 0)
        {
            if (ret < sizeof(buf)) {
                buf[ret] = 0;
            }
            printf("Data: %s\n", buf);
        }
        else
        {
            ret = WSAGetLastError();
            printf("Error: %d\n", ret);
        }
     }
}
