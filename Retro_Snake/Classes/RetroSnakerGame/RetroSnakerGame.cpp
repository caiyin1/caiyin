#include "RetroSnakerGame.h"
#include "MakeDraw/MakeDraw.h"
#include "GameOver/GameOver.h"



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
		CC_BREAK_IF(!initDirection("res\\Direction.png"));
		CC_BREAK_IF(!initMyData());
		CC_BREAK_IF(!initRead());
		auto limitSprite = MakeDraw::create(4);
		limitSprite->setPosition(Vec2(320, 0));
		addChild(limitSprite);	
		schedule(schedule_selector(RetroSnakerGame::unPack), 0.05f);
		schedule(schedule_selector(RetroSnakerGame::scheduleSendHeartBeat), 15.0f);
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
	while (true)
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
		//判断游戏是否在界面是否在结算界面
		if(!GameData::getInstance()->m_bRecvThread)
		{
			//删除new出来的对象
			delete[] pChMsg;
			continue;
		}
		//将消息添加到任务队列
		addTaskPack(pChMsg);

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
	gameData->m_MsgID++;
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

void RetroSnakerGame::menuLowerCallBack(cocos2d::Ref* Spende)
{
	if (m_bState)
	{
		m_pMenuHorizontal->setEnabled(false);
		m_pMenuVertical->setEnabled(false);
		m_SnakeHeadDirection = SnakeHeadLower;
		sendDirection();
	}
}

// void RetroSnakerGame::schedulePlayerStateTask(TagPlayerStateData Task)
// {
// 	switch (Task.nHead)
// 	{
// 	case HEAD_READ:
// 		gameData->setPlayerData(Task.nPlayerID, Task);
// 		addPlayerData(Task);
// 		break;
// 	default:
// 		break;
// 	}
// }

char* RetroSnakerGame::deleteMessage(char chRecBuf[], int nMsgLen, int nBufLen)
{
	for (int i = 0; nMsgLen + i <= nBufLen; i++)
	{
		chRecBuf[i] = chRecBuf[i + nMsgLen];
	}
	memset(chRecBuf + (nBufLen - nMsgLen), 0, 1024 - (nBufLen - nMsgLen));
	return chRecBuf;
}

void RetroSnakerGame::unPack(float at)
{
	do
	{
		CC_BREAK_IF(!isTaskPack());
		//获取消息地址
		m_mutexPack.lock();
		auto pChMsg = *m_vTsakPack.begin();
		m_vTsakPack.erase(m_vTsakPack.begin());
		m_mutexPack.unlock();

		Message::TagMsgHead msgHead = *(Message::TagMsgHead*) pChMsg;
		if (GameData::getInstance()->m_bRecvThread)
		{
			switch (msgHead.nMessageHead)
			{
			case HEAD_LOGIN:
				break;
			case HEAD_READ:
			{
				auto gameData = GameData::getInstance();
				Message::TagPlayerData* msg = (Message::TagPlayerData*) pChMsg;
				if (gameData->getPlayerData(msg->nPlayerID)->nPlayerID == -1)
				{
					char* pStart = &msg->chStart;
					TagPlayerStateData playerData;
					for (; *pStart != 0; pStart++)
					{
						playerData.strPlayerName += *pStart;
					}
					playerData.nHead = msg->nMessageHead;
					playerData.nPlayerID = msg->nPlayerID;
					playerData.nState = msg->nState;
					playerData.nColour = msg->nColour;
					playerData.nScore = 0;
					playerData.nDirection = 0;
					gameData->setPlayerData(playerData.nPlayerID, playerData);
					addPlayerData(playerData);
				}
				else
				{
					gameData->getPlayerData(msg->nPlayerID)->nState = msg->nState;
				}
				break;
			}
			case HEAD_SNAKE_POSITION:
			{
				//接收到游戏开始信号
				m_bState = true;

				Message::TagDotPosition* dotPosition = (Message::TagDotPosition*) pChMsg;
				TagSnakePosition tagDotPosition;
				tagDotPosition.nDirection = 0;
				tagDotPosition.nHead = HEAD_DOT;
				tagDotPosition.nPlayerID = 0;
				Size dotSize;
				dotSize.width = dotPosition->DotPos.nPositionX;
				dotSize.height = dotPosition->DotPos.nPositionY;
				tagDotPosition.SnakePosition.push_back(dotSize);
				auto gameData = GameData::getInstance();
				//处理dot的位置消息
				handlPostionTask(tagDotPosition);
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
					//解包结束 处理消息
					handlPostionTask(tagSnakePosition);
				}
				break;
			}
			case HEAD_GAME_SCORE:
			{
				//TagMsgHead 占用12个字节
				int nSize = sizeof(int) * 3;
				int nNum = *(int*)(pChMsg + nSize);
				nSize += sizeof(int);
				auto gameData = GameData::getInstance();
				for (int i = 0; i < nNum; i++)
				{
					int nPlayerID = *(int*)(pChMsg + nSize);
					nSize += sizeof(int);
					int nScore = *(int*)(pChMsg + nSize);
					nSize += sizeof(int);
					gameData->setPlayerScore(nPlayerID, nScore);
				}
				//在游戏结束界面关闭 任务处理功能
				gameData->m_bRecvThread = false;
				//重新初始化IU界面
				againInitUI();
				Director::getInstance()->pushScene(GameOver::CreateScene());
				break;
			}
			case HEAD_COUNTDOWN:
			{
				int nCountDown = *(int*)(pChMsg + sizeof(int) * 3);
				if (m_pCountDown != nullptr)
				{
					removeChild(m_pCountDown);
					m_pCountDown = nullptr;
					// 如果倒计时是0 则不显示
					if (nCountDown == 0)
					{
						break;
					}
				}
				char chCountDown[32] = { 0 };
				sprintf(chCountDown, "Count Down  %d", nCountDown);
				m_pCountDown = Label::createWithTTF(chCountDown, "fonts/Marker Felt.ttf", 20);
				m_pCountDown->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
				m_pCountDown->setPosition(16 * 20 * 0.5f, _contentSize.height);
				CC_BREAK_IF(!m_pCountDown);
				addChild(m_pCountDown);
				break;
			}
			}
		}
		delete[]pChMsg;
	} while (0);
}

