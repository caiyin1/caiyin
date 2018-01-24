#include "BombDataManager.h"
#include "FortDataManager.h"
#include "cocos2d.h"
USING_NS_CC;


static BombDataManager* s_global = nullptr;
BombDataManager* BombDataManager::getInstance()
{
	if (s_global == nullptr)
	{
		s_global = new BombDataManager;
		s_global->init();
	}
	return s_global;
}

void BombDataManager::init()
{
	// 初始化数据

	// 清空数据
	m_vBombNode.clear();

}

const std::vector<BombNode*>& BombDataManager::getBombVector()
{
	return m_vBombNode;
}

BombNode* BombDataManager::addBombNode()
{
	auto pBombNode = BombNode::create();
	m_vBombNode.push_back(pBombNode);
	// 子弹总数量加一
	FortDataManager::getInstance()->setBombSize(1);
	return pBombNode;
}

void BombDataManager::addFlyBombToVector(BombNode* pBombNode)
{
	m_vFlyBombNode.push_back(pBombNode);
}

void BombDataManager::removeFlyBombToVector(BombNode* pBombNode)
{
	for (auto iter = m_vFlyBombNode.begin(); iter != m_vFlyBombNode.end(); iter++)
	{
		if (*iter == pBombNode)
		{
			m_vFlyBombNode.erase(iter);
			return;
		}
	}
}

void BombDataManager::clearFlyBomb()
{
	m_vFlyBombNode.clear();
}

const std::vector<BombNode* >& BombDataManager::getFlyBombVector()
{
	return m_vFlyBombNode;
}

void BombDataManager::removeBomb(int nNum /*= 1*/)
{
	if (m_vBombNode.size() <= nNum)
	{
		return;
	}
	for (int i = 0; i < nNum; i++)
	{
		auto iter = m_vBombNode.begin();

		// 延迟1 秒删除场景中的子弹
		auto pDelayTime = DelayTime::create(1);
		(*iter)->stopAllActions();
		(*iter)->removeFromParent();
		m_vBombNode.erase(iter);
	}
	// 删除炮台管理类中的子弹个数
	FortDataManager::getInstance()->setBombSize(-nNum);
}
