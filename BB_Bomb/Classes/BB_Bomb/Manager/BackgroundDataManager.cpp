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

// 根据百分比得到当前是什么类型的块块
static BlockData::Type getBlockType()
{
	// 默认为空
	BlockData::Type eType = BlockData::Type::Type_None;

	// TODO: 先得到所有块块的生成概率，把所有概率累加起来

	// TODO: 随机一个数，得到对应分布的块块区段

	// TODO: 返回对应的块块类型
	// 获取游戏配置
	auto pGameDeploy = GameDeploy::getInstance();
	// 获取生成块块的概率
	// 获取正方形的身材概率
	int nNomeBlockProBaility = pGameDeploy->getAddCubeBlockProbaility();
	// 获取三角形的生成概率
	int nTriangleBlockProBaility = pGameDeploy->getTriangleBlockAddProbaility();
	// 获取黑洞的生成概率
	int nBlackHoleProbaility = pGameDeploy->getBlackHoleBlockAddProbaility();
	// 获取无块块的生成概率
	int nNormalBlockProbaility = pGameDeploy->getNormalBlockProbaility();
	// 获取八边形块块
	int nOctagonBlockProbaility = pGameDeploy->getOctagonBlockProbaility();
	// 获取块块的概率和
	int nTotalTatio = pGameDeploy->getTotalRatio();
	int nSum = nNormalBlockProbaility + nTotalTatio;
	int nNum = random(0, nSum);
	if (nNum > nNormalBlockProbaility)
	{
		nNum = random(1, nTotalTatio);
		if (nNum < nNomeBlockProBaility)
		{
			eType = BlockData::Type::Type_Cube;
		}
		else if (nNum < nNomeBlockProBaility + nTriangleBlockProBaility)
		{
			eType = BlockData::Type::Type_Triangle;
		}
		else if (nNum < nNomeBlockProBaility + nTriangleBlockProBaility + nBlackHoleProbaility)
		{
			eType = BlockData::Type::Type_BlackHole;
		}
		else if (nNum <= nNomeBlockProBaility + nTriangleBlockProBaility + nBlackHoleProbaility + nOctagonBlockProbaility)
		{
			eType = BlockData::Type::Type_Octagon;
		}
	}

	return eType;
}

void BackgroundDataManager::addBackData()
{
	// 获取生成块块的概率
	auto pGameDeploy = GameDeploy::getInstance();

	// 获取块块的生命值
	int nBLockHP = pGameDeploy->getBlockHP();
	// 获取子弹的数量
	int nBombSize = FortDataManager::getInstance()->getBombSize();
	// 增加难度	
	pGameDeploy->addGameDifficult();
	// 获取屏幕缩放比
	float fScalingRatio = pGameDeploy->getScalingRatio();
	float fSize = BLOCK_SIZE;
	int nNum = pGameDeploy->getGameColumnNum();
	for (int i = 0; i < nNum; i++)
	{
		auto eTpye = getBlockType();
		switch (eTpye)
		{
		case BlockData::Type::Type_None:
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
		break;
		case BlockData::Type::Type_Cube:
		{
			BlockData blockData;
			blockData.m_nHP = nBLockHP;
			if (nBLockHP % 10 == 0 && nNum % 2 == 0)
			{
				blockData.m_nHP = 2 * blockData.m_nHP;
			}
			blockData.m_eType = BlockData::Type::Type_Cube;
			// 添加Block
			auto pBlockNode = BlockNode::create(blockData);
			pBlockNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			pBlockNode->setPositionX(fSize * i + fSize*0.5f);
			m_callbackAddBackground(pBlockNode);
		}
		break;
		case BlockData::Type::Type_Octagon:
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
		break;
		case BlockData::Type::Type_Triangle:
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
		break;
		case BlockData::Type::Type_BlackHole:
		{
			if (nBombSize <= 1 || nBLockHP < 10)
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
		break;
		}
	}
	// 添加Block的生命值
	pGameDeploy->additionBlockHP();
}

void BackgroundDataManager::setAddBackgroundCallBack(const std::function<void(cocos2d::Node*)>& callBack)
{
	m_callbackAddBackground = callBack;
}
