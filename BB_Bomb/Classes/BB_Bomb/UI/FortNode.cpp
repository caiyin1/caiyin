#include "FortNode.h"
#include "../Manager/GameDeploy.h"
USING_NS_CC;
FortNode* FortNode::create()
{
	FortNode* pRet = new FortNode;
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

bool FortNode::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!Node::init());
		m_pFortSprite = Sprite::create("res/BB_Bomb/Image/bb_fort.png");
		auto fortSize = m_pFortSprite->getContentSize();
		setContentSize(fortSize);
		m_pFortSprite->setPosition(_contentSize * 0.5f);
		addChild(m_pFortSprite);
		bRet = true;
	} while (0);
	return bRet;
}

void FortNode::addArrow(const cocos2d::Vec2& pos)
{
	if (m_pArrowSprite == nullptr)
	{
		// 装换坐标并记录传入的位置 
		m_beginPos = m_pFortSprite->convertToNodeSpace(pos);

		m_pArrowSprite = Sprite::create("res/BB_Bomb/Image/bb_arrow.png");
		m_pArrowSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
		m_pArrowSprite->setPosition(_contentSize.width * 0.5f, 0);
		addChild(m_pArrowSprite, -1);
		// 创建虚线
		auto arrowSize = m_pArrowSprite->getContentSize();
		m_pDottedLineSprite = Sprite::create("res/BB_Bomb/Image/bb_dotted_line.png");
		m_pDottedLineSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
		m_pDottedLineSprite->setPosition(arrowSize.width, arrowSize.height * 0.5f);
		m_pArrowSprite->addChild(m_pDottedLineSprite, -1);
		// 添加提示位置的 Button
		m_pButtonSprite = Sprite::create("res/BB_Bomb/Image/bb_button.png");
		m_pButtonSprite->setPosition(m_beginPos);
		addChild(m_pButtonSprite);

		// 计算向量的方向
		Vec2 vector1(0, pos.y);
		m_startVector = ccpSub(vector1, pos);
		// 转换为标准向量
		m_startVector = ccpNormalize(m_startVector);
		// 透明度为0
		m_pArrowSprite->setOpacity(0);
		m_pDottedLineSprite->setOpacity(0);
		// 角度为零
		m_fArrowRadian = 0;
	}
}

void FortNode::removeArrow()
{
	m_pArrowSprite->removeFromParent();
	m_pButtonSprite->removeFromParent();
	m_pButtonSprite = nullptr;
	m_pArrowSprite = nullptr;
}

void FortNode::setArrowRotation(const cocos2d::Vec2& Pos)
{
	// 转换坐标
	auto endPos = m_pFortSprite->convertToNodeSpace(Pos);
	// 获取箭头提示缩放比
	auto fArrowScalingRatio = GameDeploy::getInstance()->getArrowScalingRatio();
	if (endPos.y < m_beginPos.y)
	{
		float fSize = m_beginPos.y - endPos.y > fArrowScalingRatio ? m_beginPos.y - endPos.y : fArrowScalingRatio;
		fSize = fSize / fArrowScalingRatio;
		m_pDottedLineSprite->setScaleX(fSize);
	}
	else
	{
		m_pDottedLineSprite->setScaleX(1);
	}
	// 计算传入的向量
	Point vec2 = -ccpNormalize(ccpSub(endPos, m_beginPos));
	// 获取弧度
	m_fArrowRadian = ccpAngle(vec2, m_startVector);

	// 弧度转换角度
	float fRotation = CC_RADIANS_TO_DEGREES(m_fArrowRadian);
	/*
	* @brief 射击角度错误取消射击模式
	*/
	//if (fRotation < 20.0f || fRotation > 160.0f)
	//{
	//	// 不符合射击角度 透明度为0
	//	m_pArrowSprite->setOpacity(0);
	//	m_pDottedLineSprite->setOpacity(0);
	//}
	//{
	//	m_pArrowSprite->setOpacity(255);
	//	// 获取block的HP
	//	float fNum = GameDeploy::getInstance()->getBlockHP();
	//	fNum = 50 > fNum ? fNum : 50;
	//	fNum = (50 - fNum) / 50.0f;
	//	m_pDottedLineSprite->setOpacity(fNum * 255);
	//}
	// m_pArrowSprite->setRotation(fRotation);
	handleShootAngle(fRotation);
}

float FortNode::getArrowRadian()
{
	return m_fArrowRadian;
}

void FortNode::handleShootAngle(float fAngle)
{
	// 获取箭头的极限角度
	auto fArrowLimitAngle = GameDeploy::getInstance()->getArrowLimitAngle();
	if (fAngle < fArrowLimitAngle)
	{
		fAngle = fArrowLimitAngle;
	}
	else if (fAngle > 180 - fArrowLimitAngle)
	{
		fAngle = 180 - fArrowLimitAngle;
	}
	m_fArrowRadian = CC_DEGREES_TO_RADIANS(fAngle);
	m_pArrowSprite->setRotation(fAngle);

	m_pArrowSprite->setOpacity(255);
	// 获取block的HP
	float fNum = GameDeploy::getInstance()->getBlockHP();
	fNum = 50 > fNum ? fNum : 50;
	fNum = (50 - fNum) / 50.0f;
	m_pDottedLineSprite->setOpacity(fNum * 255);
}
