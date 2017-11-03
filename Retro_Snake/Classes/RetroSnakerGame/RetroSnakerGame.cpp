#include "RetroSnakerGame.h"
#include "MakeDraw/MakeDraw.h"



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
		CC_BREAK_IF(!initMyData());
		CC_BREAK_IF(!initRead());
		schedule(schedule_selector(RetroSnakerGame::schedulePlayerStateTask), 0.05f);
		schedule(schedule_selector(RetroSnakerGame::scheduleSnakePostionTask), 0.05f);
 		std::thread thRecv(&RetroSnakerGame::threadRecv, this);
 		thRecv.detach();
		
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

void RetroSnakerGame::threadRecv()
{
	int nSize = 0;	
	char chRecvBuf[MSG_PACK_LENG] = { 0 };
	while (1)
	{
		auto gameData = GameData::getInstance();
		if (gameData->getSockServer() == 0)
		{
			continue;
		}
		if (gameData->getRecvLen() > 0)
		{
			memcpy(chRecvBuf, gameData->getRecBuf(), gameData->getRecvLen());
			nSize = gameData->getRecvLen();
			gameData->deleteRecvData();
		}
		while (nSize < sizeof(int) * 3)
		{
			int nLen = recv(gameData->getSockServer(), chRecvBuf + nSize, MSG_PACK_LENG, 0);
			if (nLen == SOCKET_ERROR || nLen == 0)
			{
				closesocket(gameData->getSockServer());
				return;
			}
			nSize += nLen;
		}
		Message::TagMsgHead msgHead = *(Message::TagMsgHead*) chRecvBuf;
		while (msgHead.nMsgLeng > nSize)
		{
			int nLen = recv(gameData->getSockServer(), chRecvBuf + nSize, MSG_PACK_LENG, 0);
			if (nLen == SOCKET_ERROR || nLen == 0)
			{
				closesocket(gameData->getSockServer());
				return;
			}
			nSize += nLen;
		}
		char* pChMsg = new char[msgHead.nMsgLeng];
		std::memset(pChMsg, 0, msgHead.nMsgLeng);
		memcpy(pChMsg, chRecvBuf, msgHead.nMsgLeng);
		CCLOG("RecvBuf = %s", pChMsg);
		*chRecvBuf = *deleteMessage(chRecvBuf, msgHead.nMsgLeng, nSize);
		nSize -= msgHead.nMsgLeng;
		unPack(&msgHead, pChMsg);
		delete[]pChMsg;
	}
	CCLOG("ERROR : exit RecvThread!!!");
}

