#include <iostream> 
#include <thread>
#include <time.h>
#include<stdio.h>
#include<stdlib.h>
#include "RetroSnaker.h"

#pragma warning( disable : 4996)
using namespace std;

struct RetroSnaker* s_global = nullptr;

const static std::unordered_map<int, int> g_MapColour{
	//黄色
	std::make_pair(1, 16757760),
	//橙色
	std::make_pair(2, 16737330),
	//粉色
	std::make_pair(3, 16757960),
	//蓝色
	std::make_pair(4, 6579400),
	//荧绿色
	std::make_pair(5, 3323080),
	//草绿色
	std::make_pair(6, 46080),
	//红色
	std::make_pair(7, 16384000),
};



RetroSnaker* RetroSnaker::getInstance()
{
	if (s_global == NULL)
	{
		s_global = new RetroSnaker;
		//s_global->init();
	}
	return s_global;

}
void RetroSnaker::initGame()
{
	int l = 20;
	char* p = (char*)l;
	int nSzie = (int)p;
	int nLen = getMessageLen(p, 4);
	do 
	{
		init();
		SOCKET sock = CreateSock();
		m_sock = bindListen(sock, MY_PORT);
		if (m_sock == 0)
		{
			return;
		}
		std::thread AcceptThread(&RetroSnaker::AcceptCallBack, this);

		std::thread RecvThread(&RetroSnaker::RecvCallBack, this);
		
		std::thread unPackThread(&RetroSnaker::TimerCallBack, this);
		
		//std::thread TaskThread(&RetroSnaker::TaskCallBack, this);
		AcceptThread.join();
		RecvThread.join();
		unPackThread.join();
		
	} while (0);
}

