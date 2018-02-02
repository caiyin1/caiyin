#include "BlockNode.h"
#include "../Manager/GameDeploy.h"
#include "PhysicsShapeCache.h"

USING_NS_CC;


BlockNode* BlockNode::create(const BlockData& blockData)
{
	BlockNode* pRet = new BlockNode;
	if (pRet && pRet->initWithData(blockData))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}

bool BlockNode::initWithData(const BlockData& blockData)
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!Node::init());

		m_BlockData.m_nHP = blockData.m_nHP;
		m_BlockData.m_eType = blockData.m_eType;
		// 添加HP Label
		// 字体大小先不设置
		m_pHPLabel = Label::createWithBMFont(BLOCK_NUM_BMFNT, StringUtils::format("%d", m_BlockData.m_nHP));
		// 获取屏幕缩放比
		float fScalingRatio = GameDeploy::getInstance()->getScalingRatio();
		// 设置Node 
		setContentSize(Size(BLOCK_SIZE, BLOCK_SIZE));
		setAnchorPoint(Vec2::ANCHOR_MIDDLE);

		m_pHPLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		m_pHPLabel->setPosition(_contentSize*0.5f);

		switch (m_BlockData.m_eType)
		{
		case BlockData::Type::Type_Cube:
		{
			m_pBlockSprite = Sprite::create("res/BB_Bomb/Image/blocks/3_3.png");
			m_pBlockBody = PhysicsShapeCache::getInstance()->createBodyWithName("cube");
			setBlockTag(BLOCK_TAG_NUM);
			break;
		}
		case BlockData::Type::Type_Triangle:
		{
			m_pBlockSprite = Sprite::create("res/BB_Bomb/Image/blocks/1_1.png");
			m_pBlockBody = PhysicsShapeCache::getInstance()->createBodyWithName("triangle");
			m_pHPLabel->setPosition(_contentSize.width*0.5f, _contentSize.height*0.4f);
			setBlockTag(BLOCK_TAG_NUM);
			break;
		}
		case BlockData::Type::Type_Octagon:
		{
			m_pBlockSprite = Sprite::create("res/BB_Bomb/Image/blocks/2_2.png");
			m_pBlockBody = PhysicsShapeCache::getInstance()->createBodyWithName("otagon");
			m_pHPLabel->setPosition(_contentSize*0.5f);
			setBlockTag(BLOCK_TAG_NUM);
			break;
		}
		case BlockData::Type::Type_BlackHole:
		{
			initBlackHole();
			break;
		}
		}

		// 设置遮掩码
		// 设置类别表示遮掩码  0010
		m_pBlockBody->setCategoryBitmask(2);
		// 设置接触测试遮掩码  0100
		m_pBlockBody->setContactTestBitmask(4);
		// 设置碰撞遮掩码     0100
		m_pBlockBody->setCollisionBitmask(4);
		// 设置成静态的
		m_pBlockBody->setDynamic(false);
		// 添加到精灵上
		m_pBlockSprite->setPhysicsBody(m_pBlockBody);
		// 添加BlockSprite
		m_pBlockSprite->setPosition(_contentSize * 0.5f);
		addChild(m_pBlockSprite);

		// 根据当前大小缩放
		// m_pBlockSprite->setScale(_contentSize.width / m_pBlockSprite->getContentSize().width);
		// m_pHPLabel->setScale(m_pBlockSprite->getScale());
		addChild(m_pHPLabel);

		// 给三角形随机换个方向
		if (blockData.m_eType == BlockData::Type::Type_Triangle)
		{
			int r = cocos2d::random(1, 10);
			if (r > 3)
			{
				m_pBlockSprite->setRotation(180);
				m_pHPLabel->setPositionY(_contentSize.height*0.6f);
			}
		}

		bRet = true;
	} while (0);
	return bRet;
}

void BlockNode::hitted(int nNum)
{
	m_BlockData.m_nHP -= nNum;
	if (m_BlockData.m_nHP > 0)
	{
		m_pHPLabel->setString(StringUtils::format("%d", m_BlockData.m_nHP));
	}
}

int BlockNode::getBLockHp()
{
	return m_BlockData.m_nHP;
}

void BlockNode::initBlackHole()
{
	m_pBlockSprite = Sprite::create("res/BB_Bomb/Image/bb_black_hole.png");
	auto blockSize = m_pBlockSprite->getContentSize();

	m_pBlockBody = PhysicsBody::createCircle(blockSize.width * 0.5f, PhysicsMaterial(0, 1.0f, 0));
	m_pBlockSprite->setPhysicsBody(m_pBlockBody);
	m_pBlockSprite->setColor(Color3B::BLUE);

	// 设置字体位置
	m_pHPLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_pHPLabel->setPosition(_contentSize.width * 0.5f, _contentSize.height * 0.5f);
	// m_pHPLabel->setColor(Color3B::BLACK);
	// 添加动画
	auto pAction = RotateBy::create(2, 360);
	m_pBlockSprite->runAction(RepeatForever::create(pAction));

	setBlockTag(BLACK_HOLE_TAG_NUM);
}

BlockData::Type BlockNode::getBlockType()
{
	return m_BlockData.m_eType;
}

void BlockNode::setBlockTag(int nNum)
{
	m_pBlockBody->setTag(nNum);
	setTag(nNum);
}
