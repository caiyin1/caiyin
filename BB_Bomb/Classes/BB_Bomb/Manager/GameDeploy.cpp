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
	m_fNomeBlockAddProbaility = 30;
	// 右三角形块块生成概率
	m_fRightBlockAddProBaility = 10;
	// Block的 生命值
	m_nBlockHP = 1;
	// 食物的生成概率
	m_fAddFoodProbaility = 30;
	// 子弹发送时间间隔
	m_nBombLaunchTimer = 4;
	// 偏移放大量
	m_fMoveScalingRatio = 2;
	// 子弹速度
	m_fBombSpeed = 1000;
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
	return m_fBombSpeed;
}

float GameDeploy::getRightBlockAddProbaility()
{
	return m_fRightBlockAddProBaility + m_fNomeBlockAddProbaility;
}

void GameDeploy::addGameDifficult()
{
	if (m_fNomeBlockAddProbaility > 50)
	{
		return;
	}
	m_fNomeBlockAddProbaility += 2;
	m_fRightBlockAddProBaility += 1;

}
