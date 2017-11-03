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

RetroSnaker* RetroSnaker::GetInstance()
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
	// 初始化随机数组序列
	srand((unsigned)time(NULL));
	do 
	{
		init();
		SOCKET sock = CreateSock();
		m_sock = bindListen(sock, MY_PORT);
		if (m_sock == 0)
		{
			return;
		}
		std::thread acceptThread(&RetroSnaker::acceptCallBack, this);

		std::thread recvThread(&RetroSnaker::recvCallBack, this);
		
		std::thread timerThread(&RetroSnaker::timerCallBack, this);
		
		//std::thread TaskThread(&RetroSnaker::TaskCallBack, this);
		acceptThread.join();
		recvThread.join();
		timerThread.join();
		
	} while (0);
}


void RetroSnaker::acceptCallBack()
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

void RetroSnaker::recvCallBack()
{
	fd_set fdRead;
	FD_ZERO(&fdRead);
	timeval timeOut;
	timeOut.tv_sec = 0.002f;
	timeOut.tv_usec = 0;
	int nContinue = 0;
	while (true)
	{
		makefd(&fdRead);
		nContinue = select(0, &fdRead, NULL, NULL, &timeOut);
		if (nContinue != 0)
		{
			for (int i = 0; i < fdRead.fd_count; i++)
			{
				if (FD_ISSET(fdRead.fd_array[i], &fdRead))
				{
					m_mutexRecv.lock();
					//根据套接字查找 对应的nPlayerID
					int nPlayerID = getPlayerID(fdRead.fd_array[i]);
					auto PlayerData = m_MapPlayerData.find(nPlayerID)->second;
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

char* RetroSnaker::DeleteMessage(char chRecBuf[], int nMsgLen, int nBufLen)
{
	for (int i = 0; nMsgLen + i < nBufLen; i++)
	{
		chRecBuf[i] = chRecBuf[i + nMsgLen];
	}
	memset(chRecBuf + (nBufLen - nMsgLen), 0, 1024 - (nBufLen - nMsgLen));
	printf("(DeleteMessage) nBufLen - bMsgLen = %d", nBufLen - nMsgLen);
	return chRecBuf;
}

void RetroSnaker::handleTask(Message::TagMsgHead msg, char* pChMsg, int nPlayerID)
{
	switch (msg.nMessageHead)
	{
	case HEAD_LOGIN:
		{
			Message::TagPlayerData *PlayerData = (Message::TagPlayerData*) pChMsg;
			PlayerData->nPlayerID = nPlayerID;
			char* p = &PlayerData->chStart;
			std::string strPlayerName = p;
// 			for (; *p != 0; p++)
// 			{
// 				strPlayerName += *p;
//  			}
			responseSignIn(PlayerData, strPlayerName);
		}
			break;
	case HEAD_READY:
		{
			m_nReadyNum++;
			Message::TagSendState* pSendState = (Message::TagSendState*) pChMsg;
			sendPlayerState(pSendState);
		}
			break;
	case HEAD_PLAYER_DATA:
										
		break;
	case HEAD_DIRECTION:
		{
			Message::TagSnakeHeadDirection* pSnakeDirection = (Message::TagSnakeHeadDirection*) pChMsg;
			m_mutexPlayerData.lock();
			auto playerData = m_MapPlayerData.find(pSnakeDirection->nPlayerID);
			playerData->second->nSnakeDirecotion = pSnakeDirection->nDirection;
			m_mutexPlayerData.unlock();
		}
		break;
	}
}

int RetroSnaker::createPlayerID()
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
				char* pChMsg = new char[MsgHead.nMsgLeng];
				std::memset(pChMsg, 0, MsgHead.nMsgLeng);
				memcpy(pChMsg, playerData.second->chRecvBuf, MsgHead.nMsgLeng);
				m_mutexRecv.lock();
				*playerData.second->chRecvBuf = *DeleteMessage(playerData.second->chRecvBuf, MsgHead.nMsgLeng, playerData.second->nRecvSize);
				playerData.second->nRecvSize -= MsgHead.nMsgLeng;
				m_mutexRecv.unlock();
				handleTask(MsgHead, pChMsg, playerData.second->nPlayerID);
				delete[] pChMsg;
				pChMsg = NULL;
			}
		}
	}
}

void RetroSnaker::initPlayer(SOCKET sock)
{
	auto pPlayerData = new g_TagPlayerData;
	//g_TagPlayerData PlayerData = *p;
	pPlayerData->sock = sock;
	int nPlayerID = createPlayerID();
	pPlayerData->nRecvSize = 0;
	pPlayerData->nPlayerID = nPlayerID;
	pPlayerData->nColour = -1;
	pPlayerData->nRead = -1;
	pPlayerData->nSnakeDirecotion = -1;
	memset(pPlayerData->chRecvBuf, 0, MSG_PACK_LENG);
	m_mutexPlayerData.lock();
	m_MapPlayerData.insert(std::make_pair(nPlayerID, pPlayerData));
	m_mutexPlayerData.unlock();
}

void RetroSnaker::responseSignIn(Message::TagPlayerData *pMsg, std::string strPlayerName)
{
	Message::TagPlayerData sendData;
	sendData.nColour = initColour();
	sendData.nMessageHead = HEAD_LOGIN;
	sendData.nMsgID = pMsg->nMsgID;
	sendData.nPlayerID = pMsg->nPlayerID;
	sendData.chStart = *strPlayerName.c_str();
	sendData.nState = 0;
	sendData.nMsgLeng = sizeof(int)* 6 + 1 +  strPlayerName.size();
	char chSendBuf[MSG_PACK_LENG] = { 0 };
	memcpy(chSendBuf, &sendData, sizeof(int)* 6 + 1);
	memcpy(chSendBuf + sizeof(int)* 6 + 1, strPlayerName.c_str() + 1, strPlayerName.size() - 1);
	m_mutexPlayerData.lock();
	auto playerData = m_MapPlayerData.find(pMsg->nPlayerID);
	playerData->second->strPlayerName = strPlayerName;
	playerData->second->nColour = sendData.nColour;
	m_mutexPlayerData.unlock();
	auto sockClient = playerData->second->sock;
	send(sockClient, chSendBuf, sendData.nMsgLeng, 0);
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

void RetroSnaker::timerCallBack()
{
	MSG msg;
	// PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);

	UINT timerid = SetTimer(NULL, 1, 10, onTimer);
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
	//首先将fd_list清0
	FD_ZERO(fd_list);  
	//将每一个socket加入fd_list中  
	for (auto& iter : m_MapPlayerData)
	{
		FD_SET(iter.second->sock, fd_list);
	}
}

int RetroSnaker::getPlayerID(SOCKET sock)
{
	for (const auto& iter : m_MapPlayerData)
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
		char* pSendBuf = new char[MSG_PACK_LENG];
		memset(pSendBuf, 0, MSG_PACK_LENG);
		if (m_bInitPlayerPosition)
		{
			nSize = initPlayerPosition(pSendBuf);
			m_bInitPlayerPosition = false;
		}
		else
		{
			nSize = getSnakePosition(pSendBuf);
			m_bInitPlayerPosition = false;
		}
		for (const auto& iter : m_MapPlayerData)
		{
			auto PlayerData = iter.second;
			int nSenleng = send(PlayerData->sock, pSendBuf, nSize, 0);
		}
		delete[]pSendBuf;
		pSendBuf = NULL;
	}
}

int RetroSnaker::getSnakePosition(char* pSendBuf)
{
	
	int nSize = 4;
	int nHead = HEAD_SNAKE_POSITION;
	memcpy(pSendBuf + nSize, &nHead, sizeof(int));
	nSize += sizeof(int);
	int nMsgID = 0;
	memcpy(pSendBuf + nSize, &nMsgID, sizeof(int));
	nSize += sizeof(int);
	memcpy(pSendBuf + nSize, &m_nReadyNum, sizeof(int));
	nSize += sizeof(int);
	/*点的位置先不变 先不判断碰撞机制*/
	memcpy(pSendBuf + nSize, &m_DotPos.nPositionX, sizeof(int));
	nSize += sizeof(int);
	memcpy(pSendBuf + nSize, &m_DotPos.nPositionY, sizeof(int));
	nSize += sizeof(int);

	for (auto iter : m_MapPlayerData)
	{
		auto playerData = iter.second;
		if (playerData->vSnakePosition.size() > 1)
		{
			auto PositionLsat = playerData->vSnakePosition.end();
			auto PositionHead = playerData->vSnakePosition.begin();
			*PositionHead = *PositionLsat;
			playerData->vSnakePosition.insert(playerData->vSnakePosition.begin() + 2, *PositionLsat);
			playerData->vSnakePosition.erase(playerData->vSnakePosition.end());
		}
		
		auto SnakeHead = playerData->vSnakePosition.begin();
		switch (playerData->nSnakeDirecotion)
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
		}
		int nLeng = playerData->vSnakePosition.size();
		memcpy(pSendBuf + nSize, &nLeng, sizeof(int));
		nSize += sizeof(int);
		memcpy(pSendBuf + nSize, &playerData->nPlayerID, sizeof(int));
		nSize += sizeof(int);
		memcpy(pSendBuf + nSize, &playerData->nSnakeDirecotion, sizeof(int));
		nSize += sizeof(int);
		for (int i = 0; i < playerData->vSnakePosition.size(); i++)
		{
			memcpy(pSendBuf + nSize, &playerData->vSnakePosition[i], sizeof(int)* 2);
			nSize += sizeof(int)* 2;
		}
	}
	memcpy(pSendBuf, &nSize, sizeof(int));
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

	for (const auto& iter:m_MapPlayerData)
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
	m_DotPos = createPosition();
	int nMsgID = 0;
	memcpy(sendBuf + nSize, &nMsgID, sizeof(int));
	nSize += sizeof(int);
	memcpy(sendBuf + nSize, &m_nReadyNum, sizeof(int));
	nSize += sizeof(int);
	memcpy(sendBuf + nSize, &m_DotPos, sizeof(m_DotPos));
	nSize += sizeof(m_DotPos);
	m_mutexPlayerData.lock();
	for (auto iter : m_MapPlayerData)
	{
		auto playerData = iter.second;
		BodyPosition pos = createPosition();
								
		playerData->vSnakePosition.push_back(pos);
		if (pos.nPositionX / 20 >= 8)
		{
			playerData->nSnakeDirecotion = SnakeHeadLeft;
		}
		else
		{
			playerData->nSnakeDirecotion = SnakeHeadRight;
		}
		printf("SnakePosX = %d\n" ,pos.nPositionX);
		const auto& pPos = playerData->vSnakePosition.front();
	    // printf("PlayerData.SnakePosition.x =  d%\n", playerData->SnakePosition.begin()->nPositionX);
		printf("PlayerData.SnakePosition.x =  d%\n", pos.nPositionX);
		/*初始化时 蛇的身体长度为 1 */
		int nSnakeSize = 1;
		memcpy(sendBuf + nSize, &nSnakeSize, sizeof(int));
		nSize += sizeof(int);
		memcpy(sendBuf + nSize, &playerData->nPlayerID, sizeof(int));
		nSize += sizeof(int);
		memcpy(sendBuf + nSize, &playerData->nSnakeDirecotion, sizeof(int));
		nSize += sizeof(int);
		for (int i = 0; i < playerData->vSnakePosition.size(); i++)
		{
			memcpy(sendBuf + nSize, &playerData->vSnakePosition[i].nPositionX, sizeof(int));
			nSize += sizeof(int);
			memcpy(sendBuf + nSize, &playerData->vSnakePosition[i].nPositionY, sizeof(int));
			nSize += sizeof(int);
		}
	}
	m_mutexPlayerData.unlock();
	memcpy(sendBuf,&nSize, sizeof(int));
	return nSize;
}

BodyPosition RetroSnaker::createPosition()
{
	BodyPosition Pos;
	Pos.nPositionX = ((int)rand() % 16) * 20;
	Pos.nPositionY = ((int)rand() % 16) * 20;
	return Pos;
}

void CALLBACK onTimer(HWND hwnd, UINT uMsg, UINT IDEvent, DWORD DwTime)
{
	RetroSnaker::GetInstance()->unPackCallBack();
}

void CALLBACK sendPositionTimer(HWND hwnd, UINT uMsg, UINT IDEvent, DWORD DwTime)
{
	RetroSnaker::GetInstance()->sendPosition();
}
 