void RetroSnakerGame::addPlayerData(TagPlayerStateData& Task)
{
	auto gameData = GameData::getInstance();
	auto PlayerData = gameData->getPlayerData(Task.nPlayerID);
	//判断是名字的Lebal是否为空为空就不添加Label
	if (PlayerData->pLabelName == nullptr)
	{
		char chPlayerName[32] = { 0 };
		sprintf(chPlayerName, "%s  %d", Task.strPlayerName.c_str(), Task.nPlayerID);
		PlayerData->pLabelName = Label::createWithTTF(chPlayerName, "fonts/Marker Felt.ttf", 13);
		PlayerData->pLabelName->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
		PlayerData->pLabelName->setPosition(_contentSize.width, _contentSize.height - (PlayerData->pLabelName->getContentSize().height + 5)  * m_nPlayerNum++);
		Color4B* colour = (Color4B*)&Task.nColour;
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
		m_pMenuRead->setPosition(MAX_POS * 20 * 0.5f, MAX_POS * 20 * 0.5f);
		addChild(m_pMenuRead);
		bRet = true;
	} while (0);
	return bRet;
}

void RetroSnakerGame::handlPostionTask(TagSnakePosition positionTask)
{	
	auto gameData = GameData::getInstance();
	switch (positionTask.nHead)
	{
	case HEAD_SNAKE:
	{
		
		auto playerData = gameData->getPlayerData(positionTask.nPlayerID);
		// 如果ID为 -1表未找到该玩家数据
		if (playerData->nPlayerID == -1)
		{
			return;
		}
		//解析蛇的颜色
		Color3B color = *(Color3B*)&playerData->nColour;
		//如果蛇不存在的时候初始化蛇头
		if (playerData->SnakeBody.size() == 0)
		{
			auto SnakeHead = MakeDraw::create(1, Color4B(color));
			SnakeHead->setPosition(*positionTask.SnakePosition.begin());
			playerData->SnakeBody.push_back(SnakeHead);
		
			addChild(SnakeHead);
		}
		else
		{
			int i = 0;
			for (; i < playerData->SnakeBody.size(); i++)
			{
				auto spriteSnake = playerData->SnakeBody[i];
				spriteSnake->setPosition(positionTask.SnakePosition[i]);
			}
			for (; i < positionTask.SnakePosition.size(); i++)
			{
				auto SnakeBody = MakeDraw::create(2, Color4B(color));
				SnakeBody->setPosition(positionTask.SnakePosition[i]);
				playerData->SnakeBody.push_back(SnakeBody);
				addChild(SnakeBody);
			}
		}
		setSnakeDirection(positionTask.nPlayerID, positionTask.nDirection);
		break;
	}
	case HEAD_DOT:
	{
		if (m_pDotDraw == nullptr)
		{
			m_pDotDraw = MakeDraw::create(3);
			m_pDotDraw->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			m_pDotDraw->setPosition(*positionTask.SnakePosition.begin());
			addChild(m_pDotDraw);
		}
		else
		{
			auto dotSize = m_pDotDraw->getPosition();
			auto recvPosition = *positionTask.SnakePosition.begin();
			if (dotSize != recvPosition)
			{
				m_pDotDraw->setPosition(*positionTask.SnakePosition.begin());
				CCLOG("dot Pos x = %f\t y = %f\n", positionTask.SnakePosition.begin()->width, positionTask.SnakePosition.begin()->height);
			}
		}
		break;
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
	m_pMenuRead->setVisible(false);
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
			Color3B* colour = (Color3B*)&PlayerData->nColour;
			PlayerData->pLabelName->setColor(Color3B(colour->r, colour->g, colour->b));
			CC_BREAK_IF(!PlayerData->pLabelName);
			addChild(PlayerData->pLabelName);
		}
		bRet = true;
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
		if (nPlayerID == gameData->getPlayerID())
		{
			m_pMenuHorizontal->setEnabled(true);
			m_pMenuVertical->setEnabled(false);
		}
 		break;
 	case SnakeHeadRight:
 		SnakeHead->setRotation(90);
		if (nPlayerID == gameData->getPlayerID())
		{
			m_pMenuHorizontal->setEnabled(false);
			m_pMenuVertical->setEnabled(true);
		} 		break;
 	case SnakeHeadLower:
 		SnakeHead->setRotation(180);
		if (nPlayerID == gameData->getPlayerID())
		{
			m_pMenuHorizontal->setEnabled(true);
			m_pMenuVertical->setEnabled(false);
		}
 		break;
 	case SnakeHeadLeft:
 		SnakeHead->setRotation(270);
		if (nPlayerID == gameData->getPlayerID())
		{
			m_pMenuHorizontal->setEnabled(false);
			m_pMenuVertical->setEnabled(true);
		}
		break;
	}
	
}

