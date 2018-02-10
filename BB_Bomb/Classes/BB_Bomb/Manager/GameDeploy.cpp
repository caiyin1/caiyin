#include "GameDeploy.h"

static GameDeploy* s_global = nullptr;
GameDeploy* GameDeploy::getInstance()
{
	if (s_global == nullptr)
	{
		s_global = new GameDeploy;
		s_global->init();
	}
	return s_global;
}

void GameDeploy::init()
{

	// 初始化游戏数据
	// 正常块块的生成概率
	m_nCubeBlockAddProbaility = 320;
	// 八角形块块的生成概率
	m_nOctagonBlockGenerateRate = 120;
	// 三角形块块生成概率
	m_nTriangleBlockAddProBaility = 120;
	// 无块块生成概率
	m_nNormalBlockProBaility = 700;
	// 黑洞块块的生成概率
	m_nBlackHoleBlockAddProBaility = 40;
	// Block的 生命值
	m_nBlockHP = 1;
	// 食物的生成概率
	m_fAddFoodProbaility = 50;
	// 子弹发送时间间隔
	m_nBombLaunchTimer = 4;
	// 偏移放大量
	m_fMoveScalingRatio = 2;
	// 子弹速度
	m_fBombSpeed = 1000;
	// 箭头缩放比
	m_fArrowScalingRatio = 20;
	// 箭头的极限角度
	m_fArrowLimitAngle = 9;
}

int GameDeploy::getTotalRatio()
{
	return m_nCubeBlockAddProbaility + m_nOctagonBlockGenerateRate + m_nTriangleBlockAddProBaility + m_nBlackHoleBlockAddProBaility;
}

void GameDeploy::setScalingRatio(float fScalingRatio)
{
	m_fScalingRatio = fScalingRatio;
}

float GameDeploy::getScalingRatio()
{
	return m_fScalingRatio;
}

int GameDeploy::getAddCubeBlockProbaility()
{
	return m_nCubeBlockAddProbaility;
}


void GameDeploy::setAddCubeBlockProbaility(int nProbabilty)
{
	m_nCubeBlockAddProbaility = nProbabilty;
}

int GameDeploy::getBlockHp()
{
	return m_nBlockHP;
}


void GameDeploy::setBlockHp(int nHp)
{
	m_nBlockHP = nHp;
}

void GameDeploy::additionBlockHP(int nNum /*= 1*/)
{
	m_nBlockHP += nNum;
}

float GameDeploy::getAddFoodProbaility()
{
	return m_fAddFoodProbaility;
}

int GameDeploy::getBombShootTimer()
{
	return m_nBombLaunchTimer;
}

float GameDeploy::getMoveScalingRatio()
{
	return m_fMoveScalingRatio;
}

float GameDeploy::getBombSpeed()
{
	return m_fBombSpeed / m_fScalingRatio;
}

int GameDeploy::getTriangleBlockAddProbaility()
{
	return m_nTriangleBlockAddProBaility;
}


void GameDeploy::setTriangleBlockAddProbaility(int nProbabilty)
{
	m_nTriangleBlockAddProBaility = nProbabilty;
}

int GameDeploy::getNormalBlockProbaility()
{
	return m_nNormalBlockProBaility;
}


int GameDeploy::getOctagonBlockProbaility()
{
	return m_nOctagonBlockGenerateRate;
}


void GameDeploy::setOctagonBlockProbaility(int nProbability)
{
	m_nOctagonBlockGenerateRate = nProbability;
}

int GameDeploy::getBlackHoleBlockAddProbaility()
{
	return m_nBlackHoleBlockAddProBaility;
}


void GameDeploy::setBlackHoleBlockAddProbaility(int nProbability)
{
	m_nBlackHoleBlockAddProBaility = nProbability;
}

void GameDeploy::addGameDifficult()
{
	m_nBlackHoleBlockAddProBaility += 2;
	m_nOctagonBlockGenerateRate += 3;
	m_nTriangleBlockAddProBaility += 3;
}

void GameDeploy::setGameColumnNun(float fSize)
{
	if (m_fScalingRatio == 0)
	{
		// CCLOG("Error : setGameColunmNum = 0");
		return;
	}
	m_nGameColumnNum = fSize / BLOCK_SIZE;
}

int GameDeploy::getGameColumnNum()
{
	return m_nGameColumnNum;
}

float GameDeploy::getArrowScalingRatio()
{
	return m_fArrowScalingRatio / m_fScalingRatio;
}

float GameDeploy::getArrowLimitAngle()
{
	return m_fArrowLimitAngle;
}

void GameDeploy::setGameLayerSize(const cocos2d::Size& gameLayerSize)
{
	m_gameLayerSize = gameLayerSize;
}

const cocos2d::Size& GameDeploy::getInterfaceSize()
{
	return m_gameLayerSize;
}
