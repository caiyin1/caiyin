#include "UserDataManager.h"
#include "GameDeploy.h"
#include "cocos2d.h"
#include "../Manager/DataManager.h"
#include "../Util/archiveMacro.h"
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
	m_nHighScore = 0;
	m_nScore = 0;
	// 读取历史最高分
	loadData();
}

void UserDataManager::saveData()
{
	std::string strScore = StringUtils::format("%d", m_nHighScore);
	auto pDataManager = DataManager::getInstance();
	auto strScoreData = pDataManager->encryptData(strScore.c_str(), (int)strScore.length());
	UserDefault::getInstance()->setStringForKey(USER_HIGH_SCORE, strScoreData);
}

int UserDataManager::getHighScore()
{
	return m_nHighScore;
}

void UserDataManager::setHighScore(int nScore)
{
	// 记录历史最高分
	DataManager::getInstance()->safeModifyHighScore(m_nHighScore, SAFE_HIGHEST_SCORE, nScore);
	// 存档
	saveData();
}

void UserDataManager::setPlayerScore(int nScore)
{
	m_nScore = nScore;
}

int UserDataManager::getPlayerScore()
{
	return m_nScore;
}

void UserDataManager::loadData()
{
	std::string strScoreData = UserDefault::getInstance()->getStringForKey(USER_HIGH_SCORE);

	int nScore = 0;
	if (!strScoreData.empty())
	{
		auto pDataManager = DataManager::getInstance();
		auto strScore = pDataManager->decryptData((unsigned char*)strScoreData.c_str(), (int)strScoreData.length());
		nScore = (int)strtoll(strScore.c_str(), nullptr, 10);
	}
	DataManager::getInstance()->safeModifyHighScore(m_nHighScore, SAFE_HIGHEST_SCORE, nScore, false);
}
