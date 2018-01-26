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
	m_fNomeBlockAddProbaility = 25;
	// 三角形块块生成概率
	m_fTriangleBlockAddProBaility = 10;
	// 黑洞块块的生成概率
	m_fBlackHoleBlockAddProBaility = 5;
	// Block的 生命值
	m_nBlockHP = 1;
	// 食物的生成概率
	m_fAddFoodProbaility = 50;
	// 子弹发送时间间隔
	m_nBombLaunchTimer = 4;
	// 偏移放大量
	m_fMoveScalingRatio = 2;
	// 子弹速度
	m_fBombSpeed = 1500;
	// 箭头缩放比
	m_fArrowScalingRatio = 20;
	// 箭头的极限角度
	m_fArrowLimitAngle = 20;
}



void GameDeploy::setScalingRatio(float fScalingRatio)
{
	m_fScalingRatio = fScalingRatio;
}

float GameDeploy::getScalingRatio()
{
	return m_fScalingRatio;
}

float GameDeploy::getAddNomeBlockProbaility()
{
	return m_fNomeBlockAddProbaility;
}

int GameDeploy::getBlockHP()
{
	return m_nBlockHP;
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

float GameDeploy::getTriangleBlockAddProbaility()
{
	return m_fTriangleBlockAddProBaility + m_fNomeBlockAddProbaility;
}


float GameDeploy::getBlackHoleBlockAddProbaility()
{
	return m_fBlackHoleBlockAddProBaility + m_fTriangleBlockAddProBaility + m_fNomeBlockAddProbaility;
}

void GameDeploy::addGameDifficult()
{
	if (m_fNomeBlockAddProbaility > 45)
	{
		return;
	}
	m_fNomeBlockAddProbaility += 2;
	m_fTriangleBlockAddProBaility += 1;

}

void GameDeploy::setGameColumnNun(float fSize)
{
	if (m_fScalingRatio == 0)
	{
		// CCLOG("Error : setGameColunmNum = 0");
		return;
	}
	m_nGameColumnNum = fSize / (BLOCK_SIZE / m_fScalingRatio);
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
