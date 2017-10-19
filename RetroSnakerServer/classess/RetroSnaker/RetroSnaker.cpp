#include "RetroSnaker.h"
#include <thread>
#pragma warning( disable : 4996)

RetroSnaker::RetroSnaker()
{
	initGame();
}

void RetroSnaker::initGame()
{
	do 
	{
		init();
		SOCKET sock = CreateSock();
		m_sock = bindListen(sock, MY_PORT);

		std::thread AcceptThread(&RetroSnaker::AcceptCallBack, this);
		AcceptThread.join();
		std::thread RecvThread(&RetroSnaker::RecvCallBack, this);
		RecvThread.join();	
	} while (0);

}

void RetroSnaker::AcceptCallBack()
{
	while (true)
	{
		if (m_sock == SOCKET_ERROR)
		{
			closeSocket(m_sock);
			break;
		}
		SOCKET sockServer = AcceptClinet(m_sock);
		u_long nonBlock = 1;
		ioctlsocket(sockServer, FIONBIO, &nonBlock);
		m_mutex.lock();
		m_VecSock.push_back(sockServer);
		m_mutex.unlock();
	}
}

void RetroSnaker::RecvCallBack()
{
	char recBuf[MSG_PACK_LENG] = { 0 };
	while (true)
	{
		
	}
} 

int RetroSnaker::getMessageLen(char chRecBuf[], int nHeadLen)
{
	char chHead[4] = {0};
	strncpy(chHead, chRecBuf, 4);
	return (int)chHead;
}

char* RetroSnaker::DeleteMessage(char chRecBuf[], int nMsgLen, int nBufLen)
{
	for (int i = 0; nMsgLen + i <= nBufLen; i++)
	{
		chRecBuf[i] = chRecBuf[i + nMsgLen];
	}
	memset(chRecBuf + (nBufLen - nMsgLen), 0, 1024 - (nBufLen - nMsgLen));
	return chRecBuf;
}

void RetroSnaker::TaskCallBack()
{
	if (m_Task.empty())
	{
		return;
	}
	m_mutex.lock();
	auto Task = m_Task.begin();
	m_Task.erase(m_Task.begin());
	m_mutex;
	Message::TagPlayerData *msg = (Message::TagPlayer)






}
