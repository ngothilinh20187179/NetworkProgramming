#include <stdio.h>
#include <windows.h>

BOOL threadDone = FALSE;
DWORD WINAPI helloFunc(LPVOID arg) {
  printf(“Hello Thread\n”);
  threadDone = TRUE;
  return 0;
}

int main() {
  HANDLE hThread = CreateThread(NULL, 0, helloFunc, NULL, 0, NULL );
  WaitForSingleObject(hThread, INFINITE); // chỉ muốn chờ cho đến khi hàm thực thi xong -> truyền INFINITE cho tham số dwMilliseconds.
}
