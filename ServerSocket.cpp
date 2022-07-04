// ServerSocket.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

int main()
{
	std::cout << "***Server socket***\n";
		
	SOCKET iSocket;
	WSADATA wsData;
	if (WSAStartup(MAKEWORD(2, 2), &wsData) != 0)
	{
		return false;
	}
	if ((LOBYTE(wsData.wVersion) != 2) || (HIBYTE(wsData.wVersion) != 2))
	{
		WSACleanup();
		return false;
	}
	if ((iSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == SOCKET_ERROR)
	{
		return false;
	}

	std::cout << "Server socket id is: " << (int)iSocket << endl;

	// Server details for client to connect
	struct sockaddr_in serverAddress;
	// serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	InetPton(AF_INET, L"127.0.0.1", &serverAddress.sin_addr.s_addr);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(9200);

	// bind the above details with unique server id.
	if (bind(iSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
	{
		int i = WSAGetLastError();
		return -1;
	}

	std::cout << "Bind is successful" << endl;

	// server is on listen mode now.. 10 means 10 clients can connect to it
	if (listen(iSocket, 10) == SOCKET_ERROR)
	{
		std::cout << "Error while listening" << endl;
		return true;
	}

	std::cout << "Listen successful" << endl;

	// Server will accept client request here. Here again 1 unique id we will create,
	// so server will communicate with client using below socket id.

	SOCKET clientCommunicationSocket;
	struct sockaddr_in clientAddress;
	int len = sizeof(clientAddress);
	// if ((clientCommunicationSocket = accept(iSocket, NULL, NULL)) == SOCKET_ERROR)
	if ((clientCommunicationSocket = accept(iSocket, (sockaddr*)&clientAddress, &len)) == SOCKET_ERROR)
	{
		WSACleanup();
		return -1;
	}
	std::cout << "Client connected to server now! Client address is: ";
	char cClientIPAddr[200] = { 0 };
	InetNtopA(AF_INET, &clientAddress.sin_addr, cClientIPAddr, sizeof(cClientIPAddr));
	std::cout << cClientIPAddr << endl;

	char cServerBuffMsg[250] = { 0 };

	int i = 2;
	while (i)
	{
		recv(clientCommunicationSocket, cServerBuffMsg, 250, 0);
		std::cout << "Server received message " << i << " from client. Data: " << cServerBuffMsg << endl;
		i--;
	}

	strcpy_s(cServerBuffMsg, "Bye!");
	std::cout << "Server sending message to client: " << cServerBuffMsg << endl;
	send(clientCommunicationSocket, cServerBuffMsg, 250, 0);

	std::cout << "Server now closing client socket..." << endl;
	closesocket(clientCommunicationSocket);			// close client socket communication

	std::cout << "Server now closing main socket..." << endl;
	closesocket(iSocket);							// close main   socket communication

	WSACleanup();
	return 0;
}