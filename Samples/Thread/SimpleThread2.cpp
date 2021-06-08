// 4luong.cpp 
#include <stdio.h>
#include <windows.h>

// Lay tham so da truyen vao luong tu con tro lpParam
DWORD WINAPI helloFunc(LPVOID ipParam) 
{
	// int *pIdx = (int*)ipParam; 
	// int idx = pIdx;
	int idx = *(int*)ipParam; 
	printf("Hello: %d\n", idx);
	return 0;
}
int main()
{
	HANDLE handles[4];
	int params[4];
	for (int i = 0; i < 4; i++)
	{
		params[i] = i;
		handles[i] = CreateThread(0, 0, helloFunc, &params[i], 0, 0);
	}
	WaitForMultipleObjects(4, handles, TRUE, INFINITE);
	return 0;
}
