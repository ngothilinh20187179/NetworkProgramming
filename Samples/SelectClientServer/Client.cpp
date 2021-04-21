// 4.3Clienttr154.cpp 
/*int select(
    int nfds,               // Không sử dụng
    fd_set FAR* readfds,   // Tập các socket hàm sẽ thăm dò cho sự kiện read
    fd_set FAR* writefds,  // Tập các socket hàm sẽ thăm dò cho sự kiện write
    fd_set FAR* exceptfds, // Tập các socket hàm sẽ thăm dò cho sự kiện except
    const struct timeval FAR* timeout // Thời gian thăm dò tối đa
);*/
// client nhan dl tu server va in ra man hinh

#include <stdio.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#pragma comment (lib, "ws2_32")

int main()
{
    // Khoi tao WinSock
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    // Tao socket
    SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Khai bao bien luu dchi server
    SOCKADDR_IN addr;
    // Ho dia chi la IPv4 -> AF_INET
    addr.sin_family = AF_INET;
    // Chuyen xau dia chi ip 127.0.0.1 sang so 4 byte dang network-byte order, roi gan cho truong sin_addr
    addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 127.0.0.1 goi la dchi loopback (reserved ip)
    // Chuyen doi cong sang dang network-byte order roi gan cho truong sin_port
    addr.sin_port = htons(9000);

    connect(client, (SOCKADDR*)&addr, sizeof(addr));
    
    
    fd_set fdread;
    int ret;
    char buf[256];

    timeval tv; // sau khoang tg tv ma ko truyen dl thi time out
    tv.tv_sec = 5;
    tv.tv_usec = 0;

    while (1) {
        // Xoa tap fdread
        FD_ZERO(&fdread);
        // Thêm socket vào tập fdread
        FD_SET(client, &fdread);
        // Đợi sự kiện trên socket
        ret = select(0, &fdread, 0, 0, 0);
        if (ret < 0) {
            break;
        }
        if (ret == 0) {
            printf("Time out\n");
            continue;
        }
        if (FD_ISSET(client, &fdread)) 
        { // ktra xem co xay ra su kien ko
            ret = recv(client, buf, sizeof(buf));
            if (ret <= 0) {
                break;
            }
            if (ret < sizeof(buf))
            {
                buf[ret] = 0;
            }
            printf("Received: %s\n", buf);
        }
}
