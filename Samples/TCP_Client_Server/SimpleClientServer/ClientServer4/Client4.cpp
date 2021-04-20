// Client4.cpp : Tao client gui so thuc float den server
// chay cung server 4
// chuot phai vao Solution -> Set Startup Projects -> chon server3 va client 3

#include <stdio.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS

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

    // Gui du lieu cau truc sang server
    SV sv;
    sv.mssv = 20186789;
    strcpy(sv.hoten, "Nguyen Van A");
    strcpy(sv.lop, "CNTT");
    sv.diem = 9.0;
    send(client, (char*)&sv, sizeof(sv), 0);
    
    float f;

    // Nhap s thuc tu ban phim va gui cho server
    while (1)
    {
        printf("Nhap so thuc: ");
        scanf("%f", &f);
        
        send(client, (char *)&f, sizeof(f), 0);

        if (f == 0.0)
            break;
    }

    closesocket(client);
    WSACleanup();
}
