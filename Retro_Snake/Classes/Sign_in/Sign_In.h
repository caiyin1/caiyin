#ifndef __SIGN_IN_ H__
#define __SIGN_IN_H__

#include "cocos-ext.h"
#include "cocos2d.h"
#include <string.h>
#include < iostream > 
#include <list>
#include <thread>
#include "Client/RetroSnakerClient.h"
#include "MakeDraw/MakeDraw.h"
#include "GameData/message.h"
USING_NS_CC_EXT;
USING_NS_CC;

class SignIn : public Scene , public EditBoxDelegate
{
public:
	static SignIn* createScene();
	virtual bool init();
	bool initGame();
	void MenuSignInCallBack(Ref* Spende);
	void RecvThread();
public:
	virtual void editBoxEditingDidBegin(cocos2d::extension::EditBox* editBox);
	virtual void editBoxEditingDidEnd(cocos2d::extension::EditBox* editBox);
	virtual void editBoxTextChanged(cocos2d::extension::EditBox* editBox, const std::string &text);
	virtual void editBoxReturn(cocos2d::extension::EditBox *editBox);
public:
	SOCKET m_clientSock;
	MakeDraw *m_pDot = nullptr;
	std::string m_name;
private:
	std::mutex m_RecvMutex;
	int m_SignInMsgID;
private:
	bool sendSignIn();
	bool recvPlayerData();
	char* getMessageHead(char chRecBuf[], int nHeadLen);
};


#endif //__SIGN_IN_ H__