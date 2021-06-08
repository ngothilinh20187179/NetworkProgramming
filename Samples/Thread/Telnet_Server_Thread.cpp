// TelnetServer.cpp 
// - ket noi vs 1 client, y/c client gui user va pass, ssanh voi file CSDL la mot file text(moi dong chua mot cap user+pass)
//   + fail -> bao loi dang nhap
//   + true -> doi lenh tu client, thuc hien lenh va tra ve kq cho client
// - dung ham system ("dir c:\temp > c:\\temp\\out.txt") de thuc hien lenh
//   + dir c:\temp la vd lenh dir ma client gui
//   + > c:\\temp\\out.txt de dinh huong lai dl ra tu lenh dir, khi do kq lenh dir se dc ghi vao file van ban

#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <string.h>
#pragma comment(lib, "ws2_32")
#define _CRT_SECURE_NO_WARNINGS

DWORD WINAPI ClientThread(LPVOID lpParam);

SOCKET clients[64];
int numClient = 0;

int main()
{
    // Khoi tao WinSock
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // dia chi server
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY); // htonl: 4byte little -> big
    addr.sin_port = htons(9000); // htons: 2byte little -> big

    bind(listener, (SOCKADDR*)&addr, sizeof(addr)); // gan socket vao mot giao dien mang cua may
    listen(listener, 5); // chuyen socket sang trang thai cho ket noi

    while (1)
    {
        SOCKET client = accept(listener, NULL, NULL); // nhan ket noi tu client
        printf("New client: %d\n", client);

        // khoi tao luong moi chay ham thuc thi ClientThread
        CreateThread(0, 0, ClientThread, &client, 0, 0);
    }
}
DWORD WINAPI ClientThread(LPVOID lpParam) 
{
    SOCKET client = *(SOCKET*)lpParam;
    
    const char* msg = "Nhap tai khoan va password theo cu phap 'user password'\n";
    send(client, msg, strlen(msg), 0);

    int ret;
    char buf[256], fbuf[256];

    char user[32], pass[32], tmp[32];
    while (1)
    {   // nhan cau trl ve user va pass; tach buf ra user,pass,tmp; so sanh vs dl trong file txt
        ret = recv(client, buf, sizeof(buf), 0);
        if (ret < 0)
        {
            closesocket(client);
            return 1;
        }
        buf[ret] = 0;
        int n = sscanf(buf, "%s %s %s", user, pass, tmp);

        //nhap dung cu phap user pass
        if (n == 2) 
        {
            // So sanh voi CSDL trong file UserPass.txt
            FILE* f = fopen("C:\\Users\\ngoth\\Documents\\NetworkProgramming\\NetworkProgramming\\Thread\\UserPass.txt", "r");
            int foundAccount = 0;
            while (!feof(f))
            {
                // Doc tung dong trong f va luu vao fbuf
                fgets(fbuf, sizeof(buf), f);/*Doc 1 dong: char *fgets(char *str, int n, FILE *stream)*/
                // So sanh tung dong trong UserPass.txt voi buf
                if (strcmp(buf, fbuf) == 0)
                {
                    foundAccount = 1;
                    break;
                }
            }
            fclose(f);
            if (foundAccount == 1)
            {
                // Tim thay tai khoan
                const char* msg = "Dang nhap thanh cong. Hay nhap lenh de thuc hien.\n";
                send(client, msg, strlen(msg), 0);
                break;
            }
            else
            {
                // Sai tai khoan
                const char* msg = "Sai tai khoan. Hay nhap lai.\n";
                send(client, msg, strlen(msg), 0);
            }
        }
        else
        {
            // Sai cu phap
            const char* msg = "Sai cu phap. Hay nhap lai.\n";
            send(client, msg, strlen(msg), 0);
        }
    }
    while (1)
    {
        ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
        {
            closesocket(client);
            return 1;
        }
        // Xoa ky tu xuong dong
        buf[ret - 1] = 0;

        char cmd[256];
        sprintf(cmd, "%s > C:\\Users\\ngoth\\Documents\\NetworkProgramming\\NetworkProgramming\\Thread\\OutPut.txt", buf);
        system(cmd);

        // Doc file ket qua va gui cho client
        FILE* f = fopen("C:\\Users\\ngoth\\Documents\\NetworkProgramming\\NetworkProgramming\\Thread\\OutPut.txt", "r");
        while (!feof(f))
        {
            fgets(fbuf, sizeof(buf), f);
            send(client, fbuf, strlen(fbuf), 0);
        }
        fclose(f);
    }
    return 0;
}
