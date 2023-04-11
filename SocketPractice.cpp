#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

const int BUF_SIZE = 64;


int SendMsg(SOCKET s, SOCKADDR_IN addr);
int RecvMsg(SOCKET s);


int main()
{
	SOCKET s;
	SOCKADDR_IN addr;
	char SendIp[BUF_SIZE] = {};
	char msg[BUF_SIZE]; //buffer to send data
	char buf[BUF_SIZE] = {}; //buffer to recieve data
	int iWSAStatus;		// Windows Socket API Status
	WSADATA wsaData;
	int sockErr;
	bool loop;

	iWSAStatus = WSAStartup(MAKEWORD(2, 2), &wsaData);

	// what did we get?
	std::cout << wsaData.szDescription << std::endl;

	// address family (TCP, UDP), datagram socket (UDP), 0 for default protocol
	s = socket(AF_INET, SOCK_DGRAM, 0);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(49152);

	//Professor's IP CHANGE WHEN NEEDED
	//inet_pton(AF_INET, "64.72.1.234", &(addr.sin_addr));

	if (s == INVALID_SOCKET)
	{
		// see windows socket errors
		std::cout << "Error creating socket: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return 1;
	}

	if (s == SOCKET_ERROR)
	{
		closesocket(s);
		std::cout << "Error connecting: " << WSAGetLastError() << std::endl;

		WSACleanup();
		return 1;
	}

	loop = true;
	char choice;
	char c[2];
	int retVal = 1;
	while (loop)
	{
		std::cout << "Are you Sending or Receiving? (S/R or s/r): ";
		std::cin.getline(c,2);
		choice = c[0];
		if (choice == 'S' || choice == 's')
		{
			retVal = SendMsg(s, addr);
			RecvMsg(s);
		}
		else if (choice == 'R' || choice == 'r')
		{
			retVal = RecvMsg(s);
		}

		if (retVal == 0)
		{
			loop = false;
		}
	}
	closesocket(s);
	WSACleanup();
	return 0;
}

int SendMsg(SOCKET s, SOCKADDR_IN addr)
{
	char SendIp[BUF_SIZE];
	char msg[BUF_SIZE] = {}; //buffer to send data
	char buf[BUF_SIZE] = {}; //buffer to recieve data
	std::cout << "Enter IP address you wish to send to: ";
	std::cin.getline(SendIp, BUF_SIZE);
	std::cout << std::endl;
	std::cout << "Enter the message you wish to send (or quit): " << std::endl;
	std::cin.getline(msg, BUF_SIZE);

	if (std::cin.fail())
	{
		std::cin.clear();
		std::cin.ignore(255, '\n');
	}
	inet_pton(AF_INET, SendIp, &(addr.sin_addr));
	int error = sendto(s, msg, strlen(msg) + 1, 0, (sockaddr*)&addr, sizeof(addr));

	if (error == SOCKET_ERROR)
	{
		closesocket(s);
		std::cout << "Error connecting: " << WSAGetLastError() << std::endl;

		WSACleanup();
		return 0;
	}

	if (!_stricmp(buf, "quit"))
	{
		return 0;
	}

	return 1;
}

int RecvMsg(SOCKET s)
{
	SOCKADDR_IN address = {};
	char ip[BUF_SIZE];
	char buf[BUF_SIZE];
	char str_addr[INET_ADDRSTRLEN];

	//inet_pton(AF_INET, "127.0.0.1", &address);
	int addressSize = sizeof(address);

	int bytesRecv = recvfrom(s, buf, BUF_SIZE, 0, (SOCKADDR*)&address, &addressSize);
	inet_ntop(AF_INET, &address.sin_addr, str_addr, INET_ADDRSTRLEN);

	std::cout << "Message recieved from: " << str_addr << std::endl;
    std::cout << "recv " << bytesRecv << " bytes.  Msg: " << buf << std::endl;

	if (bytesRecv == SOCKET_ERROR)
	{
		closesocket(s);
		std::cout << "Error recv: " << WSAGetLastError() << std::endl;
		return 0;
	}

	if (!_stricmp(buf, "quit"))
	{
		return 0;
	}


}
