// Tao server chap nhan nhieu ket noi

#include <stdio.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32")

// khai bao ham thuc thi
DWORD WINAPI ClientThread(LPVOID);

int main()
{
    // Khoi tao WinSock
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
     
    SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    //dia chi server
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(9000);

    bind(listener, (SOCKADDR*)&addr, sizeof(addr));
    listen(listener, 5);

    while (1)
    {
        SOCKET client = accept(listener, NULL, NULL);
        printf("New client: %d\n", client);
        // khoi tao luong moi chay ham thuc thi ClientThread
        CreateThread(0, 0, ClientThread, &client, 0,0);
    }
}
DWORD WINAPI ClientThread(LPVOID lpParam) // lpParam chua dia chi
{
    SOCKET client = *(SOCKET*)lpParam;
    char buf[256];
    int ret;
    while (1)
    {
        ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
            break;
        buf[ret] = 0;
        printf("Data: %s\n", buf);
    }
    closesocket(client);
}
