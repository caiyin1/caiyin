#include "FortDataManager.h"
#include "cocos2d.h"
#include "GameDeploy.h"
USING_NS_CC;
static FortDataManager* s_global = nullptr;
FortDataManager* FortDataManager::getInstance()
{
	if (s_global == nullptr)
	{
		s_global = new FortDataManager;
		s_global->init();
	}
	return s_global;
}

void FortDataManager::init()
{
	// 初始化数据
	// 子弹个数
	m_nBombSize = 0;
}


void FortDataManager::setBombSpeed(const float& fAngle)
{
	// 根据弧度 返回一个标准向量
	auto standardVector = ccpForAngle(fAngle);
	// 返回向量的的长度
	auto fLength = ccpLength(standardVector);
	// 获取子弹的原始速度
	float fSpeed = GameDeploy::getInstance()->getBombSpeed();
	float fScaling = fSpeed / fLength * 1.0f;
	m_bombSpeed = ccpMult(standardVector, fScaling);
	m_bombSpeed.x = -m_bombSpeed.x;
}

const cocos2d::Vec2& FortDataManager::getBombSpeed()
{
	return m_bombSpeed;
}

void FortDataManager::setFortPosition(const cocos2d::Point& fortPosition)
{
	m_fortPoint = fortPosition;
}

const cocos2d::Point& FortDataManager::getForPosition()
{
	return m_fortPoint;
}

int FortDataManager::getBombSize()
{
	return m_nBombSize;
}

void FortDataManager::setBombSize(int nNum /*= 1*/)
{
	m_nBombSize += nNum;
}

void FortDataManager::setShootAngle(const float& fAngle)
{
	m_fShootAngle = fAngle;
}
