// Server4.cpp : Nhan so thuc tu client va in ra man hinh
// chay cung client 4

#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32")

typedef struct {
    int mssv;
    char hoten[32];
    char lop[16];
    float diem;
} SV;

int main()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(8000);

    bind(listener, (SOCKADDR*)&addr, sizeof(addr));
    listen(listener, 5);

    printf("Dang cho ket noi\n");
    SOCKET client = accept(listener, NULL, NULL);

    printf("Chap nhan ket noi: %d\n", client);

    float f;
    int ret;

    // Nhan du lieu cau truc tu client
    SV sv;
    ret = recv(client, (char*)&sv, sizeof(sv), 0);

    // Nhan so thuc tu client
    while (1)
    {
        ret = recv(client, (char *)&f, sizeof(f), 0);
        if (ret <= 0)
            break;

        printf("Received: %f\n", f);
    }

    closesocket(client);
    closesocket(listener);
    WSACleanup();
}
