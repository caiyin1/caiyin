#include "GameOver.h"
#include "MakeDraw/MakeDraw.h"
#include "GameOver/GameOver.h"
#include "RetroSnakerGame/RetroSnakerGame.h"

USING_NS_CC;


Scene* GameOver::CreateScene()
{
	return GameOver::create();
}

bool GameOver::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Scene::init());
		CC_BREAK_IF(!initGameOverLabel());
		CC_BREAK_IF(!initPlayerScore());
		CC_BREAK_IF(!initLabelButton());
		
		bRet = true;
	} while (0);
	return bRet;
}

bool GameOver::initGameOverLabel()
{
	bool bRet = false;
	do
	{
		m_pGameOverLabel = Label::createWithTTF("Game Over", "fonts/Marker Felt.ttf", 40);
		m_pGameOverLabel->setPosition(_contentSize.width * 0.5f, _contentSize.height - (m_pGameOverLabel->getContentSize().height));
		CC_BREAK_IF(!m_pGameOverLabel);
		addChild(m_pGameOverLabel);
		bRet = true;
	} while (0);
	return bRet;
}

bool GameOver::initLabelButton()
{
	bool bRet = false;
	do 
	{
		auto recurLabel = Label::createWithTTF("Recur", "fonts/Marker Felt.ttf", 30);
		auto recurButton = MenuItemLabel::create(recurLabel, this, menu_selector(GameOver::recurCallback));
		auto recurMenu = Menu::create(recurButton, NULL);
		// 设置锚点为左下角
		recurButton->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		recurMenu->setPosition(5, 5);
		CC_BREAK_IF(!recurMenu);
		addChild(recurMenu);

		auto exitLabel = Label::createWithTTF("Exit", "fonts/Marker Felt.ttf", 30);
		auto exitButton = MenuItemLabel::create(exitLabel, this, menu_selector(GameOver::exitCallback));
		auto exitMenu = Menu::create(exitButton, NULL);
		// 设置锚点为右下角
		exitButton->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
		exitMenu->setPosition(_contentSize.width - 5, 5);
		CC_BREAK_IF(!exitMenu);
		addChild(exitMenu);

		bRet = true;
	} while (0);
	return bRet;
}

void GameOver::exitCallback(Ref* pSende)
{
	Director::getInstance()->end();
	
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	    exit(0); 
	#endif

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

	EventCustom customEndEvent("game_scene_close_event");
	_eventDispatcher->dispatchEvent(&customEndEvent);

}

void GameOver::recurCallback(Ref* pSende)
{
	//从新初始化 玩家的数据
	auto gameData = GameData::getInstance();
	gameData->againInitGameData();
	Director::getInstance()->popScene();
}

bool GameOver::initPlayerScore()
{
	bool bRet = false;
	//label间的间隔
	int nInterval = 20;
	float nHorizontalInterval = 0.25;
	float fheight = m_pGameOverLabel->getPositionY() - m_pGameOverLabel->getContentSize().height * 0.5f - nInterval;
	do 
	{
		//添加Naml label
		auto playerNameLabel = Label::createWithTTF("Name", "fonts/Marker Felt.ttf", 20);
		playerNameLabel->setPosition(_contentSize.width * nHorizontalInterval, fheight - (playerNameLabel->getContentSize().height) * 0.5f);
		CC_BREAK_IF(!playerNameLabel);
		addChild(playerNameLabel);
		// 添加 ID label
		auto playerIDLabel = Label::createWithTTF("ID", "fonts/Marker Felt.ttf", 20);
		playerIDLabel->setPosition(_contentSize.width * nHorizontalInterval * 2, fheight - (playerIDLabel->getContentSize().height) * 0.5f);
		CC_BREAK_IF(!playerIDLabel);
		addChild(playerIDLabel);

		// 添加 score label
		auto playerScoreLabel = Label::createWithTTF("Score", "fonts/Marker Felt.ttf", 20);
		playerScoreLabel->setPosition(_contentSize.width * nHorizontalInterval * 3, fheight - (playerScoreLabel->getContentSize().height) * 0.5f);
		CC_BREAK_IF(!playerScoreLabel);
		addChild(playerScoreLabel);

		fheight -= ((playerScoreLabel->getContentSize().height) * 0.5f + nInterval);
		auto gameData = GameData::getInstance();
		auto mapPlayerData = gameData->m_MapPlayerData;

		for (const auto& iter : mapPlayerData)
		{
			auto playerData = iter.second;
			//判断打印玩家数据，未参加游戏的玩家不会输出成绩
			if (playerData.nScore == 0)
			{
				continue;
			}
			//添加Naml label
			auto playerNameLabel = Label::createWithTTF(playerData.strPlayerName.c_str(), "fonts/Marker Felt.ttf", 20);
			playerNameLabel->setPosition(_contentSize.width * nHorizontalInterval, fheight - (playerNameLabel->getContentSize().height) * 0.5f);
			// 强转颜色
			Color3B* color = (Color3B*)& playerData.nColour;
			playerNameLabel->setColor(Color3B(color->r, color->g, color->b));
			CC_BREAK_IF(!playerNameLabel);
			addChild(playerNameLabel);

			// 添加 ID label
			char chID[32] = { 0 };
			sprintf(chID, "%d", playerData.nPlayerID);
			auto playerIDLabel = Label::createWithTTF(chID, "fonts/Marker Felt.ttf", 20);
			playerIDLabel->setPosition(_contentSize.width * nHorizontalInterval * 2, fheight - (playerIDLabel->getContentSize().height) * 0.5f);
			playerIDLabel->setColor(Color3B(color->r, color->g, color->b));
			CC_BREAK_IF(!playerIDLabel);
			addChild(playerIDLabel);

			// 添加 score label
			char chScore[32] = { 0 };
			sprintf(chScore, "%d", playerData.nScore);
			auto playerScoreLabel = Label::createWithTTF(chScore, "fonts/Marker Felt.ttf", 20);
			playerScoreLabel->setPosition(_contentSize.width * nHorizontalInterval * 3, fheight - (playerScoreLabel->getContentSize().height) * 0.5f);
			playerScoreLabel->setColor(Color3B(color->r, color->g, color->b));
			CC_BREAK_IF(!playerScoreLabel);
			addChild(playerScoreLabel);

			fheight -= ((playerScoreLabel->getContentSize().height) * 0.5f + nInterval);
		}

		bRet = true;
	} while (0);
	return bRet;
}

