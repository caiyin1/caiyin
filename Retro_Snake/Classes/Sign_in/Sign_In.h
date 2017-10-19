#ifndef __SIGN_IN_ H__
#define __SIGN_IN_H__

#include "cocos-ext.h"
#include "cocos2d.h"
#include <string.h>
#include <iostream > 
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
	/*
	* @breif 4个关于编辑框的重载函数 
	*/
	virtual void editBoxEditingDidBegin(cocos2d::extension::EditBox* editBox);
	virtual void editBoxEditingDidEnd(cocos2d::extension::EditBox* editBox);
	virtual void editBoxTextChanged(cocos2d::extension::EditBox* editBox, const std::string &text);
	virtual void editBoxReturn(cocos2d::extension::EditBox *editBox);
	/*
	* @breif 定时扫描 任务列表
	*/
	void scheduleTask(float at);
	/*
	* @breif 判断任务列表是否为空 
	*/
	bool isTask();
public:
	SOCKET m_clientSock;
	MakeDraw *m_pDot = nullptr;
	std::string m_name;
private:
	std::mutex m_RecvMutex;
	int m_SignInMsgID;
	std::vector<Message::TagMsgHead* > m_Task;
private:
	bool sendSignIn();
	bool recvPlayerData();
	int getMessageLen(char chRecBuf[], int nHeadLen);
	/*
	* @breif 删除已经接收的数据包
	* @param chRecBuf接收的数据
	* @param nMsgLen上一个包的长度
	* @param nBufLen 接收的长度（chRecBuf有用的数据）
	*/
	char* DeleteMessage(char chRecBuf[], int nMsgLen, int nBufLen);
	void HandleTask(Message::TagMsgHead* msg);
};


#endif //__SIGN_IN_ H__