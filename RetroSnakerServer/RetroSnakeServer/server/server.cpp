#include "server.h"

#include <iostream>
#include <string.h>
using namespace std;

bool Server::init()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		return false;
	}
	return true;
}

SOCKET Server::CreateSock()
{
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		return 0;
	}
	return sock;
}

SOCKET Server::BindListen(SOCKET sock)
{
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORT);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(sock,(LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		closeSockt(sock);
		return 0;
	}
	if (listen(sock, 10) == SOCKET_ERROR)
	{
		return 0;
	}
	return sock;
}

SOCKET Server::acceptClient(SOCKET sock)
{
	sockaddr_in remoteAddr;
	int nRemoteAddr = sizeof(remoteAddr);
	SOCKET sockClient = accept(sock, (SOCKADDR*)&remoteAddr, &nRemoteAddr);
	if (sockClient == INVALID_SOCKET)
	{
		//log("accept error!");
		return 0;
	}
	return sockClient;
}

void Server::closeSockt(SOCKET sock)
{
	closesocket(sock);
	WSACleanup();
}

