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
		m_pFortSprite = Sprite::create("res/BB_Bomb/Image/Forts/bb_fort_1.png");
		m_pFortSprite->setScale(0.7f);
		auto fortSize = m_pFortSprite->getContentSize();
		auto fortRect = m_pFortSprite->getBoundingBox();
		setContentSize(fortRect.size);
		setAnchorPoint(Vec2::ANCHOR_MIDDLE);

		//auto nDar = DrawNode::create();
		//nDar->drawRect(Vec2(0, 0), _contentSize, Color4F::RED);
		//addChild(nDar);

		m_pFortSprite->setPosition(_contentSize.width * 0.5f, _contentSize.height * 0.5f);
		addChild(m_pFortSprite);
		// 设置炮台透明度
		// m_pFortSprite->setOpacity(0.5f * 255);
		setFortAnchorPoint();
		m_pFortSprite->setRotation(90);
		bRet = true;
	} while (0);
	return bRet;
}

void FortNode::addArrow(const cocos2d::Vec2& pos)
{
	if (m_pDottedLineSprite == nullptr)
	{
		// 装换坐标并记录传入的位置 
		m_beginPos = this->convertToNodeSpace(pos);

		// 创建虚线
		auto fortSize = m_pFortSprite->getContentSize();
		m_pDottedLineSprite = Sprite::create("res/BB_Bomb/Image/bb_dotted_line.png");
		m_pDottedLineSprite->setScale(1.5f);
		m_pDottedLineSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
		m_pDottedLineSprite->setPosition(fortSize.width, fortSize.height * 0.5f);
		m_pFortSprite->addChild(m_pDottedLineSprite, -1);
		// 添加提示位置的 Button
		m_pButtonSprite = Sprite::create("res/BB_Bomb/Image/bb_button.png");
		m_pButtonSprite->setPosition(m_beginPos);
		addChild(m_pButtonSprite);
		// 添加是否发射的提升圈
		m_pPromptRingSprite = Sprite::create("res/BB_Bomb/Image/bb_prompt_ring.png");
		m_pPromptRingSprite->setPosition(m_beginPos);
		addChild(m_pPromptRingSprite);
		// 隐藏提示圈
		m_pPromptRingSprite->setOpacity(255 * 0);

		// 计算向量的方向
		Vec2 vector1(0, pos.y);
		m_startVector = ccpSub(vector1, pos);
		// 转换为标准向量
		m_startVector = ccpNormalize(m_startVector);
		// 透明度为0
		m_pDottedLineSprite->setOpacity(0);
		// 角度为零
		m_fArrowRadian = 0;
	}
}

void FortNode::removeArrow()
{
	m_pButtonSprite->removeFromParent();
	m_pDottedLineSprite->removeFromParent();
	m_pPromptRingSprite->removeFromParent();
	m_pButtonSprite = nullptr;
	m_pArrowSprite = nullptr;
	m_pDottedLineSprite = nullptr;
}


void FortNode::setArrowRotation(const cocos2d::Vec2& Pos)
{
	// 转换坐标
	auto endPos = this->convertToNodeSpace(Pos);
	/*
	* @brief 关闭子弹取消发射功能
	*/
	// 判断是否可以发射
	//if (!judgeShoot(endPos))
	//{
	//	return;
	//}
	// 获取箭头提示缩放比
	auto fArrowScalingRatio = GameDeploy::getInstance()->getArrowScalingRatio();
	// 关闭缩放功能
	//if (endPos.y < m_beginPos.y)
	//{
	//	/*float fSize = m_beginPos.y - endPos.y > fArrowScalingRatio ? m_beginPos.y - endPos.y : fArrowScalingRatio;
	//	fSize = fSize / fArrowScalingRatio;*/
	//	// m_pDottedLineSprite->setScale(1.6f);
	//}
	//else
	//{
	//	m_pDottedLineSprite->setScaleX(1);
	//}

	// 计算传入的向量
	// 根据角度发射
	// Point vec2 = ccpNormalize(ccpSub(endPos, m_beginPos));
	// 指那打哪
	auto anchorPoint = m_pFortSprite->getAnchorPoint();
	Point vec2 = ccpNormalize(ccpSub(endPos, Vec2(_contentSize.width * anchorPoint.x, _contentSize.height * anchorPoint.y)));
	// 转换成指那打哪模式
	Point vec1 = -ccpNormalize(Vec2(_contentSize.width * anchorPoint.x, 0));
	// 获取弧度
	m_fArrowRadian = ccpAngle(vec2, vec1);

	// 弧度转换角度
	float fRotation = CC_RADIANS_TO_DEGREES(m_fArrowRadian);
	/*
	* @brief 上移发射 : 射击角度错误取消射击模式
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
	// fAngle -= 90;
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
	m_pFortSprite->setRotation(fAngle);

	// 获取block的HP
	float fNum = GameDeploy::getInstance()->getBlockHP();
	fNum = 50 > fNum ? fNum : 50;
	fNum = (50 - fNum) / 50.0f;
	m_pDottedLineSprite->setOpacity(1 * 255);
}

bool FortNode::judgeShoot(cocos2d::Vect pos)
{
	auto promptRingSize = m_pPromptRingSprite->getContentSize();

	// 计算点到圆心的长度
	float fSize = ccpLength(ccpSub(m_beginPos, pos));
	if (fSize > promptRingSize.width*0.5f)
	{
		// 提示圈常亮
		m_pPromptRingSprite->setOpacity(255 * 0.5f);
		return true;
	}
	else
	{
		m_pPromptRingSprite->setOpacity(255 * 0.5f);
		m_pDottedLineSprite->setOpacity(0);
		// 角度设置为0度 无法发射
		m_fArrowRadian = 0;
		return false;
	}
}

void FortNode::setFortAnchorPoint()
{
	auto fortRect = m_pFortSprite->getBoundingBox();
	m_pFortSprite->setAnchorPoint(Vec2(1.0f - 0.35904628330995f,
		fortRect.size.height * 0.5f / fortRect.size.height));
}
