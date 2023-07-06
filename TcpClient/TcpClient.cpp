#include "stdafx.h"
#include <winsock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "ws2_32")
//#pragma warning(disable:4996) 

int _tmain(int argc, _TCHAR* argv[])
{
	// 0. initialize winsock
	WSADATA wsa = { 0 };
	if (::WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		puts("Error: Can not initialize winsock.");
		return 0;
	}

	// 1. create socket (client requires only one socket; communication)
	SOCKET hSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
	{
		puts("Error: Can not create socket.");
		return 0;
	}

	// 2. port binding and connect
	// client must know the server's port number and IP address
	SOCKADDR_IN	svrAddr = { 0 }; // server address
	svrAddr.sin_family = AF_INET; // IPv4
	svrAddr.sin_port = htons(25000); // port number 250000
	//svrAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); // mirroring myself
	InetPton(AF_INET, _T("127.0.0.1"), &svrAddr.sin_addr.s_addr);
	if (::connect(hSocket, (SOCKADDR*)&svrAddr, sizeof(svrAddr)) == SOCKET_ERROR)
	{
		puts("ERROR: Can not connect to server. The server may not listening.");
		return 0;
	}
	else
	{
		puts("You have connected to the server.");
		fflush(stdout);
	}

	// 3. messsage send and receive
	char szBuffer[128] = { 0 }; // buffer size
	while (1)
	{
		gets_s(szBuffer); // get a string and store it in the buffer
		if (strcmp(szBuffer, "EXIT") == 0)		
			break;

		// send message to server
		// size + 1 because last character of string is 'null'
		::send(hSocket, szBuffer, strlen(szBuffer) + 1, 0);
		// clear buffer
		memset(szBuffer, 0, sizeof(szBuffer)); 
		// receive echo message from the server
		// echo message size is fixed to 128 byte, refer server code ::send 
		::recv(hSocket, szBuffer, sizeof(szBuffer), 0);
		printf("From server: %s\n", szBuffer);
	}

	// 5. close listening socket
	::closesocket(hSocket);

	// 0. clear winsock
	::WSACleanup();
	return 0;
}