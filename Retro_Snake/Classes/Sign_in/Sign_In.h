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
	* @breif 4�����ڱ༭������غ��� 
	*/
	virtual void editBoxEditingDidBegin(cocos2d::extension::EditBox* editBox);
	virtual void editBoxEditingDidEnd(cocos2d::extension::EditBox* editBox);
	virtual void editBoxTextChanged(cocos2d::extension::EditBox* editBox, const std::string &text);
	virtual void editBoxReturn(cocos2d::extension::EditBox *editBox);
	/*
	* @breif ��ʱɨ�� �����б�
	*/
	void scheduleTask(float at);
	/*
	* @breif �ж������б��Ƿ�Ϊ�� 
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
	* @breif ɾ���Ѿ����յ����ݰ�
	* @param chRecBuf���յ�����
	* @param nMsgLen��һ�����ĳ���
	* @param nBufLen ���յĳ��ȣ�chRecBuf���õ����ݣ�
	*/
	char* DeleteMessage(char chRecBuf[], int nMsgLen, int nBufLen);
	void HandleTask(Message::TagMsgHead* msg);
};


#endif //__SIGN_IN_ H__