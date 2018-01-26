#include"BackgroundDataManager.h"
#include "GameDeploy.h"
#include "../UI/BlockNode.h"
#include "../UI/FoodNode.h"
#include "GameStatusManager.h"
#include "FortDataManager.h"
USING_NS_CC;

static BackgroundDataManager* s_global = nullptr;

BackgroundDataManager* BackgroundDataManager::getInstance()
{
	if (s_global == nullptr)
	{
		s_global = new BackgroundDataManager;
		s_global->init();
	}
	return s_global;
}

void BackgroundDataManager::init()
{
	// 初始化数据
	m_nFoodSize = 1;
}

void BackgroundDataManager::addBackData()
{
	// 获取生成块块的概率
	auto pGameDeploy = GameDeploy::getInstance();
	// 获取正方形的身材概率
	float fNomeBlockProBaility = pGameDeploy->getAddNomeBlockProbaility();
	// 获取三角形的生成概率
	float fTriangleBlockProBaility = pGameDeploy->getTriangleBlockAddProbaility();
	// 获取黑洞的生成概率
	float fBlackHoleProbaility = pGameDeploy->getBlackHoleBlockAddProbaility();
	// 获取块块的生命值
	int nBLockHP = pGameDeploy->getBlockHP();
	// 获取子弹的数量
	int nBombSize = FortDataManager::getInstance()->getBombSize();
	// 增加难度
	if (nBLockHP % 10 == 0)
	{
		pGameDeploy->addGameDifficult();
	}
	// 获取屏幕缩放比
	float fScalingRatio = pGameDeploy->getScalingRatio();
	float fSize = BLOCK_SIZE / fScalingRatio;
	int nNum = pGameDeploy->getGameColumnNum();
	for (int i = 0; i < nNum; i++)
	{
		int nNum = cocos2d::random(0, 100);
		// 判断添加食物
		if (nNum < fNomeBlockProBaility)
		{
			BlockData blockData;
			blockData.m_nHP = nBLockHP;
			if (nBLockHP % 10 == 0 && nNum % 2 == 0)
			{
				blockData.m_nHP = 2 * blockData.m_nHP;
			}
			blockData.m_eType = BlockData::Type::Type_Octagon;
			// 添加Block
			auto pBlockNode = BlockNode::create(blockData);
			pBlockNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			pBlockNode->setPositionX(fSize * i + fSize*0.5f);
			m_callbackAddBackground(pBlockNode);
		}
		else if (nNum < fTriangleBlockProBaility)
		{
			BlockData blockData;
			blockData.m_nHP = nBLockHP;
			if (nBLockHP % 10 == 0 && nNum % 2 == 0)
			{
				blockData.m_nHP = 2 * blockData.m_nHP;
			}
			blockData.m_eType = BlockData::Type::Type_Triangle;
			// 添加Block
			auto pBlockNode = BlockNode::create(blockData);
			pBlockNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			pBlockNode->setPositionX(fSize * i + fSize*0.5f);
			m_callbackAddBackground(pBlockNode);
		}
		else if (nNum < fBlackHoleProbaility)
		{

			if (nBombSize < 1 || nBLockHP < 10)
			{
				continue;
			}
			BlockData blockData;
			blockData.m_nHP = nBombSize / 2;
			blockData.m_eType = BlockData::Type::Type_BlackHole;
			auto pBlockNode = BlockNode::create(blockData);
			pBlockNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			pBlockNode->setPositionX(fSize * i + fSize * 0.5f);
			m_callbackAddBackground(pBlockNode);
		}
		else
		{
			// 获取食物的生成概率
			float fFoodProbaility = pGameDeploy->getAddFoodProbaility();
			// 添加食物
			int nNum = cocos2d::random(0, 100);
			if (fFoodProbaility > nNum && m_nFoodSize <= nBLockHP)
			{
				FoodData foodData;
				foodData.m_eFoodType = FoodData::FoodType::kType_AddBomb;
				auto pFoodNode = FoodNode::create(foodData);
				pFoodNode->setPositionX(i * fSize + fSize*0.5f);
				m_callbackAddBackground(pFoodNode);
				m_nFoodSize++;
			}
		}
	}
	// 添加Block的生命值
	pGameDeploy->additionBlockHP();
}

void BackgroundDataManager::setAddBackgroundCallBack(const std::function<void(cocos2d::Node*)>& callBack)
{
	m_callbackAddBackground = callBack;
}
