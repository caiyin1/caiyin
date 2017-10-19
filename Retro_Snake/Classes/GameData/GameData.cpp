#include "GameData.h"

//唯一静态实例
static GameData* s_global = nullptr;

SOCKET GameData::getSockServer()
{
	if (getSockServer() == SOCKET_ERROR)
	{
		return 0;
	}
	return getSockServer();
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

GameData* GameData::shareGlobal()
{
	return s_global;//GameData::getInstance();

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
	if (strlen(recBuf) < 0)
	{
		return;
	}
}

char* GameData::getRecBuf()
{
	return m_recBuf;
}

void GameData::setPlayerColour(int nPlayerID, int nPlayerColour)
{
	m_PlayerColour.insert(std::make_pair(nPlayerID, nPlayerColour));
}

int GameData::getPlayerColour(int nPlayerID)
{
	auto nColour = m_PlayerColour.find(nPlayerID);
	if (nColour == m_PlayerColour.end())
	{
		return -1;
	}
	return nColour->second;
}

void GameData::setPlayerName(int nPlayerID, const std::string& strPlayerName)
{
	m_PlayerName.insert(std::make_pair(nPlayerID, strPlayerName));
}

std::string GameData::getPlayerName(int nPlayerID)
{
	auto strRet = m_PlayerName.find(nPlayerID);
	if (strRet == m_PlayerName.end())
	{
		return 0;
	}
	return strRet->second;
}

//Director
