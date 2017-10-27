#include "GameOverScene.h"
#include "Classes/BackGroundScene/BackGroundScene.h"
USING_NS_CC;




bool GameOverScene::init()
{
	bool bRet = false;
	do 
	{
		if (! Scene::init())
		{
			return false;
		}
		setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		auto GameOverLabel = Label::createWithTTF("Game Over !", "fonts/Marker Felt.ttf", 40);
		GameOverLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		GameOverLabel->setPosition(_contentSize * 0.5);
		addChild(GameOverLabel);

		auto AgainStartLabel = Label::createWithTTF("Again Start", "fonts/Marker Felt.ttf", 20);
		auto AgainStartButton = MenuItemLabel::create(AgainStartLabel, this, menu_selector(GameOverScene::MenuStartCallBack));
		auto MenuAgain = Menu::create(AgainStartButton, NULL);
		MenuAgain->setPosition(Vec2(AgainStartButton->getContentSize().width * 0.5f, AgainStartButton->getContentSize().height * 0.5f));
		CCLOG("x = %f, y = %f", AgainStartLabel->getContentSize().width, AgainStartLabel->getContentSize().height);
		addChild(MenuAgain);

		auto ExitLabel = Label::createWithTTF("exit", "fonts/Marker Felt.ttf", 20);
		auto ExitButton = MenuItemLabel::create(ExitLabel, this, menu_selector(GameOverScene::MenuCloseCallback));
		auto MenuExit = Menu::create(ExitButton, NULL);
		MenuExit->setPosition(Vec2(_contentSize.width - ExitLabel->getContentSize().width * 0.5f , ExitLabel->getContentSize().height * 0.5f));
		addChild(MenuExit);
		bRet = true;
	} while (0);
	return bRet;
}

Scene* GameOverScene::createScene()
{
	return GameOverScene::create();
}

void GameOverScene::MenuStartCallBack(cocos2d::Ref* pSender)
{
	Director::getInstance()->pushScene(BackGroundScene::createScene());
}

void GameOverScene::MenuCloseCallback(cocos2d::Ref* pSender)
{
	Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
