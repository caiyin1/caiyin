#include "GameData.h"


//唯一静态实例
static GameData* s_global = nullptr;

SOCKET GameData::getSockServer()
{
	if (m_sockServer == SOCKET_ERROR)
	{
		return 0;
	}
	return m_sockServer;
}

void GameData::setSockServer(SOCKET sock)
{
	if (sock != SOCKET_ERROR)
	{
		m_sockServer = sock;
	}
}


bool GameData::init()
{
	bool bRet = false;
	do
	{
		m_sockServer = -1;
		m_MsgID = 0;
		bRet = true;
	} while (0);
	return bRet;
}

GameData* GameData::getInstance()
{
	//判断m_global是否已被实例化，如果没有，将之实例化
	if (s_global == NULL)
	{
		s_global = new GameData;
		CCASSERT(s_global, "ERROR: m_global!");
		s_global->init();
	}
	return s_global;
}

void GameData::setPlayerID(int nPlayerID)
{
	m_nPlayerID = nPlayerID;
}

int GameData::getPlayerID()
{
	return m_nPlayerID;
}

void GameData::setRecBuf(char recBuf[], int nLen)
{
	memset(m_recBuf, 0, MSG_PACK_LENG);
	m_nRecvLen = nLen;
	if (nLen <= 0)
	{
		return;
	}
	memcpy(m_recBuf, recBuf, nLen);
}

char* GameData::getRecBuf()
{
	return m_recBuf;
}

void GameData::setPlayerColour(int nPlayerID, int nPlayerColour)
{
	m_MapPlayerColour.insert(std::make_pair(nPlayerID, nPlayerColour));
}

int GameData::getPlayerColour(int nPlayerID)
{
	auto nColour = m_MapPlayerColour.find(nPlayerID);
	if (nColour == m_MapPlayerColour.end())
	{
		return -1;
	}
	return nColour->second;
}

void GameData::addPlayerStateTask(TagPlayerStateData msg)
{
	m_PlayerDataMutex.lock();
	m_vPlayerState.push_back(msg);
	m_PlayerDataMutex.unlock();
}

bool GameData::isPlayerStateTask()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(m_vPlayerState.empty());
		bRet = true;
	} while (0);
	return bRet;
}

TagPlayerStateData GameData::getPlayerStateTask()
{

	auto msg = *m_vPlayerState.begin();
	m_PlayerDataMutex.lock();
	m_vPlayerState.erase(m_vPlayerState.begin());
	m_PlayerDataMutex.unlock();
	return msg;
}

int GameData::getRecvLen()
{
	return m_nRecvLen;
}

void GameData::deleteRecvData()
{
	std::memset(m_recBuf, 0, MSG_PACK_LENG);
	m_nRecvLen = 0;
}

void GameData::setPlayerData(int nPlayerID, TagPlayerStateData Task)
{
	Task.pLabelName = nullptr;
	m_PlayerDataMutex.lock();
	m_MapPlayerData.insert(std::make_pair(nPlayerID, Task));
	m_PlayerDataMutex.unlock();
}

TagPlayerStateData* GameData::getPlayerData(int nPlayerID)
{
	auto strRet = m_MapPlayerData.find(nPlayerID);
	if (strRet == m_MapPlayerData.end())
	{
		TagPlayerStateData Ret;
		Ret.nPlayerID = -1;
		return &Ret;
	}
	return &strRet->second;

}

void GameData::deletePlayerData(int nPlayerID)
{
	auto strRet = m_MapPlayerData.find(nPlayerID);
	if (strRet == m_MapPlayerData.end())
	{
		return ;
	}
	m_PlayerDataMutex.lock();
	m_MapPlayerData.erase(nPlayerID);
	m_PlayerDataMutex.unlock();
}

void GameData::setSnakePositionTask(TagSnakePosition tagSnakePosition)
{
	m_PositionMutex.lock();
	m_vSnakePosition.push_back(tagSnakePosition);
	m_PositionMutex.unlock();
}

bool GameData::isSnakePositionTask()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!m_vSnakePosition.empty());
		bRet = true;
	} while (0);
	return bRet;
}

TagSnakePosition GameData::getSnakePositionTask()
{
	auto TagRet = *m_vSnakePosition.begin();
	m_PositionMutex.lock();
	m_vSnakePosition.erase(m_vSnakePosition.begin());
	m_PositionMutex.unlock();
	return TagRet;
}

void GameData::setPlayerScore(int nPlayerID, int nScore)
{
	m_PlayerDataMutex.lock();
	auto pIter = m_MapPlayerData.find(nPlayerID);
	auto playerData = &pIter->second;
	playerData->nScore = nScore;
	m_PlayerDataMutex.unlock();
}

void GameData::againInitGameData()
{
	m_PlayerDataMutex.lock();
	int nPlayerID = getPlayerID();
	auto playerData = *getPlayerData(nPlayerID);
	playerData.nScore = 0;
	playerData.nDirection = -1;
	playerData.nState = 0;
	playerData.SnakeBody.clear();
	m_MapPlayerData.clear();
	m_MapPlayerData.insert(std::make_pair(nPlayerID, playerData));
	m_PlayerDataMutex.unlock();

	m_bRecvThread = true;
}

void GameData::deleteThreadRecv()
{
	m_bRecvThread = false;
}
