// ChatGroup.cpp : xd ung dung chat group
// khi 1 client gui dl thi tat ca client khac deu nhan dc 
// -> client1 gui dl len server -> server gui lai dl do cho tat ca cac client nhung ko gui lai cho client1 nua

#include <stdio.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32")

// khai bao ham thuc thi
DWORD WINAPI ClientThread(LPVOID);

void RemoveSocket(SOCKET client) // khi co client ngat ket noi thi dung lenh nay de xoa client do di
{
    // tim vi tri can xoa roi chen client cuoi mang clients[] vao
    int i = 0;
    while (clients[i] != client)
        i++;
    if (i < numClient - 1)
        clients[i] = clients[numClient - 1];
    numClient--;
}

SOCKET clients[64];
int numClient;

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

    numClient = 0;
    while (1)
    {
        SOCKET client = accept(listener, NULL, NULL);
        printf("New client: %d\n", client);

        // them client vao mang, tang so luong client len
        clients[numClient] = client;
        numClient++;

        // khoi tao luong moi chay ham thuc thi ClientThread
        CreateThread(0, 0, ClientThread, &client, 0, 0);
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
        {
            RemoveSocket(client);
            break;
        }

        // in dl ra man hinh cua server
        buf[ret] = 0;
        printf("Data: %s\n", buf); 

        // server gui cho cac client sau khi nhan dc dl tu mot client nao do
        // vd xay ra: client gui dl di cung se dc server gui lai dl do chinh client do gui
        for (int i = 0; i < numClient; i++)
        {
            if(clients[i] != client) //giai quyet: server gui lai dl ma client1 gui di nhung ko gui cho client1
                send(clients[i], buf, strlen(buf), 0);
        }
    }
    closesocket(client);
}
