// EventSelectServer.cpp : Tao server theo mo hinh WSAEventSelect
/*WSAWaitForMultipleEvents(
	DWORD cEvents,     //[IN] Số lượng sự kiện cần đợi
	const WSAEVENT FAR * lphEvents, //[IN] Mảng sự kiện, max 64
	BOOL fWaitAll,    //[IN] Có đợi tất cả các sự kiện không ?
	DWORD dwTimeout, //[IN] Thời gian đợi tối đa
	BOOL fAlertable)*/

/*Giá trị trả về
	Thành công: Số thứ tự của sự kiện xảy ra + WSA_WAIT_EVENT_0.
	vd 0,1,2,3,4; xảy ra sự kiện 2,3,4 -> trả về 4 -> số sự kiện xra = 4 - WSA_WAIT_EVENT_0 = 4-1=3
	Hết giờ: WSA_WAIT_TIMEOUT.
	Thất bại: WSA_WAIT_FAILED.
*/
// cd netcat
// nc64.exe -v -l -p 127.0.0.1 9000

#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32")

#define MAX_EVENTS 64

int main()
{

	// Khoi tao thu vien Winsock
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	// Khai bao dia chi cuc bo cua server
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(9000);

	// Khai bao socket, gan voi dia chi va chuyen sang trang thai cho ket noi
	SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	bind(listener, (SOCKADDR*)&addr, sizeof(addr));
	listen(listener, 5);

	// Khai bao mang socket va event
	SOCKET sockets[MAX_EVENTS];
	WSAEVENT events[MAX_EVENTS];
	int numEvents = 0;

	// Tao doi tuong event, gan voi socket qua su kien FD_ACCEPT (cho ket noi)
	WSAEVENT newEvent = WSACreateEvent();
	WSAEventSelect(listener, newEvent, FD_ACCEPT);

	// Them socket va events vao mang
	sockets[numEvents] = listener;
	events[numEvents] = newEvent;
	numEvents++;

	// Khai bao bien chua ket qua 
	WSANETWORKEVENTS networkEvents;
	char buf[256];
	int ret, idx;

	while (1)
	{
		/* WSAWaitForMultipleEvents(...)
		* số lượng sự kiện trong mảng cần đợi là 1
		* gán vào địa chỉ newEvent
		* FALSE ko đợi tất cả các sk
		* WSA_INFINITE tg đợi tối đa, tham số này ko quan tâm thời gian timeout, chỉ trả về khi 1 trg những sk xảy ra hoặc bị lỗi gì đó
		* FALSE 
		*/
		// Cho cho den khi 1 su kien duoc bao hieu
		ret = WSAWaitForMultipleEvents(numEvents, events, FALSE, WSA_INFINITE, FALSE);
		idx = ret - WSA_WAIT_EVENT_0;

		// Duyet tu su kien duoc bao hieu den cac su kien tiep theo trong mang
		for (int i = idx; i < numEvents; i++)
		{
			/*gọi hàm WSAWaitForMultipleEvents với từng sk của đối tượng
			 slg sk =1, địa chỉ là sự kiện tại đối tượng i
			 waitAll true hay false như nhau vì chỉ có 1 sk
			 time out =0 để đối với những sk phía sau chưa chuyển sang trạng thái báo hiệu nó sẽ trả về gtr là wait time out 
			 đối tượng đã chuyển sang báo hiệu thì trả về 0*/
			// Kiem tra su kien co xay ra hay khong
			ret = WSAWaitForMultipleEvents(1, &events[i], TRUE, 0, FALSE);
			if (ret == WSA_WAIT_FAILED || ret == WSA_WAIT_TIMEOUT)
				continue;

			// Khoi tao lai doi tuong su kien
			WSAResetEvent(events[i]);

			// Lay ket qua
			WSAEnumNetworkEvents(sockets[i], events[i], &networkEvents);

			// Kiem tra su kien FD_ACCEPT
			if (networkEvents.lNetworkEvents & FD_ACCEPT)
			{
				// Kiem tra loi
				if (networkEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
				{
					printf("FD_ACCEPT failed with error code %d\n", networkEvents.iErrorCode[FD_ACCEPT_BIT]);
					continue;
				}
				
				// Chap nhan ket noi
				SOCKET client = accept(sockets[i], NULL, NULL);
				// Kiem tra neu vuot qua so luong su kien toi da thi bo qua
				if (numEvents > MAX_EVENTS) 
				{
					printf("Too many connections");
					closesocket(client);
					continue;
				}

				// Neu khong vuot qua thi tao doi tuong su kien
				newEvent = WSACreateEvent();
				// Gan voi socket va dang ky su kien FD_READ va FD_CLOSE
				WSAEventSelect(client, newEvent, FD_READ | FD_CLOSE);

				// Them vao mang
				sockets[numEvents] = client;
				events[numEvents] = newEvent;
				numEvents++;

				printf("Client accepted: %d\n", client);
			}

			// Xu ly su kien co du lieu den server
			if (networkEvents.lNetworkEvents & FD_READ)
			{
				if (networkEvents.iErrorCode[FD_READ_BIT] != 0)
				{
					printf("FD_READ failed with error code %d\n", networkEvents.iErrorCode[FD_READ_BIT]);
					continue;
				}

				ret = recv(sockets[i], buf, sizeof(buf), 0);

				buf[ret] = 0;
				printf("Received from %d: %s\n", sockets[i], buf);

			}

			// Xu ly su kien ngat ket noi
			if (networkEvents.lNetworkEvents & FD_CLOSE)
			{
				if (networkEvents.iErrorCode[FD_CLOSE_BIT] != 0)
				{
					printf("FD_CLOSE failed with error code %d\n", networkEvents.iErrorCode[FD_CLOSE_BIT]);
				}

				printf("Client disconnected: %d\n", sockets[i]);
			}
		}
	}
}
