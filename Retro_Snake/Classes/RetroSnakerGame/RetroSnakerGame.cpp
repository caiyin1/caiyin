#include "RetroSnakerGame.h"



Scene* RetroSnakerGame::createScene()
{
	return RetroSnakerGame::create();
}

bool RetroSnakerGame::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Scene::init());
		CC_BREAK_IF(!initDirection("D:\\material\\Direction.png"));

		createMutex();
		std::thread thRecv(&RetroSnakerGame::ThreadRecv, this);
		thRecv.join();

		bRet = true;
	} while (0);

	return bRet;
}

bool RetroSnakerGame::initDirection(const std::string& strDirectionImage)
{
	bool bRet = false;
	do
	{
		auto LowerButton = MenuItemImage::create(strDirectionImage, strDirectionImage, this, menu_selector(RetroSnakerGame::menuLowerCallBack));
		CC_BREAK_IF(!LowerButton);

		auto UpperButton = MenuItemImage::create(strDirectionImage, strDirectionImage, this, menu_selector(RetroSnakerGame::menuUpperCallBack));
		UpperButton->setRotation(180);
		CC_BREAK_IF(!UpperButton);

		auto LeftButton = MenuItemImage::create(strDirectionImage, strDirectionImage, this, menu_selector(RetroSnakerGame::menuLeftCallBcak));
		LeftButton->setRotation(90);
		CC_BREAK_IF(!LeftButton);

		auto RightButton = MenuItemImage::create(strDirectionImage, strDirectionImage, this, menu_selector(RetroSnakerGame::menuRightCallBack));
		RightButton->setRotation(270);
		CC_BREAK_IF(!RightButton);

		m_pMenuVertical = Menu::create(UpperButton, LowerButton, NULL);
		m_pMenuVertical->alignItemsVerticallyWithPadding(5);
		m_pMenuVertical->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		m_pMenuVertical->setPosition(Vec2(_contentSize.width - LowerButton->getContentSize().width * 2,
			5 + LowerButton->getContentSize().height));
		addChild(m_pMenuVertical);

		m_pMenuHorizontal = Menu::create(LeftButton, RightButton, NULL);
		m_pMenuHorizontal->alignItemsHorizontallyWithPadding(RightButton->getContentSize().height + 10);
		m_pMenuHorizontal->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
		m_pMenuHorizontal->setPosition(_contentSize.width + 20 - RightButton->getContentSize().width * 2.5, 22);
		addChild(m_pMenuHorizontal);
		bRet = true;
	} while (0);
	return bRet;

}

void RetroSnakerGame::ThreadRecv()
{
	auto gameData = GameData::getInstance();
	while (1)
	{
		char chRecvBuf[1024];
		


	}
}

void RetroSnakerGame::sendDirection()
{
	auto gameData = GameData::getInstance();
	char chBuf[1024] = { "0" };
	WaitForSingleObject(m_hMutex, INFINITE);
	int nLen = 4 + 4 + 4 + 4 + 4;
	sprintf_s(chBuf, "%d%d%d%d%d", nLen, gameData->m_MsgID, HEAD_DIRECTION,
		gameData->getPlayerID(), m_SnakeHeadDirection);
	gameData->m_MsgID++;
	ReleaseMutex(m_hMutex);
	if (send(gameData->getSockServer(), chBuf, nLen, 0) == SOCKET_ERROR)
	{
		CCLOG("ERROR : send error ! socket = %d ", gameData->getSockServer());
	}
}

void RetroSnakerGame::createMutex()
{
	m_hMutex = CreateMutex(NULL, FALSE, NULL);
}


void RetroSnakerGame::menuLeftCallBcak(cocos2d::Ref* Spender)
{
	if (m_bState)
	{
		m_pMenuHorizontal->setEnabled(false);
		m_pMenuVertical->setEnabled(false);

		m_SnakeHeadDirection = SnakeHeadLeft;
		sendDirection();
	}
}

void RetroSnakerGame::menuRightCallBack(cocos2d::Ref* Spender)
{
	if (m_bState)
	{
		m_pMenuHorizontal->setEnabled(false);
		m_pMenuVertical->setEnabled(false);

		m_SnakeHeadDirection = SnakeHeadRight;
		sendDirection();
	}
}

void RetroSnakerGame::menuUpperCallBack(cocos2d::Ref* Spender)
{
	if (m_bState)
	{
		m_pMenuHorizontal->setEnabled(false);
		m_pMenuVertical->setEnabled(false);

		m_SnakeHeadDirection = SnakeHeadUpper;
		sendDirection();
	}
}

void RetroSnakerGame::menuLowerCallBack(cocos2d::Ref* Spender)
{
	if (m_bState)
	{
		m_pMenuHorizontal->setEnabled(false);
		m_pMenuVertical->setEnabled(false);

		m_SnakeHeadDirection = SnakeHeadLower;
		sendDirection();
	}
}

void RetroSnakerGame::scheduleTask(float at)
{
	if (m_Task.empty())
	{
		return;
	}
	WaitForSingleObject(m_hMutex, INFINITE);
	if (m_Task.empty())
	{
		return;
	}



}

