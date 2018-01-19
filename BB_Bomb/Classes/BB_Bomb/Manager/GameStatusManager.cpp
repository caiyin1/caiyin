#include "GameStatusManager.h"

static GameStatusManager* s_global = nullptr;
GameStatusManager* GameStatusManager::getInstance()
{
	if (s_global == nullptr)
	{
		s_global = new GameStatusManager;
		s_global->init();
	}
	return s_global;
}

void GameStatusManager::init()
{
	m_eStatus = GameStatus::kStatus_None;
}

void GameStatusManager::setGameStatus(GameStatus eStatus)
{
	m_eStatus = eStatus;
}

GameStatusManager::GameStatus GameStatusManager::getGameStatus()
{
	return m_eStatus;
}
