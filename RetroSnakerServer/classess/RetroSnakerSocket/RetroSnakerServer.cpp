#include <iostream>
using namespace std;
#include "RetroSnakerServer.h"

bool RetroSnakerServer::init()
{
 	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		return false;
	}
	return true;
}


 SOCKET RetroSnakerServer::CreateSock()
 {
 	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
 	if (sock == SOCKET_ERROR)
 	{
 		closeSocket(sock);
 		return 0;
 	}
 	return sock;
 }

SOCKET RetroSnakerServer::bindListen(SOCKET sock, u_int Port)
{
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(Port);
	sin.sin_addr.S_un.S_addr = inet_addr("0, 0, 0, 0");
	if (bind(sock, (LPSOCKADDR)& sin, sizeof(sin)) == SOCKET_ERROR)
	{
		closeSocket(sock);
		return 0;
	}
	if (listen(sock, 10) == SOCKET_ERROR)
	{
		return 0;
	}

	u_long nonBlock = 1;
	ioctlsocket(sock, FIONBIO, &nonBlock);
	//CreateSocketInformation();
	return sock;
}

SOCKET RetroSnakerServer::AcceptClinet(SOCKET sock)
{
	sockaddr_in sin;
	int nSize = sizeof(sin);
	SOCKET sockClient = accept(sock, (SOCKADDR*)& sin, &nSize);
	if (sockClient == SOCKET_ERROR)
	{
		closeSocket(sockClient);
		return 0;
	}
	return sockClient;
}

 void RetroSnakerServer::closeSocket(SOCKET sock)
 {
 	closesocket(sock);
 	WSACleanup();
 }

 bool RetroSnakerServer::CreateSocketInformation(SOCKET sock)
 {
	 
	 return false;
 }

