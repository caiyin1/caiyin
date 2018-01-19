#include "BombDataManager.h"
#include "FortDataManager.h"

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
	FortDataManager::getInstance()->addBombSize(1);
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
