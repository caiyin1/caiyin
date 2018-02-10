#include "ActionFactoryNode.h"
#include "../Manager/GameDeploy.h"
USING_NS_CC;

ActionFactoryNode* ActionFactoryNode::create(int nNum)
{
	ActionFactoryNode* pRet = new ActionFactoryNode;
	if (pRet && pRet->initWith(nNum))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}

bool ActionFactoryNode::initWith(int nNum)
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!Node::init());
		// 获取缩放比
		auto pGameDeploy = GameDeploy::getInstance();
		auto fScalingRatio = pGameDeploy->getScalingRatio();
		// 创建食物
		auto pFoodSprite = Sprite::create("res/BB_Bomb/Image/balls/default.png");
		auto size = pFoodSprite->getContentSize();
		pFoodSprite->setPosition(size * 0.5f);

		setContentSize(size);
		setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		addChild(pFoodSprite);

		Label* pSocerLabel = nullptr;
		// 添加得分的Label 
		if (nNum > 0)
		{
			pSocerLabel = Label::createWithBMFont(BLOCK_NUM_BMFNT, StringUtils::format(" +%d", nNum));
		}
		else
		{
			pSocerLabel = Label::createWithBMFont(BLOCK_NUM_BMFNT, StringUtils::format(" %d", nNum));
		}
		auto labelSize = pSocerLabel->getContentSize();
		pSocerLabel->setPosition(_contentSize.width + labelSize.width*0.5f + 0 / fScalingRatio, _contentSize.height * 0.5f);
		addChild(pSocerLabel);
		// 添加动画
		runAction(getScorePlusAction(nNum));
		bRet = true;
	} while (0);
	return bRet;
}

cocos2d::Action* ActionFactoryNode::getScorePlusAction(int nNum)
{
	// 获取屏幕缩放比
	auto fScalingRatio = GameDeploy::getInstance()->getScalingRatio();
	auto pMoveBy = MoveBy::create(1.5f, Vec2(0, 100 / fScalingRatio));
	auto pFadeOut = FadeOut::create(1.5f);
	// 添加淡出和上升同时进行
	auto pSpawn = Spawn::create(pMoveBy, pFadeOut, NULL);
	auto pRemoveSelf = RemoveSelf::create();
	auto pRet = Sequence::create(pSpawn, pRemoveSelf, NULL);
	return pRet;
}
