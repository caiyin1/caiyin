
#include "BackGroundScene.h"
#include "Classes/Sign_in/Sign_In.h"
#include "MakeDraw/MakeDraw.h"



USING_NS_CC;

cocos2d::Scene* BackGroundScene::createScene()
{
	return BackGroundScene::create();
}

bool BackGroundScene::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Scene::init());



//		scheduleOnce(schedule_selector(BackGroundScene::schedulexxx), 1.0f);
		bRet = initDirection(m_strDirectionImage);
		CreateThread(NULL, 0, &ThreadRecv, this, 0, &m_nThreadRecv);

	} while (0);
	return bRet;
}

void BackGroundScene::PassSock()
{

}

void BackGroundScene::schedulexxx(float at)
{
	CCLOG("clientXXXX = %d ", m_clientSock);
}

bool BackGroundScene::initDirection(const std::string& strDirectionImage)
{
	bool bRet = false;
	do
	{
		auto LowerButton = MenuItemImage::create(strDirectionImage, strDirectionImage, this, menu_selector(BackGroundScene::menuLowerCallBack));
		CC_BREAK_IF(!LowerButton);

		auto UpperButton = MenuItemImage::create(strDirectionImage, strDirectionImage, this, menu_selector(BackGroundScene::menuUpperCallBack));
		UpperButton->setRotation(180);
		CC_BREAK_IF(!UpperButton);

		auto LeftButton = MenuItemImage::create(strDirectionImage, strDirectionImage, this, menu_selector(BackGroundScene::menuLeftCallBcak));
		LeftButton->setRotation(90);
		CC_BREAK_IF(!LeftButton);

		auto RightButton = MenuItemImage::create(strDirectionImage, strDirectionImage, this, menu_selector(BackGroundScene::menuRightCallBack));
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
void BackGroundScene::menuLowerCallBack(cocos2d::Ref* Spender)
{

	m_pMenuVertical->setEnabled(false);
	m_pMenuHorizontal->setEnabled(false);
	/*发送转向数据包*/
	struct ClientMessage meage;
	meage.nMsgId = 1;
	meage.nDirection = LOWER;
	meage.clientSock = m_clientSock;
	if (send(m_clientSock, (char *)&meage, sizeof(meage), 0) == 0)
	{ 
		CCLOG("send error!");
	}
}

void BackGroundScene::menuUpperCallBack(cocos2d::Ref* Spender)
{
	m_pMenuVertical->setEnabled(false);
	m_pMenuHorizontal->setEnabled(false);
	/*发送转向数据包*/
	struct ClientMessage meage;
	meage.nMsgId = 1;
	meage.nDirection = UPPER;
	meage.clientSock = m_clientSock;
	if (send(m_clientSock, (char *)&meage, sizeof(meage), 0) == 0)
	{
		CCLOG("send error!");
	}
}



void BackGroundScene::menuRightCallBack(cocos2d::Ref* Spender)
{
	m_pMenuVertical->setEnabled(false);
	m_pMenuHorizontal->setEnabled(false);
	/*发送转向数据包*/
	struct ClientMessage meage;
	meage.nMsgId = 1;
	meage.nDirection = RIGHT;
	meage.clientSock = m_clientSock;
	if (send(m_clientSock, (char *)&meage, sizeof(meage), 0) == 0)
	{
		CCLOG("send error!");
	}
}


void BackGroundScene::menuLeftCallBcak(cocos2d::Ref* Spender)
{
	m_pMenuVertical->setEnabled(false);
	m_pMenuHorizontal->setEnabled(false);
	/*发送转向数据包*/
	struct ClientMessage meage;
	meage.nMsgId = 1;
	meage.nDirection = LEFT;
	meage.clientSock = m_clientSock;
	if (send(m_clientSock, (char *)&meage, sizeof(meage), 0) == 0)
	{
		CCLOG("send error!");
	}
}

void BackGroundScene::scheduleDirection(float at)
{
	if (m_CurrentSnakeHeadDirection != m_RecvSnakeHeadDirection)
	{
		switch (m_RecvSnakeHeadDirection)
		{
		case SnakeHeadLeft:
			break;
		case SnakeHeadRight:
			break;
		case SnakeHeadUpper:
			break;
		case SnakeHeadLower:
			break;
		} 
	}
}

DWORD WINAPI ThreadRecv(LPVOID param)
{
	BackGroundScene* scene = (BackGroundScene*)param;
	
	char *recBuff = new char[100];
	while (recv(scene->m_clientSock, recBuff, 100, 0) == -1)
	{
		CCLOG("scene->m_clientSock = %d", scene->m_clientSock);
	}

	struct ServerMessage *pMsage = (ServerMessage*)&recBuff;
	
	switch (pMsage->nMsgId)
	{
	case 2: 
		scene->m_RecvSnakeHeadDirection = pMsage->nDirection;
		break;
	case 3:
		break;
	case 4:
		break;
	case 1:
		auto SnakeHead = MakeDraw::create(1);
		SnakeHead->setPosition(pMsage->Snake.at(1));
		scene->addChild(SnakeHead);
		break;

	}
	return 0;
}
