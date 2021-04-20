//server TCP nhan du lieu tu trinh duyet tra lai ket qua theo giao thuc HTTP
// Run : chrome -> http://127.0.0.1:8000

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

    while (true)
    {
        SOCKET client = accept(listener, NULL, NULL);
        char buf[2048];
        int r = recv(client, buf, sizeof(buf), 0);
        if (r < sizeof(buf))
            buf[r] = 0;
        printf("Data:\n%s\n", buf);


        // Tra lai du lieu la 1 trang web hien thi "Hello World"
        const char* http = "HTTP/1.1 200 OK\nContent-type: text/html\n\n<html><body><h1>Hello World</h1></body></html>";
        send(client, http, strlen(http), 0);
        closesocket(client);


        //Tra lai du lieu la 1 file anh JPG
        /*const char* header = "HTTP/1.1 200 OK\nContent-type: text/html\n\n";
        send(client, header, strlen(header), 0);
        FILE* f = fopen("c:\\test\\city.jpg", "rb");
        while (true)
        {
            r = fread(buf, 1, sizeof(buf), f);
            if (r <= 0)
                break;
            send(client, buf, r, 0);
        }
        fclose(f);
        closesocket(client);*/


        // Tra lai du lieu la 1 file MP3
        /*const char* header = "HTTP/1.1 200 OK\nContent-type: audio/mp3\n\n";
        send(client, header, strlen(header), 0);
        FILE* f = fopen("c:\\test\\test.mp3", "rb");
        while (true)
        {
            r = fread(buf, 1, sizeof(buf), f);
            if (r <= 0)
                break;
            send(client, buf, r, 0);
        }
        fclose(f);
        closesocket(client);*/
    }

    return 0;
}
