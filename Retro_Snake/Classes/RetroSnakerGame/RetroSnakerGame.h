#ifndef __RetroSnaker_Game_H__
#define __RetroSnaker_Game_H__
#include "cocos2d.h"
#include <string>
#include <vector>
#include "GameData/message.h"
#include "GameData/GameData.h"


USING_NS_CC;


class RetroSnakerGame : public Scene
{
public:
	static Scene* createScene();
	bool init();
	CREATE_FUNC(RetroSnakerGame);

private:
	bool initMyData();
	bool initDirection(const std::string& strDirectionImage);
	//��ť�ص�����
	/**
	* @ brief �����ƶ�
	*/
	void menuLeftCallBcak(cocos2d::Ref* Spender);

	/**
	* @ brief �����ƶ�
	*/
	void menuRightCallBack(cocos2d::Ref* Spender);

	/**
	* @ brief �����ƶ�
	*/
	void menuUpperCallBack(cocos2d::Ref* Spender);

	/**
	* @ brief �����ƶ�
	*/
	void menuLowerCallBack(cocos2d::Ref* Spender);

	/**
	* @brief ȡ�����Ʒ���ʱ��
	*/
	void unAllDirection();

	/*
	* @breif ������Ϣ�̻߳ص�����
	*/
	void threadRecv();
	/*
	* @breif ���ͷ���
	*/
	void sendDirection();

	bool initRead();
	/*
	* @breif �������ID���ƵĻص�����
	*/
	void schedulePlayerStateTask(float at);
	/* 
	* @breif ������λ�õĻص����� 
	*/
	void scheduleSnakePostionTask(float at);
	/*
	* @breif ����׼������ť�Ļص�����
	*/
	void readCallBack(Ref* Spende);
public:
	int getHeadLen(char recBuf[], int nlen);
	/*
	* @breif ɾ���Ѿ����յ����ݰ�
	* @param chRecBuf���յ�����
	* @param nMsgLen��һ�����ĳ���
	* @param nBufLen ���յĳ��ȣ�chRecBuf���õ����ݣ�
	*/
	char* deleteMessage(char chRecBuf[], int nMsgLen, int nBufLen);
	/*
	* @breif ���
	* @param msgHead ͷ��Ϣ
	* @param pChMsg ������������
	*/
	void unPack(Message::TagMsgHead *pMsgHead, char* pChMsg);

	/*
	* @breif ������״̬���ݵ����� 
	*/
	void addPlayerData(TagPlayerStateData& Task);
	/*
	* @breif ������ͷ���� 
	*/
	void setSnakeDirection(int nPlayerID, int nDirection);
	void setDotPosition(Message::TagDotPosition* pMsg);
	void setDirectionMem(int nDireciton);
private:
	std::vector<TaskMsg* > m_Task;
	cocos2d::Menu* m_pMenuVertical = nullptr;
	cocos2d::Menu* m_pMenuHorizontal = nullptr;
	//��Ϸ״̬
	bool m_bState = false;
	SnakeHeadDirection m_SnakeHeadDirection;
	//������
	std::mutex m_mutexDirection;
	int m_nPlayerNum = 1;
	cocos2d::Menu* m_pMenuRead;
	MakeDraw* m_pDotDraw = nullptr;
};

#endif  //__RetroSnaker_Game_H__