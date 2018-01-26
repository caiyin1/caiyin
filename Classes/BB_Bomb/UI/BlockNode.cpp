#include "BlockNode.h"
#include "../Manager/GameDeploy.h"
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
		m_pHPLabel = Label::createWithBMFont(NUM_FILE, StringUtils::format("%d", m_BlockData.m_nHP));
		// 获取屏幕缩放比
		float fScalingRatio = GameDeploy::getInstance()->getScalingRatio();
		// 设置Node 
		setContentSize(Size(BLOCK_SIZE / fScalingRatio, BLOCK_SIZE / fScalingRatio));
		setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		Size blockSize;
		switch (m_BlockData.m_eType)
		{
		case BlockData::Type::Type_Nome:
		{
			m_pBlockSprite = Sprite::create("res/BB_Bomb/Image/BB_block_nome.png");
			blockSize = m_pBlockSprite->getContentSize();
			// 创建刚体
			m_pBlockBody = PhysicsBody::createBox(blockSize, PhysicsMaterial(0.0f, 1.0f, 0.0f));
			// 设置字体位置
			m_pHPLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			m_pHPLabel->setPosition(_contentSize.width * 0.45f, _contentSize.height * 0.4f);
			m_pHPLabel->setColor(Color3B::BLACK);
			m_pBlockSprite->addChild(m_pHPLabel);
			setBlockTag(BLOCK_TAG_NUM);
			break;
		}
		case BlockData::Type::Type_Triangle:
		{
			m_pBlockSprite = Sprite::create("res/BB_Bomb/Image/BB_block_half.png");
			blockSize = m_pBlockSprite->getContentSize();
			int nNum = random(1, 4);
			handleTriangle(nNum);
#if COCOS2D_VERSION <= 0x00030330
			// 3.3刚体会随着精灵旋转
			nNum = 1;
#endif
			initTriangle(nNum, blockSize);
			setBlockTag(BLOCK_TAG_NUM);
			break;
		}
		case BlockData::Type::Type_Octagon:
		{

			initOctagon();
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
		// 设置接触测试遮掩码
		m_pBlockBody->setContactTestBitmask(4);
		// 设置碰撞遮掩码
		m_pBlockBody->setCollisionBitmask(4);
		// 设置成静态的
		m_pBlockBody->setDynamic(false);
		// 添加到精灵上
		m_pBlockSprite->setPhysicsBody(m_pBlockBody);
		// 添加BlockSprite
		m_pBlockSprite->setPosition(_contentSize * 0.5f);
		addChild(m_pBlockSprite);
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

void BlockNode::initTriangle(int nNum, cocos2d::Size blockSize)
{
	Point point[3];
	switch (nNum)
	{
	case 1:
		point[0] = Point(-blockSize.width * 0.5f, -blockSize.height *  0.5f);
		point[1] = Point(-blockSize.width * 0.5f, blockSize.height * 0.5f);
		point[2] = Point(blockSize.width * 0.5f, -blockSize.height * 0.5f);
		break;
	case 4:
		point[0] = Point(-blockSize.width * 0.5f, -blockSize.height *  0.5f);
		point[1] = Point(-blockSize.width * 0.5f, blockSize.height * 0.5f);
		point[2] = Point(blockSize.width * 0.5f, blockSize.height * 0.5f);
		break;
	case 3:
		point[0] = Point(-blockSize.width * 0.5f, blockSize.height *  0.5f);
		point[1] = Point(blockSize.width * 0.5f, blockSize.height * 0.5f);
		point[2] = Point(blockSize.width * 0.5f, -blockSize.height * 0.5f);
		break;
	case 2:
		point[0] = Point(-blockSize.width * 0.5f, -blockSize.height *  0.5f);
		point[1] = Point(blockSize.width * 0.5f, blockSize.height * 0.5f);
		point[2] = Point(blockSize.width * 0.5f, -blockSize.height * 0.5f);
		break;
	default:
		break;
	}
	// 创建刚体
	m_pBlockBody = PhysicsBody::createPolygon(point, 3, PhysicsMaterial(0.0f, 1.0f, 0.0f));
}

void BlockNode::handleTriangle(int nNum)
{
	// 设置锚点位置
	m_pHPLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	switch (nNum)
	{
	case 1:
		m_pHPLabel->setPosition(_contentSize.width * 0.35f, _contentSize.height * 0.2f);
		m_pHPLabel->setColor(Color3B::BLACK);
		m_pBlockSprite->addChild(m_pHPLabel);
		break;
	case 2:
		m_pBlockSprite->setRotation(90);
		m_pHPLabel->setPosition(_contentSize.width * 0.35f, _contentSize.height * 0.2f);
		m_pHPLabel->setColor(Color3B::BLACK);
		m_pBlockSprite->addChild(m_pHPLabel);
		m_pHPLabel->setRotation(270);
		break;
	case 3:
		m_pBlockSprite->setRotation(180);
		m_pHPLabel->setPosition(_contentSize.width * 0.35f, _contentSize.height * 0.2f);
		m_pHPLabel->setColor(Color3B::BLACK);
		m_pBlockSprite->addChild(m_pHPLabel);
		m_pHPLabel->setRotation(180);
		break;
	case 4:
		m_pBlockSprite->setRotation(270);
		m_pHPLabel->setPosition(_contentSize.width * 0.35f, _contentSize.height * 0.2f);
		m_pHPLabel->setColor(Color3B::BLACK);
		m_pBlockSprite->addChild(m_pHPLabel);
		m_pHPLabel->setRotation(90);
		break;
	default:
		break;
	}

}

void BlockNode::initOctagon()
{
	// 获取屏幕缩放比
	auto fScalingRatio = GameDeploy::getInstance()->getScalingRatio();
	float fScale = 0.7f;
	m_pBlockSprite = Sprite::create("res/BB_Bomb/Image/bb_octagon.png");
	// m_pBlockSprite->setOpacity(0.3f * 255);
	/*auto blockSize = m_pBlockSprite->getContentSize();
	Point point[8] = { Point(-blockSize.width * fScale, -blockSize.height) * 0.5f, Point(-blockSize.width, -blockSize.height * fScale) * 0.5f,
						Point(-blockSize.width, blockSize.height * fScale) * 0.5f, Point(-blockSize.width * fScale, blockSize.height) * 0.5f,
						Point(blockSize.width * fScale, blockSize.height) * 0.5f, Point(blockSize.width, blockSize.height * fScale) * 0.5f,
						Point(blockSize.width, -blockSize.height * fScale) * 0.5f, Point(blockSize.width * fScale, -blockSize.height) * 0.5f };
	m_pBlockBody = PhysicsBody::createPolygon(point, 8, PhysicsMaterial(0.0f, 1.0f, 0.0f));
	m_pBlockSprite->setPhysicsBody(m_pBlockBody);*/

	ValueMap root_val = FileUtils::getInstance()->getValueMapFromFile("res/BB_Bomb/PhysicsBody/bb_octagon.plist");
	ValueMap bodies_val = root_val["bodies"].asValueMap();
	ValueMap coco_val = bodies_val["bb_octagon"].asValueMap();
	ValueVector fixtures_val = coco_val["fixtures"].asValueVector();
	ValueMap array_val = fixtures_val.at(0).asValueMap();
	ValueVector polygons_val = array_val["polygons"].asValueVector();
	m_pBlockBody = PhysicsBody::create();
	for (int i = 0; i < polygons_val.size(); i++)
	{
		ValueVector m_array = polygons_val.at(i).asValueVector();
		Point* points = new Point[m_array.size()];
		for (int j = 0; j < m_array.size(); j++)
		{
			std::string pointString = m_array.at(j).asString();
			int x = atoi(pointString.substr(pointString.find("{") + 1, pointString.find(",") - 1).c_str());
			int y = atoi(pointString.substr(pointString.find(",") + 1, pointString.length() - pointString.find(",")).c_str());
			points[j] = Point(x, y) / fScalingRatio;
		}
		m_pBlockBody->addShape(PhysicsShapePolygon::create(points, m_array.size(), PhysicsMaterial(0, 1.0f, 0)));
		delete[] points;
	}
	m_pBlockSprite->setPhysicsBody(m_pBlockBody);
	// 设置字体位置
	m_pHPLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_pHPLabel->setPosition(_contentSize.width * 0.45f, _contentSize.height * 0.4f);
	m_pHPLabel->setColor(Color3B::BLACK);
	m_pBlockSprite->addChild(m_pHPLabel);

	setBlockTag(BLOCK_TAG_NUM);
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
	m_pHPLabel->setPosition(_contentSize.width * 0.45f, _contentSize.height * 0.4f);
	m_pHPLabel->setColor(Color3B::BLACK);
	m_pBlockSprite->addChild(m_pHPLabel);

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
