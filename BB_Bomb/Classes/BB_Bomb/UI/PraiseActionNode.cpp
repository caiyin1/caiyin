#include "PraiseActionNode.h"
#include "../Manager/GameDeploy.h"
#include "../Scene/BB_GameScene.h"
USING_NS_CC;

PraiseActionNode* PraiseActionNode::create(const Type& eType)
{
	PraiseActionNode* pRef = new PraiseActionNode;
	if (pRef && pRef->initWithData(eType))
	{
		pRef->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRef);
	}
	return pRef;
}

bool PraiseActionNode::initWithData(const Type& eType)
{
	bool bRef = false;
	do
	{
		m_nAddedScore = 0;
		// 获取block的Hp
		int nBlockHp = GameDeploy::getInstance()->getBlockHp();
		CC_BREAK_IF(!Node::init());
		std::string strImageFile;
		switch (eType)
		{
		case PraiseActionNode::Type::kType_Good:
			strImageFile = "res/BB_Bomb/Image/praise/bb_good.png";
			m_nAddedScore = 1 * nBlockHp;
			break;
		case PraiseActionNode::Type::kType_Perfect:
			strImageFile = "res/BB_Bomb/Image/praise/bb_perfect.png";
			m_nAddedScore = 5 * nBlockHp;
			break;
		case PraiseActionNode::Type::kType_Legendary:
			strImageFile = "res/BB_Bomb/Image/praise/bb_legendary.png";
			m_nAddedScore = 10 * nBlockHp;
			break;
		}
		// 创建精灵
		m_pSprite = Sprite::create(strImageFile);
		auto spriteSize = m_pSprite->getContentSize();
		// 设置Node的尺寸
		setNode();
		m_pSprite->setPosition(_contentSize * 0.5f);
		addChild(m_pSprite);
		// 添加分数label
		auto pAddedScoreLabel = Label::createWithBMFont(GOLDEN_SCORE_BMFNT, StringUtils::format("%d", m_nAddedScore));
		pAddedScoreLabel->setName("pAddScoreLabel");
		pAddedScoreLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
		pAddedScoreLabel->setPosition(_contentSize.width * 0.5f, _contentSize.height);
		addChild(pAddedScoreLabel);
		addAnction();
		bRef = true;
	} while (0);
	return bRef;
}

void PraiseActionNode::setNode()
{
	if (m_pSprite == nullptr)
	{
		return;
	}
	auto spriteSize = m_pSprite->getContentSize();
	setContentSize(spriteSize);
	setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	// 开启影响还能子节点
	setCascadeOpacityEnabled(true);
}

void PraiseActionNode::addAnction()
{
	if (m_pSprite == nullptr)
	{
		return;
	}
	setScale(0.01f);
	auto pGameDeploy = GameDeploy::getInstance();
	auto fScalingRatio = pGameDeploy->getScalingRatio();
	auto interfaceSize = pGameDeploy->getInterfaceSize();
	auto nHeight = 300 / fScalingRatio;
	float fActionTime = 1;
	// 跳的动作
	auto pJump = JumpBy::create(fActionTime, Vec2(0, nHeight), nHeight, 1);
	// 小到大
	auto pScaleTo = ScaleTo::create(fActionTime, 2.0f);
	auto pSpawn = Spawn::create(pJump, pScaleTo, nullptr);
	// 淡出 
	auto pFadeOut = FadeOut::create(1.0f);
	// 添加一个回调函数 
	auto pMoveTo = MoveTo::create(fActionTime, Vec2(interfaceSize.width * 0.5f, interfaceSize.height * SCREEN_TOP_COFFICIENT));
	auto pFunAddedScoreCallBack = CallFunc::create(CC_CALLBACK_0(PraiseActionNode::addedScoreCallFun, this));
	auto pSequence = Sequence::create(pSpawn, pFunAddedScoreCallBack, DelayTime::create(1.0f), pFadeOut, RemoveSelf::create(), nullptr);
	runAction(pSequence);
}


void PraiseActionNode::setAddedCallBack(const std::function<void(cocos2d::Node*, int)>& fun)
{
	m_pFunCallBack = fun;
}

void PraiseActionNode::addedScoreCallFun()
{
	if (m_pFunCallBack)
	{
		m_pFunCallBack(this->getChildByName("pAddScoreLabel"), m_nAddedScore);
	}
}