void RetroSnakerGame::addTaskPack(char* pChPack)
{
	
	m_mutexPack.lock();
	m_vTsakPack.push_back(pChPack);
	m_mutexPack.unlock();
}

bool RetroSnakerGame::isTaskPack()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(m_vTsakPack.empty());
		bRet = true;
	} while (0);
	return bRet;
}

void RetroSnakerGame::againInitUI()
{
	//显示ready按钮
	m_pMenuRead->setVisible(true);
	//删除玩家信息
	auto gameData = GameData::getInstance();
	gameData->m_PlayerDataMutex.lock();
	for (auto iter : gameData->m_MapPlayerData)
	{
		auto playerData = iter.second;
		//删除其他用户的 数据
		if (playerData.nPlayerID != gameData->getPlayerID())
		{
			removeChild(playerData.pLabelName);
		}
		//删除蛇
		for (int i = 0; i < playerData.SnakeBody.size(); i++)
		{
			removeChild(playerData.SnakeBody[i]);
		}
	}
	gameData->m_PlayerDataMutex.unlock();
	//删除点的
	removeChild(m_pDotDraw);
	m_pDotDraw = nullptr;
	//玩家数剩下自己 +1
	m_nPlayerNum = 1;
}

void RetroSnakerGame::scheduleSendHeartBeat(float at)
{
	auto gameData = GameData::getInstance();
	Message::TagMsgHead breatBeat;
	breatBeat.nMessageHead = HEAD_HEARTBEAT;
	breatBeat.nMsgLeng = sizeof(int) * 5;
	breatBeat.nMsgID = gameData->m_MsgID++;
	char sendBuf[32] = { 0 };
	int nSize = sizeof(int) * 3;
	memcpy(sendBuf, &breatBeat, nSize);
	int nPlayerID = gameData->getPlayerID();
	memcpy(sendBuf, &nPlayerID, sizeof(int));
	nSize += sizeof(int);
	int nHeartBeat = 1;
	memcpy(sendBuf, &nHeartBeat, sizeof(int));
	send(gameData->getSockServer(), sendBuf, breatBeat.nMsgLeng, 0);
}

