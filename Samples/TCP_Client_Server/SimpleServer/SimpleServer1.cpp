// Tao server don gian chap nhan ket noi
// cmd -> cd netcat -> nc64.exe -v 127.0.0.1 8000 (client)

#include <stdio.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

int main()
{
    WSADATA wsa; // Khai bao thu vien Winsock
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  // Tao doi tuong socket

    // Khai bao dia chi cua server
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(8000);
    
    bind(listener, (SOCKADDR*)&addr, sizeof(addr));// Gan dia chi vao socket

    listen(listener, 5); // Chuyen sang trang thai cho ket noi
    printf("Dang cho ket noi moi...\n");
    
    // Truong hop 1: ket noi den server khong quan den dia chi client
    SOCKET client = accept(listener, NULL, NULL);
    printf("Chap nhan ket noi moi: %d", client);

    // Truong hop 2: ket noi den server can biet dia chi client
    SOCKADDR_IN clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    SOCKET client = accept(listener, (SOCKADDR *)&clientAddr, &clientAddrLen);
    printf("Chap nhan ket noi moi: %d\n", client);
    printf("Dia chi client: %s\n", inet_ntoa(clientAddr.sin_addr));

    return 0;
}
