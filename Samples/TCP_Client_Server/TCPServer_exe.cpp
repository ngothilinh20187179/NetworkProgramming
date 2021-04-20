// B2:tr111
// Viết chương trình TCPServer, đợi kết nối ở cổng xác định bởi tham số dòng lệnh. 
// Mỗi khi có client kết nối đến, thì gửi xâu chào được chỉ ra trong một tệp tin xác định, 
// sau đó ghi toàn bộ nội dung client gửi đến vào một tệp tin khác được chỉ ra trong tham số dòng lệnh
// TCPServer.exe <Cổng> <Tệp tin chứa câu chào> <Tệp tin lưu nội dung client gửi đến>
// VD: TCPServer.exe 8888 chao.txt client.txt

#include <stdio.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#pragma comment(lib, "ws2_32")

int main(int argc, char* argv[])
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addr; // ipv4
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(atoi(argv[1]));

	bind(listener, (SOCKADDR*)&addr, sizeof(addr)); // gan cau truc dia chi voi socket o tren
	listen(listener, 5); // chuyen socket sang trang thai cho ket noi

	SOCKET client = accept(listener, NULL, NULL);
	char buf[256];

	FILE* f = fopen(argv[2], "r");
	fgets(buf, sizeof(buf), f);
	fclose(f);

	send(client, buf, strlen(buf), 0);
	f = fopen(argv[3], "wb");

	int ret;
	while (1)
	{
		ret = recv(client, buf, sizeof(buf), 0);
		if (ret <= 0)
			break;
		fwrite(buf, 1, ret, f);
	}
	closesocket(client);
	closesocket(listener);
	WSACleanup();
