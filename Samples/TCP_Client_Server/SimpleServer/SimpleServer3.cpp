//Tao server, gui dl den netcat client
// cd netcat
// nc64.exe -v 127.0.0.1 8000

#include <stdio.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

typedef struct {
    int mssv;
    char hoten[32];
    char lop[8];
    float diemthi;
}SINHVIEN;

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
    printf("Dang cho ket noi moi...\n");

    // Truong hop 1: khong quan den dia chi client
    /*SOCKET client = accept(listener, NULL, NULL);
    printf("Chap nhan ket noi moi: %d", client);*/

    // Truong hop 2: can biet dia chi client
    SOCKADDR_IN clientAddr;
    int clientAddrLen = sizeof(clientAddr);

    SOCKET client = accept(listener, (SOCKADDR*)&clientAddr, &clientAddrLen);
    printf("Chap nhan ket noi moi: %d\n", client);
    printf("Dia chi client: %s\n", inet_ntoa(clientAddr.sin_addr));


    // Gui chuoi ky tu
    /*const char* str = "Hello World";
    int r = send(client, str, strlen(str), 0);
    printf("Result: %d\n", r);
    if (r == SOCKET_ERROR)
    {
        r = WSAGetLastError();
        printf("Error code: %d\n", r);
    }*/


    // Gui buffer chua du lieu
    /*char buf[256];
    for (int i = 0; i < 10; i++)
        buf[i] = i + 'A';
    int r = send(client, buf, 10, 0);
    printf("Result: %d\n", r);*/


    /*float f = 3.1415926;
    send(client, (char*)&f, sizeof(f), 0);*/


    /*SINHVIEN sv;
    send(client, (char*)&sv, sizeof(sv), 0);*/


   /*printf("Dang cho du lieu\n");
    char buf[256];
    int  r = recv(client, buf, sizeof(buf), 0);
    printf("Result: %d\n", r);
    // lenh printf in tu dau buf cho den khi gap ki tu kthuc xau, neu ko gap ki tu do no se in ra het 256byte
    // them ky tu ket thuc buf[r] = 0;, r la so byte nhan dc
    buf[r] = 0;
    printf("Data: %s", buf);*/
    
    
    /*char buf[16] 
    // kich thuoc buf nho nen dung vong while de nhan lien tuc tranh mat mat du lieu
    // moi lan nhan chi nhan dc 16byte 
    int r;
    while (true)
    {
        r = recv(client, buf, sizeof(buf), 0);
        printf("Result: %d\n", r);
        if (r < 0) // 
            break;
        if (r < sizeof(buf))
            buf[r] = 0;
        printf("Data: %s", buf);
    }*/
    return 0;
}
