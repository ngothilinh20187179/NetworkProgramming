// server TCP nhan cau chao tu client, nhap du lieu tu ban phim va gui cho client
// cd netcat
// nc64.exe -v 127.0.0.1 8000

#include <stdio.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32")

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

    char buf[256];
    printf("Dang cho du lieu\n");
    int r = recv(client, buf, sizeof(buf), 0);
    if (r <= 0)
    {
        // Ket noi da bi ngat
        closesocket(client);
        closesocket(listener);
        WSACleanup();
        return 0;
    }

    // Neu co du lieu thi them ky tu ket thuc xau
    buf[r] = 0;
    printf("From client: %s\n", buf);
    // lient tuc nhap xau va gui sang client
    while (true)
    {
        printf("Nhap xau: ");
        gets_s(buf, sizeof(buf));
        send(client, buf, strlen(buf), 0);
        send(client, "\n", 1, 0);

        // Neu chuoi nhap vao la "exit" thi thoat khoi vong lap
        if (strcmp(buf, "exit") == 0)
            break;
    }
    closesocket(client);
    closesocket(listener);
    WSACleanup();
}
