// FTPClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Đăng nhập và thực hiện lệnh LIST

#include <stdio.h>
#include <string.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)
int main()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(21);
    SOCKET controlSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    connect(controlSocket, (SOCKADDR*)&addr, sizeof(addr));
    char buf[2048];
    int ret = recv(controlSocket, buf, sizeof(buf), 0);
    buf[ret] = 0;
    puts(buf);
    // Dang nhap    
    char msg[256];
    char user[32], pass[32];
    while (1)
    {
        printf("Nhap username va password: ");
        scanf("%s%s", user, pass);
      
        // Gui lenh USER        
        sprintf(msg, "USER %s\n", user);
        send(controlSocket, msg, strlen(msg), 0);
        ret = recv(controlSocket, buf, sizeof(buf), 0);
        buf[ret] = 0;
        puts(buf);
      
        // Gui lenh PASS        
        sprintf(msg, "PASS %s\n", pass);
        send(controlSocket, msg, strlen(msg), 0);
        ret = recv(controlSocket, buf, sizeof(buf), 0);
        buf[ret] = 0;
        puts(buf);
      
        // Kiem tra ket qua dang nhap        
        if (strncmp(buf, "230", 3) == 0)
        {
            puts("Dang nhap thanh cong!");
            break;
        }
        else        {
            puts("Dang nhap that bai!");
        }
    }
  
    // Gui lenh PASV    
    strcpy(msg, "PASV\n");
    send(controlSocket, msg, strlen(msg), 0);
    ret = recv(controlSocket, buf, sizeof(buf), 0);
    buf[ret] = 0;
    puts(buf);
  
    // Tach ra dia chi IP va cong    
    char* p1 = strchr(buf, '(');
    char* p2 = strchr(buf, ')');
    char pAdd[64];
    memcpy(pAdd, p1 + 1, p2 - p1);
    pAdd[p2 - p1 - 1] = 0;
    int arrAdd[6], i = 0;
    char* p = strtok(pAdd, ",");
    while (p != NULL)
    {
        arrAdd[i] = atoi(p);
        i++;
        p = strtok(NULL, ",");
    }
  
    // Tao ket noi truyen du lieu    
    SOCKADDR_IN addrData;
    addrData.sin_family = AF_INET;
    addrData.sin_addr.s_addr = inet_addr("127.0.0.1");
    addrData.sin_port = htons(arrAdd[4] * 256 + arrAdd[5]);
    SOCKET dataSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    connect(dataSocket, (SOCKADDR*)&addrData, sizeof(addrData));
  
    // Gui lenh TYPE A    
    strcpy(msg, "TYPE A\n");
    send(controlSocket, msg, strlen(msg), 0);
    ret = recv(controlSocket, buf, sizeof(buf), 0);
    buf[ret] = 0;
    puts(buf);
  
    // Gui lenh LIST    
    strcpy(msg, "LIST\n");
    send(controlSocket, msg, strlen(msg), 0);
    ret = recv(controlSocket, buf, sizeof(buf), 0);
    buf[ret] = 0;
    puts(buf);
  
    // Nhan du lieu thong qua kenh du lieu    
    printf("\n------\n");
    while (1)
    {
        ret = recv(dataSocket, buf, sizeof(buf), 0);
        if (ret <= 0)
            break;
        if (ret < sizeof(buf))
            buf[ret] = 0;
        printf("%s", buf);
    }
    printf("\n------\n");
    closesocket(dataSocket);
  
    // Gui lenh QUIT    
    strcpy(msg, "QUIT\n");
    send(controlSocket, msg, strlen(msg), 0);
    ret = recv(controlSocket, buf, sizeof(buf), 0);
    buf[ret] = 0;
    puts(buf);
    closesocket(controlSocket);
    WSACleanup();
}
