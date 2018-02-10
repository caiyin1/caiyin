#include "FoodNode.h"
#include "../Manager/GameDeploy.h"
USING_NS_CC;

FoodNode* FoodNode::create(const FoodData& foodData)
{
	FoodNode* pRet = new FoodNode;
	if (pRet && pRet->initWithData(foodData))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}

bool FoodNode::initWithData(const FoodData& foodData)
{
	bool bRet = false;
	m_data = foodData;
	do
	{
		CC_BREAK_IF(!Node::init());
		switch (foodData.m_eFoodType)
		{
		case FoodData::FoodType::kType_AddBomb:
		{
			// 创建食物
			m_pFoodSprite = Sprite::create("res/BB_Bomb/Image/balls/default.png");
			auto foodSize = m_pFoodSprite->getContentSize();
			// 创建一个刚体
			m_pFoodBody = PhysicsBody::createCircle(foodSize.width * 0.5f,
				PhysicsMaterial(0.0f, 0.0f, 0.0f));
			// 设置遮掩码
			// 设置类别遮掩码 0011
			m_pFoodBody->setCategoryBitmask(4);
			// 设置触碰测试遮掩码 0011
			m_pFoodBody->setContactTestBitmask(4);
			// 设置碰撞遮掩码 0011
			m_pFoodBody->setCollisionBitmask(4);
			// 设置成静态的
			m_pFoodBody->setDynamic(false);
			m_pFoodSprite->setPhysicsBody(m_pFoodBody);
			setTag(FOOD_TAG_NUM);
			m_pFoodBody->setTag(FOOD_TAG_NUM);
			addChild(m_pFoodSprite);
			break;
		}
		default:
			break;
		}
		bRet = true;
	} while (0);
	return bRet;
}

void FoodNode::reomveFoodNode()
{
	this->removeFromParent();
}

void FoodNode::setBodyEnabled(bool b)
{
	if (m_pFoodBody == nullptr)
	{
		return;
	}
#if COCOS2D_VERSION <= 0x0030330
	m_pFoodBody->setEnable(b);
#else
	m_pFoodBody->setEnabled(b);
#endif
}

const FoodData::FoodType& FoodNode::getFoodType()
{
	return m_data.m_eFoodType;
}
