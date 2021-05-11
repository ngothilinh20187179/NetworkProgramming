#include <stdio.h>
#include <windows.h>

// Khai bao ham thuc thi luong 1
DWORD WINAPI Thread1(LPVOID ipParam) {
	for (int i = 1; i <= 9; i += 2) // in so le ra man hinh
	{
		Sleep(2000); // milliseconds
		printf("so le: %d\n", i);
	}
	printf("Thread1 done\n");
	return 0;
}
// Khai bao ham thuc thi luong 2
DWORD WINAPI Thread2(LPVOID ipParam) {
	for (int i = 0; i <= 10; i += 2) // in so chan ra man hinh
	{
		Sleep(1000); // milliseconds
		printf("so chan: %d\n", i);
	}
	printf("Thread2 done\n");
	return 0;
}
int main()
{	// Khoi tao, thuc thi luong 1 luong 2
	HANDLE thread1 = CreateThread(0, 0, Thread1, 0, 0, 0);
	HANDLE thread2 = CreateThread(0, 0, Thread2, 0, 0, 0);

	// Tao mang handles de truyen tham so cho ham WaitForMultipleObject
	HANDLE handles[2];
	handles[0] = thread1;
	handles[1] = thread2;

	DWORD ret = WaitForMultipleObjects(2, handles, TRUE, INFINITE);
	printf("Result: %d", ret);
	return 0;
}
