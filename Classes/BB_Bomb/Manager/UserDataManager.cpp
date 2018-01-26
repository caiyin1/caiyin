#include "UserDataManager.h"
#include "GameDeploy.h"
#include "cocos2d.h"
USING_NS_CC;

static UserDataManager* s_global = nullptr;
UserDataManager* UserDataManager::getInstance()
{
	if (s_global == nullptr)
	{
		s_global = new UserDataManager;
		s_global->init();
	}
	return s_global;
}

void UserDataManager::init()
{
	// 初始化数据
	// 读取历史最高分
	loadData();
}

void UserDataManager::saveData()
{
	UserDefault::getInstance()->setIntegerForKey(USER_NAME, m_nHighScore);
}

int UserDataManager::getHighScore()
{
	return m_nHighScore;
}

void UserDataManager::setHighScore(int nScore)
{
	// 记录历史最高分
	m_nHighScore = nScore;
	// 存档
	saveData();
}

void UserDataManager::loadData()
{
	m_nHighScore = UserDefault::getInstance()->getIntegerForKey(USER_NAME);
}
