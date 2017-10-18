#include "GameData.h"

//Ψһ��̬ʵ��
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

GameData* GameData::shareGlobal()
{
	return s_global;//GameData::getInstance();

}

GameData* GameData::getInstance()
{
	//�ж�m_global�Ƿ��ѱ�ʵ���������û�У���֮ʵ����
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

//Director
