// ChatCaNhan.cpp 
// -> giao thuc [id][msg]
// [id] xac dinh chat nhom hay ca nhan, [msg] la dl gui di

#include <stdio.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32")

DWORD WINAPI ClientThread(LPVOID);

void RemoveSocket(SOCKET client);

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
    char buf[256], cmd[32];
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

        // doc buf[] tu ki tu dau tien va luu vao cmd[]
        sscanf(buf, "%s", cmd);
        if (strcmp(cmd, "all") == 0) // all -> chat group
        {
            // server gui cho cac client sau khi nhan dc dl tu mot client nao do
            // vd xay ra: client gui dl di cung se dc server gui lai dl do chinh client do gui
            for (int i = 0; i < numClient; i++)
            {
                if (clients[i] != client) //giai quyet: server gui lai dl ma client1 gui di nhung ko gui cho client1
                {
                    char* msg = buf + strlen(cmd) + 1;// buf+strlen(cmd)+1 -> chi gui ki tu sau tu all
                    send(clients[i], msg, strlen(buf), 0);
                
                }   
            }
        }
        else // chat ca nhan
        {
            int id = atoi(cmd); // char -> int
            for (int i = 0; i < numClient; i++)
            {
                if (clients[i] == id) //giai quyet: server gui lai dl ma client1 gui di nhung ko gui cho client1
                {
                    char* msg = buf + strlen(cmd) + 1;// buf+strlen(cmd)+1 -> chi gui ki tu sau tu all
                    send(clients[i], msg, strlen(buf), 0);

                }
            }
        }
    }
    closesocket(client);
}
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
