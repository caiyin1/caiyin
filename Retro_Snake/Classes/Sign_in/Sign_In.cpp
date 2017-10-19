#include "Classes/Sign_in/Sign_In.h"
#include "GameData/GameData.h"
#include "GameData/message.h"
#include "RetroSnakerGame/RetroSnakerGame.h"




SignIn* SignIn::createScene()
{
	SignIn *pRet = new SignIn;
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return nullptr;
	}
	return pRet;
}

bool SignIn::init()
{
	bool bRet = false;
	if (!Scene::init())
	{
		return bRet;
	}
	do 
	{
		auto GameNameLabel = Label::createWithTTF("Retro Snaker", "fonts/Marker Felt.ttf", 40);
		GameNameLabel->setPosition(Vec2(_contentSize.width * 0.5f, _contentSize.height - GameNameLabel->getContentSize().height));
		CC_BREAK_IF(!GameNameLabel);
		addChild(GameNameLabel);
		
		auto SignInLabel = Label::createWithTTF("Sign In", "fonts/Marker Felt.ttf", 25);
		auto SignInButton = MenuItemLabel::create(SignInLabel, this, menu_selector(SignIn::MenuSignInCallBack));
		auto SignInMenu = Menu::create(SignInButton, NULL);
		SignInMenu->setPosition(_contentSize.width * 0.5f, SignInLabel->getContentSize().height * 2);
		CC_BREAK_IF(!SignInMenu);
		addChild(SignInMenu);

		//schedule(schedule_selector(SignIn::scheduleHanle), 0.01f);
		bRet = true;
		auto pEditBox_name = EditBox::create(CCSizeMake(250, 50), Scale9Sprite::create("D:\\material\\Button.png"));
		if (pEditBox_name == nullptr)
		{
			CCLOG("pEditBox_name = nullprt");
		}
		pEditBox_name->setPosition(_contentSize * 0.5f);
		pEditBox_name->setFontColor(Color3B(0, 0, 0));//设置字体颜色    
		pEditBox_name->setPlaceHolder("Name:");//设置预置文本    
		pEditBox_name->setMaxLength(8);//设置最大长度    
		pEditBox_name->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);//可以输入任何，但是不包括换行   
		pEditBox_name->setInputFlag(cocos2d::ui::EditBox::InputFlag::INITIAL_CAPS_WORD);//设置输入标志位    
		pEditBox_name->setReturnType(cocos2d::ui::EditBox::KeyboardReturnType::DONE);//设置返回类型    
		pEditBox_name->setDelegate(this);//当前类继承CCEditBoxDelegate类    
		pEditBox_name->setTag(101);
		this->addChild(pEditBox_name);
	} while (0);
	return bRet;
}

void SignIn::MenuSignInCallBack(Ref* Spende)
{
	RetroSNakerClient client;
	if (client.initSock())
	{
		return;
	}
	auto ip = client.ResolverAdress("127.0.0.1");
	m_clientSock = client.ConnectServer(ip, MY_PORT);
	if (m_clientSock == SOCKET_ERROR)
	{
		return;
	}
	auto gameData = GameData::getInstance();
	gameData->setSockServer(m_clientSock);
	
	sendSignIn();

}


void SignIn::editBoxEditingDidBegin(EditBox *editBox)
{
	CCLOG("start edit");
}
void SignIn::editBoxEditingDidEnd(EditBox *editBox)
{
	CCLOG("end edit");
}
void SignIn::editBoxReturn(EditBox *editBox)
{
	CCLOG("editbox return");
}
void SignIn::editBoxTextChanged(EditBox *editBox, const std::string &text)
{
	auto editbox = (EditBox*)editBox;
	switch (editBox->getTag())
	{
	case 101:
		CCLOG("EditBox_name changed");
		m_name = text;
		break;
	default:
		break;
	}
}

bool SignIn::sendSignIn()
{
	auto gameData = GameData::getInstance();
	char senBuf[1024];
	std::memset(senBuf, 0, 1024);
	int nSenBufLen = sprintf_s(senBuf, 1024, "%d%d%s", gameData->m_MsgID, 1, m_name);
	m_SignInMsgID = gameData->m_MsgID;
	gameData->m_MsgID++;
	if (send(gameData->getSockServer(), senBuf, nSenBufLen, 0) == SOCKET_ERROR)
	{
		CCLOG("ERROR: send error!");
		return false;
	}
	return true;
}

bool SignIn::recvPlayerData()
{
	char recBuf[1024];
	std::memset(recBuf, 0, 1024);
	auto gameData = GameData::getInstance()->shareGlobal();
	int nSize = 0;
	while (nSize < sizeof(int))
	{
		int nLen = 0;
		nLen = recv(gameData->getSockServer(), recBuf + nSize , 1024, 0);
		if (nLen < 0)
		{
			CCLOG("ERROR: socekt = %d , recv error ", gameData->getSockServer());
			return false;
		}
		nSize += nLen;
	}
	int nMsgLen = getMessageLen(recBuf, sizeof(int));
	while (nMsgLen)
	{
		int nLen = 0;
		nLen = recv(gameData->getSockServer(), recBuf + nSize, 1024, 0);
		if (nLen < 0)
		{
			CCLOG("ERROR: socekt = %d , recv error ", gameData->getSockServer());
			return false;
		}
		nSize += nLen;
	}
	char chMsgPack[MSG_PACK_LENG] = { 0 };
	strncpy(chMsgPack, recBuf, nMsgLen);
	*recBuf = *DeleteMessage(recBuf, nMsgLen, nSize);
	Message::TagMsgHead *msg = (Message::TagMsgHead*) chMsgPack;

	m_RecvMutex.lock();
	m_Task.push_back(msg);
	m_RecvMutex.unlock();
	gameData->setRecBuf(recBuf, nSize - nMsgLen);
	return true;
}

int SignIn::getMessageLen(char chRecBuf[], int nHeadLen)
{
	char chHead[4] = {0};
	strncpy(chHead,chRecBuf, 4);
	return (int)chHead;
}

char* SignIn::DeleteMessage(char chRecBuf[], int nMsgLen, int nBufLen)
{
	for (int i = 0; nMsgLen + i <= nBufLen; i++)
	{
		chRecBuf[i] = chRecBuf[i + nMsgLen];
	}
	memset(chRecBuf + (nBufLen - nMsgLen), 0, 1024 - (nBufLen - nMsgLen));
	return chRecBuf;


}

void SignIn::scheduleTask(float at)
{
	if (!isTask()) return;
	m_RecvMutex.lock();
	if (!isTask())
	{
		m_RecvMutex.unlock();
		return;
	}
	auto MsgTask = *m_Task.begin(); 
	m_Task.erase(m_Task.begin());
	m_RecvMutex.unlock();
	HandleTask(MsgTask);
}

bool SignIn::isTask()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(m_Task.empty());
		bRet = true;
	} while (0);
	return bRet;
}

void SignIn::HandleTask(Message::TagMsgHead* msg)
{
	if (msg->nMessageHead = HEAD_SIGN_IN)
	{
		Message::TagPlayerData *tagPlayerData;
		tagPlayerData = (Message::TagPlayerData*)msg->pChMessage;
		auto gameData = GameData::getInstance();
		gameData->setPlayerID(tagPlayerData->nPlayerID);
		gameData->setPlayerName(tagPlayerData->nPlayerID, tagPlayerData->strPlayerName);
		gameData->setPlayerColour(tagPlayerData->nPlayerID, tagPlayerData->nColour);
		Director::getInstance()->replaceScene(RetroSnakerGame::createScene());
	}
}

//Director


