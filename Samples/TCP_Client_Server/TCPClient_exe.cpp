// B1: tr 111
// Viết chương trình TCPClient, kết nối đến một máy chủ xác
// định bởi tên miền hoặc địa chỉ IP. Sau đó nhận dữ liệu từ bàn
// phím và gửi đến server. Tham số được truyền vào từ dòng lệnh có dạng
// TCPClient.exe <Địa chỉ IP/Tên miền> <Cổng>

#include <stdio.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#pragma comment(lib, "ws2_32")


int main(int argc, char* argv[])
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // dia chi cua server
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("argv[1]");
    addr.sin_port = htons(atoi(argv[2])); // can truyen port (int) ma argv la char -> ep kieu (atoi)

    connect(client, (SOCKADDR*)&addr, sizeof(addr));

    // Nhan xau chao tu sẻver
    char buf[256];
    int ret = recv(client, buf, sizeof(buf), 0);
    if (ret <= 0)
    {
        printf("Connect failed\n");
        return 1;
    }
    if (ret < sizeof(buf))
    {
        buf[ret] = 0;
    }
    printf("From server: %s", buf);
    
    // Doc du lieu tu ban phim va gui len server
    while (1)
    {
        printf("Nhap xau ky tu: ");
        gets_s(buf, sizeof(buf));

        send(client, buf, strlen(buf), 0);
        if (strcmp(buf, "exit") == 0)
            break;
    }
    closesocket(client);
    WSACleanup();
}