void RetroSnaker::AcceptCallBack()
{
	while (m_nClientNum < FD_SETSIZE)
	{
		if (m_sock == SOCKET_ERROR)
		{
			closeSocket(m_sock);
			break;
		}
		SOCKET sockServer = AcceptClinet(m_sock);
		
		initPlayer(sockServer);
		m_nClientNum++;
	//	printf("m_nClientNum = %d\n", m_nClientNum);
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
		makefd(&fdRead);
		nContinue = select(0, &fdRead, NULL, NULL, &timeout);
		if (nContinue != 0)
		{
			for (int i = 0; i < fdRead.fd_count; i++)
			{
				if (FD_ISSET(fdRead.fd_array[i], &fdRead))
				{
					int nPlayerID = getPlayerID(fdRead.fd_array[i]);
					auto PlayerData = m_MapPlayerData.find(nPlayerID)->second;
					m_mutexRecv.lock();
					int nLen = recv(fdRead.fd_array[i], PlayerData->chRecvBuf, MSG_PACK_LENG, 0);
					m_mutexRecv.unlock();

					if (nLen == 0 || nLen == SOCKET_ERROR)
					{
						m_mutexSock.lock();
						closesocket(fdRead.fd_array[i]);
						FD_CLR(fdRead.fd_array[i], &fdRead);
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
	//strncpy(chHead, chRecBuf, 4);
	return (int)chRecBuf;
}

char* RetroSnaker::DeleteMessage(char chRecBuf[], int nMsgLen, int nBufLen)
{
	for (int i = 0; nMsgLen + i < nBufLen; i++)
	{
		chRecBuf[i] = chRecBuf[i + nMsgLen];
	}
	memset(chRecBuf + (nBufLen - nMsgLen), 0, 1024 - (nBufLen - nMsgLen));
	return chRecBuf;
}

void RetroSnaker::TaskCallBack()
{
//	HandleTask(msg);

}

void RetroSnaker::HandleTask(Message::TagMsgHead msg, char* pchMsg, int nPlayerID)
{
	switch (msg.nMessageHead)
	{
	case HEAD_LOGIN:
		{
			Message::TagPlayerData *PlayerData = (Message::TagPlayerData*) pchMsg;
			PlayerData->nPlayerID = nPlayerID;
			char* p = &PlayerData->chStart;
			std::string strPlayerName;
			for (; *p != 0; p++)
			{
				strPlayerName += *p;
 			}
			PlayerSignIn(PlayerData, strPlayerName);
		}
			break;
	case HEAD_READY:
		{
			m_nReadyNum++;
			Message::TagSendState* sendState = (Message::TagSendState*) pchMsg;
			sendPlayerState(sendState);
		}
			break;
	case HEAD_PLAYER_DATA:
										
		break;
	case HEAD_DIRECTION:
		{
			Message::TagSnakeHeadDirection* SnakeDirection = (Message::TagSnakeHeadDirection*) pchMsg;
			auto PlayerData = m_MapPlayerData.find(SnakeDirection->nPlayerID);
			PlayerData->second->nSnakeDirecotion = SnakeDirection->nDirection;
		}
		break;
	}
}

int RetroSnaker::CreatePlayerID()
{
	return m_nPlayerID++;
}

void RetroSnaker::unPackCallBack()
{ 
	for (auto& playerData : m_MapPlayerData) 
	{
		if (playerData.second->nRecvSize > sizeof(int) * 3)
		{
			Message::TagMsgHead MsgHead = *(Message::TagMsgHead*)playerData.second->chRecvBuf;
			if (MsgHead.nMsgLeng <= playerData.second->nRecvSize)
			{
				char* pchMsg = new char[MsgHead.nMsgLeng];
				std::memset(pchMsg, 0, MsgHead.nMsgLeng);
				memcpy(pchMsg, playerData.second->chRecvBuf, MsgHead.nMsgLeng);
				m_mutexRecv.lock();
				*playerData.second->chRecvBuf = *DeleteMessage(playerData.second->chRecvBuf, MsgHead.nMsgLeng, playerData.second->nRecvSize);
				playerData.second->nRecvSize -= MsgHead.nMsgLeng;
				m_mutexRecv.unlock();
				printf("name =  %c%c%c\n", pchMsg[MsgHead.nMsgLeng - 3], pchMsg[MsgHead.nMsgLeng - 2], pchMsg[MsgHead.nMsgLeng - 1]);
				HandleTask(MsgHead, pchMsg, playerData.second->nPlayerID);
				delete[] pchMsg;
				pchMsg = NULL;
			}
		}
	}
}

void RetroSnaker::initPlayer(SOCKET sock)
{
	g_TagPlayerData PlayerData;
	PlayerData.sock = sock;
	int nPlayerID = CreatePlayerID();
	PlayerData.nRecvSize = 0;
	PlayerData.nPlayerID = nPlayerID;
	PlayerData.nColour = -1;
	PlayerData.nRead = -1;
	memset(PlayerData.chRecvBuf, 0, MSG_PACK_LENG);
	m_mutexPlayerData.lock();
	m_MapPlayerData.insert(std::make_pair(nPlayerID, &PlayerData));
	m_mutexPlayerData.unlock();
}

void RetroSnaker::PlayerSignIn(Message::TagPlayerData *msg, std::string strPlayerName)
{
	Message::TagPlayerData sendData;
	sendData.nColour = initColour();
	sendData.nMessageHead = HEAD_LOGIN;
	sendData.nMsgID = msg->nMsgID;
	sendData.nPlayerID = msg->nPlayerID;
	sendData.chStart = *strPlayerName.c_str();
	sendData.nState = 0;
	sendData.nMsgLeng = sizeof(int)* 6 + 1 +  strPlayerName.size();
	char sendBuf[MSG_PACK_LENG] = { 0 };
	memcpy(sendBuf, &sendData, sizeof(int)* 6 + 1);
	memcpy(sendBuf + sizeof(int)* 6 + 1, strPlayerName.c_str() + 1, strPlayerName.size() - 1);
	auto sock = m_MapPlayerData.find(msg->nPlayerID);	
	auto sockClient = sock->second->sock;
	m_mutexPlayerData.lock();
	auto playerData = m_MapPlayerData.find(msg->nPlayerID);
	playerData->second->strPlayerName = strPlayerName;
	playerData->second->nColour = sendData.nColour;
	
	m_mutexPlayerData.unlock();
	int nLen = send(sockClient, sendBuf, sendData.nMsgLeng, 0);
}

int RetroSnaker::initColour()
{
	if (m_nColour >= 7 || m_nColour <= 0)
	{
		m_nColour = 1;
	}
	auto nColour = g_MapColour.find(m_nColour);
	if ( nColour == g_MapColour.end())
	{
		return -1;
	}
	m_nColour++;
	return nColour->second;
}

void RetroSnaker::TimerCallBack()
{
	MSG msg;
	PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);

	UINT timerid = SetTimer(NULL, 1, 10, OnTimer);
	UINT SendTimerid = SetTimer(NULL, 2, 1000, sendPositionTimer);
	//UINT sendTime = SetTimer(NULL, 2, 1000, OnTimer);
	BOOL bRet;
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (bRet == -1)
		{
			printf("Error:the thread will quit,error id is %d/n", GetLastError());
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	KillTimer(NULL, timerid);
	KillTimer(NULL, SendTimerid);
	printf("thread end here/n");
}

void RetroSnaker::makefd(fd_set *fd_list)
{
	FD_ZERO(fd_list);//首先将fd_list清0  
	//将每一个socket加入fd_list中  
	for (auto& iter : m_MapPlayerData)
	{
		FD_SET(iter.second->sock, fd_list);
	}
}

int RetroSnaker::getPlayerID(SOCKET sock)
{
	for (auto& iter : m_MapPlayerData)
	{
		if (iter.second->sock == sock)
		{
			return iter.first;
		}
	}
}

void RetroSnaker::sendPosition()
{
	if (m_nReadyNum < PLAYER_NUM)
	{
		return;
	}
	else
	{
		int nSize = 0;
		char* sendBuf = new char[MSG_PACK_LENG];
		memset(sendBuf, 0, MSG_PACK_LENG);
		if (m_bInitPlayerPosition)
		{
			nSize = initPlayerPosition(sendBuf);
			m_bInitPlayerPosition = false;
		}
		else
		{
			nSize = getSnakePosition(sendBuf);
			m_bInitPlayerPosition = false;
		}
		for (auto iter : m_MapPlayerData)
		{
			auto PlayerData = iter.second;
			int nSenleng = send(PlayerData->sock, sendBuf, nSize, 0);
			int nStrLen = std::strlen(sendBuf);
			printf("nStrLen = %d", nStrLen);

		}
		delete[]sendBuf;
		sendBuf = NULL;

	}
}

int RetroSnaker::getSnakePosition(char* sendBuf)
{
	int nSize = 4;
	int nHead = HEAD_SNAKE_POSITION;
	memcpy(sendBuf + nSize, &nHead, sizeof(int));
	nSize += sizeof(int);
	int nMsgID = 0;
	memcpy(sendBuf + nSize, &nMsgID, sizeof(int));
	nSize += sizeof(int);
	memcpy(sendBuf + nSize, &m_nReadyNum, sizeof(int));
	nSize += sizeof(int);
	/*点的位置先不变 先不判断碰撞机制*/
	memcpy(sendBuf + nSize, &m_DotPos, sizeof(m_DotPos));
	nSize += sizeof(m_DotPos);

	for (auto iter : m_MapPlayerData)
	{
		auto PlayerData = iter.second;
		if (PlayerData->SnakePosition.size() > 1)
		{
			auto PositionLsat = PlayerData->SnakePosition.end();
			auto PositionHead = PlayerData->SnakePosition.begin();
			*PositionHead = *PositionLsat;
			PlayerData->SnakePosition.insert(PlayerData->SnakePosition.begin() + 2, *PositionLsat);
			PlayerData->SnakePosition.erase(PlayerData->SnakePosition.end());
		}
		auto SnakeHead = PlayerData->SnakePosition.begin();
		switch (PlayerData->nSnakeDirecotion)
		{
		case SnakeHeadUpper:
			SnakeHead->nPositionY += 20;
			break;
		case SnakeHeadRight:
			SnakeHead->nPositionX += 20;
			break;
		case SnakeHeadLower:
			SnakeHead->nPositionY -= 20;
			break;
		case SnakeHeadLeft:
			SnakeHead->nPositionX -= 20;
			break;
		default:
			break;
		}
		int nLeng = PlayerData->SnakePosition.size();
		memcpy(sendBuf + nSize, &nLeng, sizeof(int));
		nSize += sizeof(int);
		memcpy(sendBuf + nSize, &PlayerData->nPlayerID, sizeof(int));
		nSize += sizeof(int);
		for (int i = 0; i < PlayerData->SnakePosition.size(); i++)
		{
			memcpy(sendBuf + nSize, &PlayerData->SnakePosition[i], sizeof(int)* 2);
			nSize += sizeof(int)* 2;
		}
	}
	memcpy(sendBuf, (char*)nSize, sizeof(int));
	return nSize;
}

void RetroSnaker::sendPlayerState(Message::TagSendState* Msg)
{
	char sendBuf[MSG_PACK_LENG] = { 0 };
	Message::TagPlayerData sendPlayerData;
	sendPlayerData.nState = 1;
	sendPlayerData.nMessageHead = HEAD_READY;
	sendPlayerData.nMsgID = 0;
	sendPlayerData.nPlayerID = Msg->nPlayerID;
	auto playerData = m_MapPlayerData.find(Msg->nPlayerID);
	sendPlayerData.chStart = *playerData->second->strPlayerName.c_str();
	sendPlayerData.nMsgLeng = sizeof(int)* 6 + 1 + playerData->second->strPlayerName.size();
	memcpy(sendBuf, &sendPlayerData, sizeof(int)* 6 + 1);
	memcpy(sendBuf + (sizeof(int)* 6 + 1), playerData->second->strPlayerName.c_str() + 1, playerData->second->strPlayerName.size() - 1);
	m_mutexPlayerData.lock();
	playerData->second->nRead = sendPlayerData.nState;
	m_mutexPlayerData.unlock();

	for (auto& iter:m_MapPlayerData)
	{
		auto playerData = iter.second;
		send(playerData->sock, sendBuf, sendPlayerData.nMsgLeng, 0);
	}
}

int RetroSnaker::initPlayerPosition(char* sendBuf)
{
	int nSize = 4;
	int nHead = HEAD_SNAKE_POSITION;
	memcpy(sendBuf + nSize, (char*)&nHead, sizeof(int));
	nSize += sizeof(int);
	m_DotPos = CreatePosition();
	int nMsgID = 0;
	memcpy(sendBuf + nSize, &nMsgID, sizeof(int));
	nSize += sizeof(int);
	memcpy(sendBuf + nSize, &m_nReadyNum, sizeof(int));
	nSize += sizeof(int);
	memcpy(sendBuf + nSize, &m_DotPos, sizeof(m_DotPos));
	nSize += sizeof(m_DotPos);
	for (auto iter : m_MapPlayerData)
	{
		auto playerData = iter.second;
		BodyPosition Pos = CreatePosition();
								
		m_mutexPlayerData.lock();
		playerData->SnakePosition.push_back(Pos);
		if (Pos.nPositionX / 20 >= 8)
		{
			playerData->nSnakeDirecotion = SnakeHeadLeft;
		}
		else
		{
			playerData->nSnakeDirecotion = SnakeHeadRight;
		}
		printf("SnakePosX = %d\n" ,Pos.nPositionX);
		m_mutexPlayerData.unlock();
		int nSnakeSize = 1;
		memcpy(sendBuf + nSize, &nSnakeSize, sizeof(int));
		nSize += sizeof(int);
		memcpy(sendBuf + nSize, &playerData->nPlayerID, sizeof(int));
		nSize += sizeof(int);
		memcpy(sendBuf + nSize, &playerData->nSnakeDirecotion, sizeof(int));
		nSize += sizeof(int);
		for (int i = 0; i < playerData->SnakePosition.size(); i++)
		{
			memcpy(sendBuf + nSize, &playerData->SnakePosition[i].nPositionX, sizeof(int));
			nSize += sizeof(int);
			memcpy(sendBuf + nSize, &playerData->SnakePosition[i].nPositionY, sizeof(int));
			nSize += sizeof(int);
		}
	}
	memcpy(sendBuf,&nSize, sizeof(int));
	return nSize;
}

BodyPosition RetroSnaker::CreatePosition()
{
	srand((unsigned)time(NULL));
	BodyPosition Pos;
	Pos.nPositionX = ((int)rand() % 16) * 20;
	Pos.nPositionY = ((int)rand() % 16) * 20;
	return Pos;
}

void CALLBACK OnTimer(HWND hwnd, UINT uMsg, UINT IDEvent, DWORD DwTime)
{
	RetroSnaker::getInstance()->unPackCallBack();
}

void CALLBACK sendPositionTimer(HWND hwnd, UINT uMsg, UINT IDEvent, DWORD DwTime)
{
	RetroSnaker::getInstance()->sendPosition();
}
 