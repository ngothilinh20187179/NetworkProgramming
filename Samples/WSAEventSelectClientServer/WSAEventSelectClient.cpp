// WSAEventSelectClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS

int main()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(9000);

	SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	connect(client, (SOCKADDR*)&addr, sizeof(addr));

	WSAEVENT newEvent = WSACreateEvent();
	WSAEventSelect(client, newEvent, FD_READ | FD_CLOSE); // có hai sự kiện cần kiểm tra

	int ret;
	WSANETWORKEVENTS networkEvents;
	char buf[256];
	while (1)
	{
		ret = WSAWaitForMultipleEvents(1, &newEvent, TRUE, 5000, FALSE); // chỉ có 1 đối tượng sự kiện cần kiểm tra là newEvent nên truyền '1'
		if (ret == WSA_WAIT_FAILED)
			break;
		if (ret == WSA_WAIT_TIMEOUT)
		{
			printf("Time out\n");
			continue;
		}
		WSAEnumNetworkEvents(client, newEvent, &networkEvents);
		WSAResetEvent(newEvent); // chuyển đối tượng về trạng thái chuông báo hiệu

		// kiểm tra xem sk xảy ra là read hay close
		if (networkEvents.lNetworkEvents & FD_READ) 
		{
			// nếu bit đc bật lên ktr tiếp xem có bị lỗi kết nối ko, nếu ko lỗi đọc dl và in ra màn hình
			if (networkEvents.iErrorCode[FD_READ_BIT] != 0)
			{
				printf("FD_READ error: %d\n", networkEvents.iErrorCode[FD_READ_BIT]);
				closesocket(client);
				break;
			}
			ret = recv(client, buf, sizeof(buf), 0); // ko cần ktra xem ret < 0 hay ko vì nếu < 0 thì đã xảy ra lỗi ở trên
			buf[ret] = 0;
			printf("Received: %s\n", buf);

			// nếu ret = 0 thì rơi vào trường hợp FD_CLOSE
			if (networkEvents.lNetworkEvents & FD_CLOSE)
			{
				if (networkEvents.iErrorCode[FD_CLOSE_BIT] != 0)
				{
					printf("FD_CLOSE error: %d\n", networkEvents.iErrorCode[FD_CLOSE_BIT]);
					closesocket(client);
					break;
				}
				closesocket(client);
			}
		}
	}
}
