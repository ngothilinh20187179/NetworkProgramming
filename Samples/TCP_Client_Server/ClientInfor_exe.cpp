// B3: tr112
// Viết chương trình clientinfo thực hiện kết nối đến một máy chủ xác định và
// gửi thông tin về tên máy, danh sách các ổ đĩa có trong máy, kích thước các ổ đĩa. 
// Địa chỉ (tên miền) và cổng nhận vào từ tham số dòng lệnh.
// VD: clientinfo.exe localhost 1234

#include <stdio.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#pragma comment(lib, "ws2_32")

int main()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // dia chi cua server
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("argv[1]");
    addr.sin_port = htons(atoi(argv[2])); // can truyen port (int) ma argv la char -> ep kieu

    connect(client, (SOCKADDR*)&addr, sizeof(addr));

    char buf[256];

    char computerName[256];
    DWORD nameLength = sizeof(computerName);
    GetComputerNameA(computerName, &nameLength);

    char driverLetters[256];
    int ret = GetLogicalDriveStringsA(sizeof(driverLetters), driverLetters);

    DWORD sectorsPerCluster, bytesPerSector, numberOfFreeClusters, totalNumberOfClusters;

    // Tao buffer gui du lieu di
    char numDrivers = ret / 4;
    buf[0] = numDrivers;
    int pos = 1;
     for (int i = 0; i < numDrives; i++)
    {
        // Drive letter
        buf[pos] = driveLetters[i * 4];
        pos += 1;

        // Drive size
        GetDiskFreeSpaceA(driveLetters + i * 4, &sectorsPerCluster, &bytesPerSector,
            &numberOfFreeClusters, &totalNumberOfClusters);

        float size = (float)totalNumberOfClusters * (float)sectorsPerCluster * (float)bytesPerSector / (float)(1 << 30);
        memcpy(buf + pos, &size, sizeof(size));
        pos += sizeof(size);
    }
    memcpy(buf + pos, computerName, nameLength + 1);
    pos += nameLength + 1;
    
    /*DWORD sectorPerCluster, bytesPerSector, numberOfFreeClusters, totalNumberOfCluster;
    GetDiskFreeSpaceA("C:\\", &sectorPerCluster, &bytesPerSector, &numberOfFreeClusters, &totalNumberOfCluster);
    double size = (double)totalNumberOfCluster * (double)sectorPerCluster * (double)bytesPerSector;*/
}
