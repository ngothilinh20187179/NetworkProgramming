// AsyncSelectClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
/* int WSAAsyncSelect(
    SOCKET s,   // [IN] Socket sẽ xử lý sự kiện
    HWND hWnd,  // [IN] Handle cửa sổ nhận sự kiện
    unsigned int wMsg, // [IN] Mã thông điệp, tùy chọn, thường >= WM_USER
    long lEvent // [IN] Mặt nạ chứa các sự kiện ứng dụng muốn nhận bao gồm FD_READ, FD_WRITE, FD_ACCEPT, FD_CONNECT, FD_CLOSE
);*/

// cd netcat
// nc64.exe -v -l -p 9000

#include <stdio.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define WM_SOCKET WM_USER + 1

BOOL CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM); // ham xu ly su kien cua cua so

#include <stdio.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>

#pragma comment(lib, "ws2_32")

#define WM_SOCKET WM_USER + 1

BOOL CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);

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

	WNDCLASS wndclass;
	const CHAR* providerClass = "AsyncSelect";
	HWND window;

	wndclass.style = 0;
	wndclass.lpfnWndProc = (WNDPROC)WinProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = NULL;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = (LPCWSTR)providerClass;

	if (RegisterClass(&wndclass) == 0)
		return NULL;

	// Create a window
	if ((window = CreateWindow((LPCWSTR)providerClass, L"", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, NULL, NULL)) == NULL)
		return NULL;

	WSAAsyncSelect(client, window, WM_SOCKET, FD_READ | FD_CLOSE);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

BOOL CALLBACK WinProc(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
	if (wMsg == WM_SOCKET)
	{
		int errorCode = WSAGETSELECTERROR(lParam);
		if (errorCode)
		{
			// Co loi xay ra
			printf("Loi ket noi bi huy\n");
			closesocket(wParam);
			return TRUE;
		}

		if (WSAGETSELECTEVENT(lParam) == FD_READ)
		{
			// Nhan du lieu
			char buf[256];
			int ret = recv(wParam, buf, sizeof(buf), 0);

			// Xu ly du lieu
			buf[ret] = 0;
			printf("Received: %s\n", buf);
		}

		if (WSAGETSELECTEVENT(lParam) == FD_CLOSE)
		{
			// Dong ket noi
			printf("Ket noi bi dong\n");
			closesocket(wParam);
		}
	}
	return TRUE;
}
