#include "RetroSnakerClient.h"



bool RetroSNakerClient::initSock()
{
	WSADATA wsaData;
	WORD wr = MAKEWORD(2, 2);
	if (WSAStartup(wr, &wsaData) != 0)
	{
		return true;
	}
	return false;
}

u_long RetroSNakerClient::ResolverAdress(char *serverIp)
{
	u_long nAddr = inet_addr(serverIp);
	if (nAddr == INADDR_NONE)
	{
		HOSTENT *ent = gethostbyname(serverIp);
		if (ent == NULL)
		{
			CCLOG("get Host name errror ! %d", WSAGetLastError());
			return 0;
		}
		else
		{
			nAddr = *((u_long *)ent->h_addr_list[0]);
		}
	}
	if (nAddr == INADDR_NONE)
	{
		CCLOG("Analysis Host Position error");
		return 0;
	}
	return nAddr;
}

SOCKET RetroSNakerClient::ConnectServer(u_long serverIp, int port)
{
	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sock == INVALID_SOCKET)
	{
		return INVALID_SOCKET;
	}
	sockaddr_in serverSin;
	serverSin.sin_family = AF_INET;
	serverSin.sin_port = htons(port);
	serverSin.sin_addr.s_addr = serverIp;
	if (connect(m_sock, (sockaddr*)&serverSin, sizeof(sockaddr_in)) == SOCKET_ERROR)
	{
		CCLOG("conntect server error %d", WSAGetLastError());
		closesocket(m_sock);
		return SOCKET_ERROR;
	}
	return m_sock;
}



