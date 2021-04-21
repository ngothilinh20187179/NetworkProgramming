// Thread1.cpp 

#include <stdio.h>
#include <windows.h>

// Khai bao ham thuc thi luong
DWORD WINAPI helloFunc(LPVOID lpParam)
{
	Sleep(7000);
	printf("Hello Thread\n");
	return 0;
}
int main()
{
	// Tao va thuc thi luong
	HANDLE thread = CreateThread(0, 0, helloFunc, 0, 0, 0);

	// Doi den khi luong ket thuc hoac het gio
	DWORD result = WaitForSingleObject(thread, 5000);
	if (result == WAIT_OBJECT_0)
		printf("Thread done\n");
	else if (result == WAIT_TIMEOUT)
		printf("Time out\n");
	return 0;
}
