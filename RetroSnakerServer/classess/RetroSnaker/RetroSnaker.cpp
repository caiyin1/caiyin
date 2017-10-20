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
		std::thread unPackThread(&RetroSnaker::unPackCallBack, this);
		unPackThread.join();
	} while (0);
}

void RetroSnaker::AcceptCallBack()
{
	while (m_nClinetNum < FD_SETSIZE)
	{
		if (m_sock == SOCKET_ERROR)
		{
			closeSocket(m_sock);
			break;
		}
		SOCKET sockServer = AcceptClinet(m_sock);
		u_long nonBlock = 1;
		ioctlsocket(sockServer, FIONBIO, &nonBlock);

		m_nClinetNum++;
	}
}

void RetroSnaker::RecvCallBack()
{
	fd_set fdRead;
	FD_ZERO(&fdRead);
	timeval timeout;
	timeout.tv_sec = 0.002f;
	timeout.tv_usec = 0;
	int nContinue = 0;
	while (true)
	{
		fdRead = m_fdClientSock;
		nContinue = select(0, &fdRead, NULL, NULL, &timeout);
		if (nContinue != SOCKET_ERROR)
		{
			for (int i = 0; i < m_fdClientSock.fd_count; i++)
			{
				if (FD_ISSET(m_fdClientSock.fd_array[i], &fdRead))
				{
					auto nPlayerID = m_MapSockPlayerID.find(m_fdClientSock.fd_array[i])->second;
					auto PlayerData = m_MapPlayerData.find(nPlayerID)->second;
					
					m_mutexRecv.lock();
					int nLen = recv(m_fdClientSock.fd_array[i], PlayerData->chRecvBuf, MSG_PACK_LENG, 0);
					m_mutexRecv.unlock();

					if (nLen == 0 || nLen == SOCKET_ERROR)
					{
						m_mutexSock.lock();
						closesocket(m_fdClientSock.fd_array[i]);
						FD_CLR(m_fdClientSock.fd_array[i], &m_fdClientSock);
						m_mutexSock.unlock();
						continue;
					}
					PlayerData->nRecvSize += nLen;
				}
			}
		}
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
	m_mutexRecv.lock();
	auto Task = m_Task.begin();
	m_Task.erase(m_Task.begin());
	m_mutexRecv;
	Message::TagMsgHead *msg = (Message::TagMsgHead*) &Task;
}

void RetroSnaker::HandleTask(Message::TagMsgHead *msg)
{
	switch (msg->nMessageHead)
	{
	case HEAD_SIGN_IN:
	{int nPlayerID = CreatePlayerID(msg);
	m_TagPlayerData* PlayerData;
	//PlayerData->
	break;
	}	
	case HEAD_READY:
		m_nReadyNum++;
		break;
	case HEAD_PLAYER_DATA:

		break;
	case HEAD_SNAKE_POSITION:

		break;
	}
}

int RetroSnaker::CreatePlayerID(Message::TagMsgHead *msg)
{
	if (msg->nMsgID <= 0)
	{
		return m_nPlayerID++;
	}
	return msg->nMsgID;
}

void RetroSnaker::unPackCallBack()
{
	for (auto& PalyerData : m_MapPlayerData)
	{
		if (PalyerData.second->nRecvSize > sizeof(int))
		{
			int nMsgLeng = getMessageLen(PalyerData.second->chRecvBuf, sizeof(int));
			if (nMsgLeng < PalyerData.second->nRecvSize)
			{
				char chMsg[1024];
				strncpy(chMsg, PalyerData.second->chRecvBuf, nMsgLeng);
				m_mutexRecv.lock();
				*PalyerData.second->chRecvBuf = *DeleteMessage(PalyerData.second->chRecvBuf, nMsgLeng, PalyerData.second->nRecvSize);
				m_mutexRecv.unlock();
				Message::TagMsgHead *pMsg = (Message::TagMsgHead*) chMsg;

				m_mutexTask.lock();
				m_Task.push_back(pMsg);
				m_mutexTask.unlock();
			}
		}
	}



}

