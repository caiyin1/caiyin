#include <iostream> 
#include <thread>
#include <time.h>
#include<stdio.h>
#include<stdlib.h>
#include "RetroSnaker.h"

#define COLOR3B *(int*)&Color4B

#pragma warning( disable : 4996)
using namespace std;

struct RetroSnaker* s_global = nullptr;

const static std::unordered_map<int, int> g_MapColour{
	//��ɫ
	std::make_pair(1, COLOR3B(255,0,0)),
	//��ɫ
	std::make_pair(2, COLOR3B(238,64,0)),
	//��ɫ
	std::make_pair(3, COLOR3B(238, 138, 238)),
	//��ɫ
	std::make_pair(4, COLOR3B(99, 84, 205)),
	//ӫ��ɫ
	std::make_pair(5, COLOR3B(79, 255, 159)),
	//����ɫ
	std::make_pair(6, COLOR3B(47, 205, 47)),
	//��ɫ
	std::make_pair(7, COLOR3B(205, 36, 36)),
	//��ɫ
	std::make_pair(8, COLOR3B(191, 62, 255)),
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
	// ��ʼ�������������
	srand((unsigned)time(NULL));
	do 
	{
		init();
		SOCKET sock = CreateSock();
		m_sock = bindListen(sock, MY_PORT);
		//��ʼ����Ϸ��������
		initGameData();
		if (m_sock == 0)
		{
			return;
		}
		//���IP
		getIP();
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
					//��Ҫ������Ϣ �ż���
					m_mutexPlayerData.lock();
					//�����׽��ֲ��� ��Ӧ��nPlayerID
					int nPlayerID = getPlayerID(fdRead.fd_array[i]);
					auto playerData = m_MapPlayerData.find(nPlayerID)->second;
					int nLen = recv(fdRead.fd_array[i], playerData->chRecvBuf, MSG_PACK_LENG, 0);
					
					if (nLen == 0 || nLen == SOCKET_ERROR)
					{
						
						closesocket(fdRead.fd_array[i]);
						FD_CLR(fdRead.fd_array[i], &fdRead);
					
						m_MapPlayerData.erase(nPlayerID);
						delete playerData;
			
						//��ѭ��ǰ����
						m_mutexPlayerData.unlock();
						continue;
					}
					//������Ϣˢ����������ֵ
					playerData->nHeartBeat = 0;
					playerData->nRecvSize += nLen;
					m_mutexPlayerData.unlock();
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
	return chRecBuf;
}

void RetroSnaker::handleTask(Message::TagMsgHead msg, char* pChMsg, int nPlayerID)
{
	//�������ĺ����Ѿ������� �������治�ܼ���
	switch (msg.nMessageHead)
	{
	case HEAD_LOGIN:
		{
			Message::TagPlayerData *PlayerData = (Message::TagPlayerData*) pChMsg;
			PlayerData->nPlayerID = nPlayerID;
			char* p = &PlayerData->chStart;
			std::string strPlayerName = p;
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
			
			auto playerData = m_MapPlayerData.find(pSnakeDirection->nPlayerID);
			playerData->second->nSnakeDirecotion = pSnakeDirection->nDirection;
		}
		break;
	case HEAD_HEARTBEAT:
		{
			// ���������������账��
				break;
		}
	}
}

int RetroSnaker::createPlayerID()
{
	return m_nPlayerID++;
}

void RetroSnaker::handlRecvCallBack()
{ 
	// ��ֹ���� �����ݱ������̸߳ı�
	m_mutexPlayerData.lock();
	for (auto playerData : m_MapPlayerData) 
	{
		if (playerData.second->nRecvSize > sizeof(int) * 3)
		{
			Message::TagMsgHead MsgHead = *(Message::TagMsgHead*)playerData.second->chRecvBuf;
			if (MsgHead.nMsgLeng <= playerData.second->nRecvSize)
			{
				char* pChMsg = new char[MsgHead.nMsgLeng];
				std::memset(pChMsg, 0, MsgHead.nMsgLeng);
				memcpy(pChMsg, playerData.second->chRecvBuf, MsgHead.nMsgLeng);
				
				*playerData.second->chRecvBuf = *DeleteMessage(playerData.second->chRecvBuf, MsgHead.nMsgLeng, playerData.second->nRecvSize);
				playerData.second->nRecvSize -= MsgHead.nMsgLeng;
				handleTask(MsgHead, pChMsg, playerData.second->nPlayerID);
				delete[] pChMsg;
				pChMsg = NULL;
			}
		}
	}
	m_mutexPlayerData.unlock();
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
	pPlayerData->nPlayerState = -1;
	pPlayerData->nSnakeDirecotion = -1;
	pPlayerData->nHeartBeat = 0;
	memset(pPlayerData->chRecvBuf, 0, MSG_PACK_LENG);
	m_mutexPlayerData.lock();
	m_MapPlayerData.insert(std::make_pair(nPlayerID, pPlayerData));
	m_mutexPlayerData.unlock();
}

void RetroSnaker::responseSignIn(Message::TagPlayerData *pMsg, std::string strPlayerName)
{
	//�������ĺ����Ѿ������� �������治�ܼ���
	Message::TagPlayerData sendData;
	sendData.nColour = createColour();
	sendData.nMessageHead = HEAD_LOGIN;
	sendData.nMsgID = pMsg->nMsgID;
	sendData.nPlayerID = pMsg->nPlayerID;
	sendData.chStart = *strPlayerName.c_str();
	sendData.nState = 0;
	sendData.nMsgLeng = sizeof(int)* 6 + 1 +  strPlayerName.size();
	char chSendBuf[MSG_PACK_LENG] = { 0 };
	memcpy(chSendBuf, &sendData, sizeof(int)* 6 + 1);
	memcpy(chSendBuf + sizeof(int)* 6 + 1, strPlayerName.c_str() + 1, strPlayerName.size() - 1);
	auto playerData = m_MapPlayerData.find(pMsg->nPlayerID);
	playerData->second->strPlayerName = strPlayerName;
	playerData->second->nColour = sendData.nColour;
	auto sockClient = playerData->second->sock;
	send(sockClient, chSendBuf, sendData.nMsgLeng, 0);
}

int RetroSnaker::createColour()
{
	if (m_nColour >= 7 || m_nColour <= 0)
	{
		m_nColour = 1;
	}
	int nNum = 1;
	for (const auto& iter : m_MapPlayerData)
	{
		auto playerData = iter.second;
		if (playerData->nColour != g_MapColour.find(nNum)->second)
		{
			break;
		}
		nNum++;
	}
	auto nColour = g_MapColour.find(nNum);
	if (nColour == g_MapColour.end())
	{
		return -1;
	}
	Color4B* color = (Color4B*)& nColour->second;
	return nColour->second;
}

void RetroSnaker::timerCallBack()
{
	MSG msg;
	// PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);

	m_timerid = SetTimer(NULL, 1, 10, onTimer);
	//��Ϸ��ʼ�ж϶�ʱ��
	m_gameStartTimerId = SetTimer(NULL, 4, 1000, gameStartTimer);


	UINT heartBeatTimerId = SetTimer(NULL, 3, 20000, handleHeartBeatTimer);
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
	KillTimer(NULL, m_timerid);
	KillTimer(NULL, m_gameLogicTimerId);
	printf("thread end here/n");
}

void RetroSnaker::makefd(fd_set *fd_list)
{
	//���Ƚ�fd_list��0
	FD_ZERO(fd_list);  
	//��ÿһ��socket����fd_list��  
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

void RetroSnaker::handlGameLogic()
{
	switch (m_gameState)
	{
	case readyState:
		{	
			//�ı���Ϸ״̬ ��Ϊ��ʼ״̬
			m_gameState = inGameState;
			//�ı����״̬
			m_mutexPlayerData.lock();
			// ���4�����ͬʱ��
			int nNum = 0;
			for (auto iter : m_MapPlayerData)
			{
				auto playerData = iter.second;
				if (playerData->nPlayerState == 1)
				{
					playerData->nPlayerState = 2;
					nNum++;
					if (nNum >=PLAYER_NUM )
					{
						break;
					}
				}
			}
			m_nGameNum = nNum;
			m_mutexPlayerData.unlock();

			//����һ�������Ϸ�Ƿ�����Ķ�ʱ��
			m_gameOverTimerId = SetTimer(NULL, 3, 500, isGameOver);
		}
	case inGameState:
		{
			int nSize = 0;
			char* pSendBuf = new char[MSG_PACK_LENG];
			memset(pSendBuf, 0, MSG_PACK_LENG); 
			if (m_bInitPlayerPosition)
			{
				nSize = initPlayerPosData(pSendBuf);
				m_bInitPlayerPosition = false;
			}
			else
			{
				setGameData();
				m_mutexPlayerData.lock();
				// ������ȡ����ǰ����
				for (auto iter : m_MapPlayerData)
				{
					auto playerData = iter.second;
					// �ж����״̬ 
					if (playerData->nPlayerState != 2)
					{
						continue;
					}
					playerData->nPlayerState = isSnakeImpack(playerData->nPlayerID);
				}
				nSize = setSnakePositionPack(pSendBuf);
				m_bInitPlayerPosition = false;
				m_mutexPlayerData.unlock();
			}
			m_mutexPlayerData.lock();
			for (const auto& iter : m_MapPlayerData)
			{
				auto playerData = iter.second;
				//Ϊ׼����Ҳ��ܽ�����Ϣ
				if (playerData->nPlayerState == -1)
				{
					continue;
				}
				int nSenleng = send(playerData->sock, pSendBuf, nSize, 0);
			}
			m_mutexPlayerData.unlock();
			delete[]pSendBuf;
			pSendBuf = NULL;
		}

		break;
	}
	
}

int RetroSnaker::setSnakePositionPack(char* pSendBuf)
{
	/*
	*�������ĺ����Ѿ������� �������治�ܼ���
	*/
	int nSize = 4;
	int nHead = HEAD_SNAKE_POSITION;
	memcpy(pSendBuf + nSize, &nHead, sizeof(int));
	nSize += sizeof(int);
	int nMsgID = 0;
	memcpy(pSendBuf + nSize, &nMsgID, sizeof(int));
	nSize += sizeof(int);
	//�����ߵ�����
	
	int nSnakeNumPos = nSize;
	int nPlayeGameNum = 0;
	nSize += sizeof(int);
	/*���λ���Ȳ��� �Ȳ��ж���ײ����*/
	memcpy(pSendBuf + nSize, &m_DotPos.nPositionX, sizeof(int));
	nSize += sizeof(int);
	memcpy(pSendBuf + nSize, &m_DotPos.nPositionY, sizeof(int));
	nSize += sizeof(int);
	//����ǰ��Ҫ����
	for (auto iter : m_MapPlayerData)
	{
		const auto& playerData = iter.second;
		int nLeng = playerData->vSnakePosition.size();
		//�ж����Ƿ� ���� ������д���µ�λ��
		if (playerData->nPlayerState != 2)
		{
			continue;
		}
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
		nPlayeGameNum++;
	}
	//д����Ϸ�� �ߵ�����
	memcpy(pSendBuf + nSnakeNumPos, &nPlayeGameNum, sizeof(int));
	memcpy(pSendBuf, &nSize, sizeof(int));
	return nSize;
}

void RetroSnaker::sendPlayerState(Message::TagSendState* Msg)
{
	//�������ĺ����Ѿ������� �������治�ܼ��� 
	char sendBuf[MSG_PACK_LENG] = { 0 };
	Message::TagPlayerData sendPlayerData;
	sendPlayerData.nState = 1;
	sendPlayerData.nMessageHead = HEAD_READY;
	sendPlayerData.nMsgID = 0;
	sendPlayerData.nPlayerID = Msg->nPlayerID;
	auto playerData = m_MapPlayerData.find(Msg->nPlayerID);
	sendPlayerData.nColour = playerData->second->nColour;
	sendPlayerData.chStart = *playerData->second->strPlayerName.c_str();
	sendPlayerData.nMsgLeng = sizeof(int)* 6 + 1 + playerData->second->strPlayerName.size();
	memcpy(sendBuf, &sendPlayerData, sizeof(int)* 6 + 1);
	memcpy(sendBuf + (sizeof(int)* 6 + 1), playerData->second->strPlayerName.c_str() + 1, playerData->second->strPlayerName.size() - 1);
	
	playerData->second->nPlayerState = sendPlayerData.nState;

	for (const auto& iter : m_MapPlayerData)
	{
		auto playerData = iter.second;
		send(playerData->sock, sendBuf, sendPlayerData.nMsgLeng, 0);
	}
	answerReadyPlayer(Msg->nPlayerID);
}

void RetroSnaker::answerReadyPlayer(int nPlayerID)
{
	/*
	*�������ĺ����Ѿ������� �������治�ܼ���
	*/
	//����ǰ����
	for (const auto& iter : m_MapPlayerData)
	{
		auto playerData = iter.second;
		//������Ϊ׼���� �Լ����Լ��Ѿ����͹��ˣ�
		if (playerData->nPlayerState == -1 || playerData->nPlayerID == nPlayerID)
		{
			continue;
		}
		char sendBuf[MSG_PACK_LENG] = { 0 };
		Message::TagPlayerData sendPlayerData;
		sendPlayerData.nState = 1;
		sendPlayerData.nMessageHead = HEAD_READY;
		sendPlayerData.nMsgID = 0;
		sendPlayerData.nPlayerID = playerData->nPlayerID;
		sendPlayerData.chStart = *playerData->strPlayerName.c_str();
		sendPlayerData.nColour = playerData->nColour;
		sendPlayerData.nMsgLeng = sizeof(int) * 6 + 1 + playerData->strPlayerName.size();
		memcpy(sendBuf, &sendPlayerData, sizeof(int) * 6 + 1);
		memcpy(sendBuf + (sizeof(int) * 6 + 1), playerData->strPlayerName.c_str() + 1, playerData->strPlayerName.size() - 1);
		auto sock = m_MapPlayerData.find(nPlayerID)->second->sock;
		int nLen = send(sock, sendBuf, sendPlayerData.nMsgLeng, 0);
	}
}

int RetroSnaker::initPlayerPosData(char* sendBuf)
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
		//�����ж�ֻ�����ŵ����ˢ������
		if (playerData->nPlayerState != 2)
		{
			continue;
		}
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
		/*��ʼ��ʱ �ߵ����峤��Ϊ 1 */
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
	if (Pos.nPositionX == 0)
	{
		Pos.nPositionX += 10;
	}
	else
	{
		Pos.nPositionX -= 10;
	}
	if (Pos.nPositionY == 0)
	{
		Pos.nPositionY += 10;
	}
	else
	{
		Pos.nPositionY -= 10;
	}
	return Pos;
}

void RetroSnaker::setGameData()
{
	m_mutexPlayerData.lock();
	for (auto iter : m_MapPlayerData)
	{
		auto playerData = iter.second;
		//�ж����״̬
		if (playerData->nPlayerState != 2)
		{
			continue;
		}
		//�Ƚ��а�ȫ�ж�
		if (playerData->vSnakePosition.empty())
		{
			printf("ERROR : (setGameData) playerData->vSnakePosition = 0!!");
			//�Ƚ���
			m_mutexPlayerData.unlock();
			return;
		}
		auto PositionHead = playerData->vSnakePosition.begin();		
		auto PositionSecond = *PositionHead;
		playerData->vSnakePosition.insert(playerData->vSnakePosition.begin() + 1, PositionSecond);
	
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
		if (!isDotImpack(playerData->nPlayerID))
		{
			playerData->vSnakePosition.pop_back();
			printf("(setGameData) DotPos.x = %d\tDotPos.y = %d\n", m_DotPos.nPositionX, m_DotPos.nPositionY);
		}
	}
	//�����ײDot��λ�ú��߱ض����غ�
	while (!isDotImpack())
	{
		m_DotPos = createPosition();
	}
	m_mutexPlayerData.unlock();
}

bool RetroSnaker::isDotImpack(int nPlayerID)
{
	/*
	*�������ĺ����Ѿ������� �������治�ܼ���
	*/
	bool bRet = false;
	auto iter = m_MapPlayerData.find(nPlayerID);
	auto playerData = iter->second;
	for (int i = 0; i < playerData->vSnakePosition.size(); i++)
	{
		if (m_DotPos.nPositionX == playerData->vSnakePosition[i].nPositionX && m_DotPos.nPositionY == playerData->vSnakePosition[i].nPositionY)
		{
			bRet = true;
			return bRet;
		}
	}
	return bRet;
}

bool RetroSnaker::isDotImpack()
{
	/*
	*�������ĺ����Ѿ������� �������治�ܼ���
	*/
	bool bRet = true;
	for (auto iter : m_MapPlayerData)
	{
		auto playerData = iter.second;
		for (int i = 0; i < playerData->vSnakePosition.size(); i++)
		{
			if (m_DotPos.nPositionX == playerData->vSnakePosition[i].nPositionX && m_DotPos.nPositionY == playerData->vSnakePosition[i].nPositionY)
			{
				bRet = false;
				return bRet;
			}
		}
	}
	return bRet;
}

int RetroSnaker::isSnakeImpack(int nPlayerID)
{	
	/*
	*�������ĺ����Ѿ������� �������治�ܼ���
	*/
	int nRet = 0;
	//��ȡ��ͷ��λ����Ϣ
	auto snakeHeadPos = *m_MapPlayerData.find(nPlayerID)->second->vSnakePosition.begin();
	if (snakeHeadPos.nPositionX < 10 || snakeHeadPos.nPositionX > LATTICE_SIZE * LATTCE_NUM - 10|| snakeHeadPos.nPositionY < 10 || snakeHeadPos.nPositionY > LATTICE_SIZE * LATTCE_NUM -10)
	{
		m_MapPlayerData.find(nPlayerID)->second->nPlayerState = 0;
		//��ײ�������1
		m_nGameNum -= 1;
		return nRet;
	}
	for (auto iter : m_MapPlayerData)
	{
		auto playerData = iter.second;
		// �жϼ������Ƿ��� ��Ϸ�еĶ���
		if (playerData->nPlayerState != 2)
		{
			continue;
		}
		int i = 0;
		// �����������Լ�ʱ������ͷ�ļ��
		if (nPlayerID == playerData->nPlayerID)
		{
			i++;
		}
		for (; i < playerData->vSnakePosition.size(); i++)
		{
			if (snakeHeadPos.nPositionX == playerData->vSnakePosition[i].nPositionX && snakeHeadPos.nPositionY == playerData->vSnakePosition[i].nPositionY)
			{
				m_MapPlayerData.find(nPlayerID)->second->nPlayerState = 0;
				//��ײ�������1
				m_nGameNum -= 1;
				return nRet;
			}
		}
	}
	nRet = 2;
	return nRet;
}

void RetroSnaker::sendPlayerScore()
{
	char sendBuf[MSG_PACK_LENG] = { 0 };
	//������ͷ12λ�İ��� 
	int nSize = 12;
	Message::TagMsgHead tagHead;
	tagHead.nMessageHead = HEAD_GAME_SCORE;
	tagHead.nMsgID = 0;
	
	//����������ֵ����
	nSize += sizeof(int);
	int nPlayeGameNum = 0;
	m_mutexPlayerData.lock();
	for (const auto& iter : m_MapPlayerData)
	{
		auto playerData = iter.second;
		//δ�μ���Ϸ���� ��û�гɼ�
		if (playerData->nPlayerState == 0)
		{
			memcpy(sendBuf + nSize, &playerData->nPlayerID, sizeof(int));
			nSize += sizeof(int);
			int nScore = playerData->vSnakePosition.size();
			memcpy(sendBuf + nSize, &nScore, sizeof(int));
			nSize += sizeof(int);
			nPlayeGameNum++;
		}
	}
	//�����Ϸ����
	memcpy(sendBuf + 12, &nPlayeGameNum, sizeof(int));
	//��Ӱ�ͷ
	tagHead.nMsgLeng = nSize;
	memcpy(sendBuf, &tagHead, 12);
	//�����Ѱ����ݷ��͸��������
	for (const auto& iter : m_MapPlayerData)
	{
		auto playerData = iter.second;
		//Ϊ׼����Ҳ��ܽ�����Ϣ
		if (playerData->nPlayerState == -1)
		{
			continue;
		}
		send(playerData->sock, sendBuf, nSize, 0);
	}
	m_mutexPlayerData.unlock();
}

void RetroSnaker::gameOverLogic()
{
	int nPlayeGameNum = 0;
	m_mutexPlayerData.lock();
	for (const auto& iter : m_MapPlayerData)
	{
		auto playerData = iter.second;
		if (playerData->nPlayerState == 2)
		{
			nPlayeGameNum++;
		}
	}
	m_mutexPlayerData.unlock();
	if (m_nGameNum > 0 && nPlayeGameNum )
	{
		return;
	}

	sendPlayerScore();
	//��ʼ����Ϸ����
	againInitGameData();
	//��Ϸ�����ر� isGameOver ��ʱ��
	KillTimer(NULL, m_gameOverTimerId);	
	//�ر����߼���ʱ��
	KillTimer(NULL, m_gameLogicTimerId);
	//��Ϸ��ʼ�ж϶�ʱ��
	m_gameStartTimerId = SetTimer(NULL, 4, 1000, gameStartTimer);
}

void RetroSnaker::heartLogic()
{
	m_mutexPlayerData.lock();
	for (auto iter:m_MapPlayerData)
	{
		auto playerData = iter.second;
		if (playerData->nHeartBeat > 2)
		{
			closeSocket(playerData->sock);
		}
		playerData->nHeartBeat++;
	}
	m_mutexPlayerData.unlock();
}

void RetroSnaker::startJudge()
{
	if (m_nReadyNum > 0)
	{	
		m_nCountDown--;
		//׼����ҳ���4�� ֱ�ӿ�ʼ
		if (m_nReadyNum >= PLAYER_NUM)
		{
			m_nCountDown = 0;
		}
		//����ʣ��ʱ��
		char sendBuf[MSG_PACK_LENG] = { 0 };
		Message::TagMsgHead head;
		head.nMessageHead = HEAD_COUNTDOWN;
		head.nMsgID = 0;
		head.nMsgLeng = sizeof(int) * 4;
		int nHeadLen = sizeof(int) * 3;
		memcpy(sendBuf, &head, sizeof(int) * nHeadLen);
		memcpy(sendBuf + nHeadLen, &m_nCountDown, sizeof(int));
		m_mutexPlayerData.lock();
		for (const auto& iter: m_MapPlayerData)
		{
			auto playerData = iter.second;
			//��������ҷ��ͼ�ʱ����Ϣ
			send(playerData->sock, sendBuf, head.nMsgLeng, 0);
		}
		m_mutexPlayerData.unlock();
	}
	else
	{
		//����û�����ʱ ������ˢ�¼�ʱʱ��
		m_nCountDown = 10;
	}
	if (m_nCountDown == 0)
	{
		m_nCountDown = 10;
		KillTimer(NULL, m_gameStartTimerId);
		//�������� ��ʼ��Ϸ
		m_gameLogicTimerId = SetTimer(NULL, 2, 500, gameLogicTimer);
	}
}

void RetroSnaker::againInitGameData()
{
	m_mutexPlayerData.lock();
	for (auto iter : m_MapPlayerData)
	{
		auto playerData = iter.second;
		playerData->nPlayerState = -1;
		
		if (playerData->vSnakePosition.empty())
		{
			continue;
		}
		playerData->vSnakePosition.clear();
	}
	m_mutexPlayerData.unlock();
	m_nReadyNum = 0;
	m_nGameNum = -1;
	m_bInitPlayerPosition = true;
	m_gameState = readyState;
}

void RetroSnaker::initGameData()
{
	m_nClientNum = 0;
	m_nColour = 0;
	m_nReadyNum = 0;
	m_nGameNum = -1;
	m_bInitPlayerPosition = true;
	m_nPlayerID = 1001;
	m_gameState = readyState;
	m_nCountDown = 10;

}


void RetroSnaker::getIP()
{
	//2.��ȡ������  
	char hostname[256];
	 int ret = gethostname(hostname, sizeof(hostname));
	if (ret == SOCKET_ERROR)
	{
		return;
	}
	//3.��ȡ����ip  
	HOSTENT* host = gethostbyname(hostname);
	if (host == NULL)
	{
		return;
	}
	//4.ת��Ϊchar*����������  
	char ChIp[32] = { 0 };
	strcpy(ChIp, inet_ntoa(*(in_addr*)*host->h_addr_list));
	printf("����IP ��%s\n", ChIp);
}

void CALLBACK onTimer(HWND hwnd, UINT uMsg, UINT IDEvent, DWORD DwTime)
{
	RetroSnaker::GetInstance()->handlRecvCallBack();
}

void CALLBACK gameLogicTimer(HWND hwnd, UINT uMsg, UINT IDEvent, DWORD DwTime)
{
	RetroSnaker::GetInstance()->handlGameLogic();
}

void CALLBACK isGameOver(HWND hwnd, UINT uMsg, UINT IDEvent, DWORD DwTime)
{
	RetroSnaker::GetInstance()->gameOverLogic();
}

void CALLBACK handleHeartBeatTimer(HWND hwnd, UINT uMsg, UINT IDEvent, DWORD DwTime)
{
	RetroSnaker::GetInstance()->heartLogic();
}

void CALLBACK gameStartTimer(HWND hwnd, UINT uMsg, UINT IDEvent, DWORD DwTime)
{
	RetroSnaker::GetInstance()->startJudge();
}