void RetroSnakerGame::sendDirection()
{
	auto gameData = GameData::getInstance();
	Message::TagClientDirection senBuf;
	senBuf.nMessageHead = HEAD_DIRECTION;
	senBuf.nMsgID = gameData->m_MsgID;
	senBuf.nPlayerID = gameData->getPlayerID();
	senBuf.nDirection = m_SnakeHeadDirection;
	senBuf.nMsgLeng = sizeof(int)* 5;
	m_mutexDirection.lock();
	gameData->m_MsgID++;
	m_mutexDirection.unlock();
	if (send(gameData->getSockServer(), (char *) & senBuf, senBuf.nMsgLeng, 0) == SOCKET_ERROR)
	{
		CCLOG("ERROR : send error ! socket = %d ", gameData->getSockServer());
	}
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

void RetroSnakerGame::schedulePlayerStateTask(float at)
{
	auto gameData = GameData::getInstance();
	if (!gameData->isPlayerStateTask())
	{
		return;
	}
	auto Task = gameData->getPlayerStateTask();
	switch (Task.nHead)
	{
	case HEAD_READ:
		gameData->setPlayerData(Task.nPlayerID, Task);
		addPlayerData(Task);
		break;
	default:
		break;
	}
}

char* RetroSnakerGame::deleteMessage(char chRecBuf[], int nMsgLen, int nBufLen)
{
	for (int i = 0; nMsgLen + i <= nBufLen; i++)
	{
		chRecBuf[i] = chRecBuf[i + nMsgLen];
	}
	memset(chRecBuf + (nBufLen - nMsgLen), 0, 1024 - (nBufLen - nMsgLen));
	return chRecBuf;
}

void RetroSnakerGame::unPack(Message::TagMsgHead *msgHead, char* pChMsg)
{
	switch (msgHead->nMessageHead)
	{
	case HEAD_LOGIN:
		break;
	case HEAD_READ:
		{
			auto gameData = GameData::getInstance();
			Message::TagPlayerData msg = *(Message::TagPlayerData*) pChMsg;
			if (gameData->getPlayerData(msg.nPlayerID)->nPlayerID == -1)
			{
				char* pStart = &msg.chStart;
				TagPlayerStateData PlayerData;
				for ( ;pStart != 0; pStart++)
				{
					PlayerData.strPlayerName += *pStart;
				}
				PlayerData.nPlayerID = msg.nPlayerID;
				PlayerData.nState = msg.nState;
				PlayerData.nColour = msg.nColour;
				gameData->setPlayerData(msg.nPlayerID, PlayerData);
				addPlayerData(PlayerData);
			}
			else
			{
				gameData->getPlayerData(msg.nPlayerID)->nState = msg.nState;
			}
			break;
		}
	case HEAD_SNAKE_POSITION:
		{
			//接收到游戏开始信号
			m_bState = true;

			Message::TagDotPosition* dotPosition = (Message::TagDotPosition* ) pChMsg;
			TagSnakePosition tagDotPosition;
			tagDotPosition.nDirection = 0;
			tagDotPosition.nHead = HEAD_DOT;
			tagDotPosition.nPlayerID = 0;
			Size dotSize;
			dotSize.width = dotPosition->DotPos.nPositionX;
			dotSize.height = dotPosition->DotPos.nPositionY;
			tagDotPosition.SnakePosition.push_back(dotSize);
			auto gameData = GameData::getInstance();
			gameData->setSnakePositionTask(tagDotPosition);
			int nSize = sizeof(int) * 6;
			for (int i = 0; i < dotPosition->nSnakeNum; i++)
			{
				int nSnakeLen = *(int*)(pChMsg + nSize);
				nSize += sizeof(int);
				int nPlayerID = *(int*)(pChMsg + nSize);
				nSize += sizeof(int);
				int nHeadDirection = *(int*)(pChMsg + nSize);
				nSize += sizeof(int);
				TagSnakePosition tagSnakePosition;
				tagSnakePosition.nHead = HEAD_SNAKE;
				tagSnakePosition.nPlayerID = nPlayerID;
				tagSnakePosition.nDirection = nHeadDirection;
				for (int j = 0; j < nSnakeLen; j++)
				{
					Size size;
					size.width = *(int *)(pChMsg + nSize);
					int PosX = *(int *)(pChMsg + nSize);

					nSize += sizeof(float);
					size.height = *(int *)(pChMsg + nSize);
					int nPosY = *(int *)(pChMsg + nSize);
					nSize += sizeof(float);
					tagSnakePosition.SnakePosition.push_back(size);
				}
				gameData->setSnakePositionTask(tagSnakePosition);
			}
		}
	}
}

void RetroSnakerGame::addPlayerData(TagPlayerStateData& Task)
{
	auto gameData = GameData::getInstance();
	auto PlayerData = gameData->getPlayerData(Task.nPlayerID);
	//判断是名字的Lebal是否为空为空就不添加Label
	if (PlayerData->pLabelName == nullptr)
	{
		char chPlayerName[32] = { 0 };
		sprintf(chPlayerName, "%s %d", Task.strPlayerName, Task.nPlayerID);
		PlayerData->pLabelName = Label::createWithTTF(chPlayerName, "fonts/Marker Felt.ttf", 5);
		PlayerData->pLabelName->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
		PlayerData->pLabelName->setPosition(_contentSize.width, _contentSize.height - (PlayerData->pLabelName->getContentSize().height + 5)  * m_nPlayerNum++);
		Colour3b* colour = (Colour3b*)&Task.nColour;
		PlayerData->pLabelName->setColor(Color3B(colour->r, colour->g, colour->b));
		addChild(PlayerData->pLabelName);
	}
}

bool RetroSnakerGame::initRead()
{
	bool bRet = false;
	do 
	{ 
		auto labelRead = Label::createWithTTF("Read", "fonts/Marker Felt.ttf", 20);
		auto ReadButton = MenuItemLabel::create(labelRead, this, menu_selector(RetroSnakerGame::readCallBack));		
		m_pMenuRead = Menu::create(ReadButton, NULL);
		m_pMenuRead->setPosition(_contentSize * 0.5f);
		addChild(m_pMenuRead);
		bRet = true;
	} while (0);
	return bRet;
}

void RetroSnakerGame::scheduleSnakePostionTask(float at)
{
	auto gameData = GameData::getInstance();
	if (!gameData->isSnakePositionTask())
	{
		auto PositionTask = gameData->getSnakePositionTask();
		switch (PositionTask.nHead)
		{
		case HEAD_SNAKE:
			{
				auto playerData = gameData->getPlayerData(PositionTask.nPlayerID);
				if (playerData->SnakeBody.size() == 0)
				{
					auto SnakeHead = MakeDraw::create(1);
					SnakeHead->setPosition(*PositionTask.SnakePosition.begin());
					playerData->SnakeBody.push_back(SnakeHead);
					Color4B* colour = (Color4B*)playerData->nColour;
					//	SnakeHead->setColor(Color3B(colour->r, colour->b, colour->b));
					addChild(SnakeHead);
				}
				else
				{
					int i = 0;
					for (; i < playerData->SnakeBody.size(); i++)
					{
						auto spriteSnake = playerData->SnakeBody[i];
						spriteSnake->setPosition(PositionTask.SnakePosition[i]);
					}
					for (; i < PositionTask.SnakePosition.size(); i++)
					{
						auto SnakeBody = MakeDraw::create(2);
						SnakeBody->setPosition(*PositionTask.SnakePosition.begin());
						Color4B* colour = (Color4B*)playerData->nColour;
					//	SnakeBody->setColor(Color3B(colour->r, colour->b, colour->b));
						playerData->SnakeBody.push_back(SnakeBody);
						addChild(SnakeBody);
					}
				}
		
				setSnakeDirection(PositionTask.nPlayerID, PositionTask.nDirection);
				break;
			}
		case HEAD_DOT:
			{
				if (m_pDotDraw == nullptr)
				{
					m_pDotDraw = MakeDraw::create(3);
					m_pDotDraw->setPosition(*PositionTask.SnakePosition.begin());
					addChild(m_pDotDraw);
				}
				else
				{
					auto dotSize = m_pDotDraw->getPosition();
					auto recvPosition = *PositionTask.SnakePosition.begin();
					if ( dotSize != recvPosition)
					{
						m_pDotDraw->setPosition(*PositionTask.SnakePosition.begin());
					}
				}
				break;
			}

		}
	}
}

void RetroSnakerGame::readCallBack(Ref* Spende)
{
	auto gameData = GameData::getInstance();
	Message::TagRead tagRead;
	tagRead.nMessageHead = HEAD_READ;
	tagRead.nMsgID = gameData->m_MsgID;
	gameData->m_MsgID++;
	tagRead.nPlayerID = gameData->getPlayerID();
	tagRead.nRead = 1;
	tagRead.nMsgLeng = sizeof(int)* 5;
	if (send(gameData->getSockServer(), (char*)&tagRead, tagRead.nMsgLeng, 0) == SOCKET_ERROR)
	{
		///CCLog("ERROR: socek(%d) error! ", gameData->getSockServer());
		return;
	}
	removeChild(m_pMenuRead);
}

bool RetroSnakerGame::initMyData()
{
	bool bRet = false;
	do 
	{
		auto gameData = GameData::getInstance();
		auto PlayerData = gameData->getPlayerData(gameData->getPlayerID());
		if (PlayerData->pLabelName == nullptr)
		{
			char chPlayerName[32] = { 0 };
			sprintf(chPlayerName, "%s  %d", PlayerData->strPlayerName.c_str(), PlayerData->nPlayerID);
			PlayerData->pLabelName = Label::createWithTTF(chPlayerName, "fonts/Marker Felt.ttf", 13);
			PlayerData->pLabelName->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
			PlayerData->pLabelName->setPosition(_contentSize.width, _contentSize.height - (PlayerData->pLabelName->getContentSize().height + 2)  * m_nPlayerNum++);
			Color4B* colour = (Color4B*)&PlayerData->nColour;
			PlayerData->pLabelName->setColor(Color3B(colour->r, colour->g, colour->b));
			CC_BREAK_IF(!PlayerData->pLabelName);
			addChild(PlayerData->pLabelName);
			bRet = true;
		}
	} while (0);
	return bRet;
}

void RetroSnakerGame::setSnakeDirection(int nPlayerID, int nDirection)
{
	auto gameData = GameData::getInstance();
	auto playerData = gameData->getPlayerData(nPlayerID);
	auto SnakeHead = playerData->SnakeBody[0];
	switch (nDirection)
	{
 	case SnakeHeadUpper:
 		SnakeHead->setRotation(0);
		m_pMenuHorizontal->setEnabled(true);
		m_pMenuVertical->setEnabled(false);
 		break;
 	case SnakeHeadRight:
 		SnakeHead->setRotation(90);
		m_pMenuHorizontal->setEnabled(false);
		m_pMenuVertical->setEnabled(true);
 		break;
 	case SnakeHeadLower:
 		SnakeHead->setRotation(180);
		m_pMenuHorizontal->setEnabled(true);
		m_pMenuVertical->setEnabled(false);
 		break;
 	case SnakeHeadLeft:
 		SnakeHead->setRotation(270);
		m_pMenuHorizontal->setEnabled(false);
		m_pMenuVertical->setEnabled(true);
		break;
	default:
		break;
	}
	
}


// void RetroSnakerGame::setDotPosition(Message::TagDotPosition* Msg)
// {
// 	if (m_pDotDraw == nullptr)
// 	{
// 		m_pDotDraw = MakeDraw::create(3);
// 		m_pDotDraw->setPosition(Vec2(Msg->DotPos.nPositionX, Msg->DotPos.nPositionY));
// 		addChild(m_pDotDraw);
// 		auto DotDraw = MakeDraw::create(3);
// 		DotDraw->setPosition(Vec2(20, 40));
// 		this->addChild(DotDraw);
// 		
// 	}
// 	else
// 	{
// 		m_pDotDraw->setPosition(Vec2(Msg->DotPos.nPositionX, Msg->DotPos.nPositionY));
// 	}
// 	CCLOG("Dot x = %d, y = %d\n", Msg->DotPos.nPositionX, Msg->DotPos.nPositionY);
// }




