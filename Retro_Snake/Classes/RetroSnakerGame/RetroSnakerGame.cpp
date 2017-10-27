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
 		std::thread thRecv(&RetroSnakerGame::ThreadRecv, this);
		CC_BREAK_IF(!initRead());
		CC_BREAK_IF(!initMyData());
 		thRecv.detach();
		//schedule();
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
			if (nLen == SOCKET_ERROR)
			{
				closesocket(gameData->getSockServer());
				return;
			}
			nSize += nLen;
		}
		Message::TagMsgHead msgHead = *(Message::TagMsgHead*) chRecvBuf;
		while (msgHead.nMsgLeng < nSize)
		{
			int nLen = recv(gameData->getSockServer(), chRecvBuf + nSize, MSG_PACK_LENG, 0);
			if (nLen == SOCKET_ERROR)
			{
				closesocket(gameData->getSockServer());
				return;
			}
			nSize += nLen;
		}
		char* pChMsg = new char[msgHead.nMsgLeng];
		std::memset(pChMsg, 0, msgHead.nMsgLeng);
		memcpy(pChMsg, chRecvBuf, msgHead.nMsgLeng);
		*chRecvBuf = *DeleteMessage(chRecvBuf, msgHead.nMsgLeng, nSize);
		unPack(&msgHead, pChMsg);
		delete[]pChMsg;
	}
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
	if (gameData->isPlayerStateTask())
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

char* RetroSnakerGame::DeleteMessage(char chRecBuf[], int nMsgLen, int nBufLen)
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
			char* pStart = &msg.chStart;
			TagPlayerStateData TaskPack;
			for (; pStart != 0; pStart++)
			{
				TaskPack.strPlayerName += *pStart;
			}
			TaskPack.nHead = msg.nMessageHead;
			TaskPack.nPlayerID = msg.nPlayerID;
			TaskPack.nState = msg.nState;
			TaskPack.nColour = msg.nColour;
			gameData->addPlayerStateTask(TaskPack);
			break;
		}
	case HEAD_SNAKE_POSITION:
		{
			int nSnakeNum = *(int*)(pChMsg + sizeof(*msgHead));
			int nSize = sizeof(*msgHead) + sizeof(int);
			for (int i = 0; i < nSnakeNum; i++)
			{
				int nPlayerID = *(int*)(pChMsg + nSize);
				nSize += sizeof(int);
				int nSnakeLen = *(int*)(pChMsg + nSize);
				nSize += sizeof(int);
				int nHeadDirection = *(int*)(pChMsg + nSize);
				nSize += sizeof(int);
				TagSnakePosition tagSnakePosition;
				tagSnakePosition.nPlayerID = nPlayerID;
				tagSnakePosition.nDirection = nHeadDirection;
				for (int j = 0; j < nSnakeLen; j++)
				{
					Size size;
					size.width = *(int *)(pChMsg + nSize);
					nSize += sizeof(int);
					size.height = *(int *)(pChMsg + nSize);
					nSize += sizeof(int);
					tagSnakePosition.SnakePosition.push_back(size);
				}
				auto gameData = GameData::getInstance();
				gameData->setSnakePositionTask(tagSnakePosition);
			}
		}
	}
}

void RetroSnakerGame::addPlayerData(TagPlayerStateData& Task)
{
	auto gameData = GameData::getInstance();
	auto PlayerData = gameData->getPlayerData(Task.nPlayerID);
	if (PlayerData->pLabelName == nullptr)
	{
		char chPlayerName[32] = { 0 };
		sprintf(chPlayerName, "%s%d", Task.strPlayerName, Task.nPlayerID);
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
		auto labelRead = Label::createWithTTF("×¼±¸", "fonts/Marker Felt.ttf", 20);
		auto ReadButton = MenuItemLabel::create(labelRead, this, menu_selector(RetroSnakerGame::ReadCallBack));
		
		m_pMenuRead = Menu::create(ReadButton, NULL);
		m_pMenuRead->setPosition(Vec2((_contentSize.width - m_pMenuVertical->getContentSize().width)* 0.5f, _contentSize.height));
		addChild(m_pMenuRead);
		bRet = true;
	} while (0);
	return bRet;
}

void RetroSnakerGame::scheduleSnakePostionTask(float at)
{
	auto gameData = GameData::getInstance();
	if (gameData->isSnakePositionTask())
	{
		auto PositionTask = gameData->getSnakePositionTask();
		auto playerData = gameData->getPlayerData(PositionTask->nPlayerID);
		if (playerData->SnakeBody.size() == 0)
		{
			auto SnakeHead = MakeDraw::create(1);
			playerData->SnakeBody.push_back(SnakeHead);
			addChild(SnakeHead);
		}
		else
		{
			

		}

	}
}

void RetroSnakerGame::ReadCallBack(Ref* Spende)
{
	auto gameData = GameData::getInstance();
	Message::TagRead tagRead;
	tagRead.nMessageHead = HEAD_READ;
	tagRead.nMsgID = gameData->m_MsgID;
	gameData->m_MsgID++;
	tagRead.nPlayerID = gameData->getPlayerID();
	tagRead.nRead = 1;
	tagRead.nMsgLeng = sizeof(int)* 5;
	if (send(gameData->getSockServer(), (char*)&tagRead, tagRead.nRead, 0) == SOCKET_ERROR)
	{
		CCLog("ERROR: socek(%d) error! ", gameData->getSockServer());
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
			sprintf(chPlayerName, "%s%d", PlayerData->strPlayerName, PlayerData->nPlayerID);
			PlayerData->pLabelName = Label::createWithTTF(chPlayerName, "fonts/Marker Felt.ttf", 5);
			PlayerData->pLabelName->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
			PlayerData->pLabelName->setPosition(_contentSize.width, _contentSize.height - (PlayerData->pLabelName->getContentSize().height + 5)  * m_nPlayerNum++);
			Colour3b* colour = (Colour3b*)&PlayerData->nColour;
			PlayerData->pLabelName->setColor(Color3B(colour->r, colour->g, colour->b));
			CC_BREAK_IF(!PlayerData->pLabelName);
			addChild(PlayerData->pLabelName);
		}
		bRet = true;
	} while (0);
	return bRet;
}




