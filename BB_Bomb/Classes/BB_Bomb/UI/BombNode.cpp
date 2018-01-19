#include "BombNode.h"
#include "../Manager/GameDeploy.h"
USING_NS_CC;

BombNode* BombNode::create()
{
	BombNode* pRet = new BombNode;
	if (pRet && pRet->init())
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}

bool BombNode::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!Node::init());
		m_pBombSprite = Sprite::create("res/BB_Bomb/Image/bb_Bomb.png");
		auto bombSize = m_pBombSprite->getContentSize();
		setContentSize(bombSize);
		m_pBombSprite->setPosition(_contentSize * 0.5f);
		initBombPhysicsBody();
		addChild(m_pBombSprite);
		m_eBombStatus = BombStatus::Status_Sleep;
		bRet = true;
	} while (0);
	return bRet;
}

void BombNode::initBombPhysicsBody()
{
	if (!m_pBombBody && m_pBombSprite)
	{
		auto bombSize = m_pBombSprite->getContentSize();
		// 创建刚体
		m_pBombBody = PhysicsBody::createCircle(bombSize.width * 0.5f,
			PhysicsMaterial(0.0f, 1.0f, 0.0f));
		// 设置成静态的 
		m_pBombBody->setDynamic(false);
		// 设置遮掩码
		// 设置类别遮掩码 0001
		m_pBombBody->setCategoryBitmask(4);
		// 设置碰撞测试位遮掩码 0010
		m_pBombBody->setContactTestBitmask(6);
		// 设置碰撞遮掩码  0010
		m_pBombBody->setCollisionBitmask(3);
		// 设置速度
		m_pBombBody->setVelocity(Vec2(0.0f, 0.0f));
		// 设置不产生力
		m_pBombBody->applyForce(Vec2(600, 600));
		m_pBombBody->setGravityEnable(false);
		// 
		m_pBombBody->setTag(BOMB_TAG_NUM);
		setPhysicsBody(m_pBombBody);
	}
}

void BombNode::removePhysicsBody()
{
	if (m_pBombBody)
	{
		m_pBombBody->removeFromWorld();
		m_pBombBody = nullptr;
	}
}

void BombNode::setBombSpeed(const cocos2d::Vec2& speed)
{
	// initBombPhysicsBody();
	m_pBombBody->setEnabled(true);
	m_pBombBody->setDynamic(true);
	m_pBombBody->setVelocity(speed);
}


void BombNode::setBombStatus(const BombStatus& eStatus)
{
	m_eBombStatus = eStatus;
}

const BombNode::BombStatus& BombNode::getBombStatus()
{
	return m_eBombStatus;
}
