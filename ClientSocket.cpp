// SocketDemo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstring>
// #include <winsock.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
// #include <arpa/inet.h>
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

int main()
{
	cout << "*** Client socket ***\n";

	// Start :- common code for client & server
	
	// iSocket variable we will get unique Socket id.
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

	cout << "Client socket id is: " << (int)iSocket << endl;

	// End :- common code for client & server

	// On which server & port you want to connect?

	struct sockaddr_in serverAddress;
	// serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");	// deprecated - use InetPton instead
	InetPton(AF_INET, L"127.0.0.1", &serverAddress.sin_addr.s_addr);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(9200);

	//client will do connect request.
	if (connect(iSocket, (sockaddr*)&serverAddress, sizeof(sockaddr_in)) != 0)
	{
		WSACleanup();
		return false;
	}

	char cServerIPAddr[300] = { 0 };
	cout << "Client connected to server now! Server address is: ";
	InetNtopA(AF_INET, &serverAddress.sin_addr, cServerIPAddr, sizeof(cServerIPAddr));
	cout << cServerIPAddr << endl;

	char cClientBuffMsg[250] = { 0 };
	strcpy_s(cClientBuffMsg, 30, "Hello from Client!");		// strcpy is deprecated

	int i = 2;
	while (i)
	{
		send(iSocket, cClientBuffMsg, 250, 0);
		cout << "Client sending message " << i << ". Data: " << cClientBuffMsg << endl;
		--i;
	}

	char cServerResponseMsg[250] = { 0 };
	recv(iSocket, cServerResponseMsg, 250, 0);
	cout << "Received message from server: " << cServerResponseMsg << endl;

	cout << "Client is closing socket now..." << endl;
	closesocket(iSocket);

	WSACleanup();
	return true;
}