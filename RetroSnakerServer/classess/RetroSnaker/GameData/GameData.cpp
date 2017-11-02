#include "GameData.h"

//Î¨Ò»¾²Ì¬ÊµÀý
static GameData* s_global = nullptr;


GameData* GameData::getInstance()
{
	if (s_global == NULL)
	{
		s_global = new GameData;
		s_global->init();
	}
	return s_global;
}

bool GameData::init()
{
	bool bRet = false;
	do 
	{

		bRet = true;
	} while (0);
	return bRet;
}

void GameData::addTask(Message::TagMsgHead* msg)
{
	m_mutexTask.lock();
	m_vecTask.push_back(msg);
	m_mutexTask.unlock();
}

Message::TagMsgHead* GameData::getTask()
{
	auto msgHead = m_vecTask.begin();
	m_mutexTask.lock();
	m_vecTask.erase(m_vecTask.begin());
	m_mutexTask.unlock();
	return *msgHead;
}

bool GameData::isTask()
{
	if (m_vecTask.empty())
	{
		return false;
	}
	return true;
}

void GameData::addPlayerData(int nPlayerID, g_TagPlayerData *playeData)
{
	if (m_MapPlayerData.find(nPlayerID) != m_MapPlayerData.end())
	{
		deletePlayerData(nPlayerID);
	}
	m_MapPlayerData.insert(std::make_pair(nPlayerID, playeData));
}

void GameData::deletePlayerData(int nPlayerID)
{
	if (m_MapPlayerData.find(nPlayerID) == m_MapPlayerData.end())
	{
		return;
	}
	m_mutexPlayer.lock();
	m_MapPlayerData.erase(nPlayerID);
	m_mutexPlayer.unlock();
}

g_TagPlayerData* GameData::getPlayerData(int nPlayerID)
{
	auto PlalerData = m_MapPlayerData.find(nPlayerID);
	if (PlalerData == m_MapPlayerData.end())
	{
		return NULL;
	}
	return PlalerData->second;
}
