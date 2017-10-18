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
	void ThreadRecv();
	/*
	* @breif ���ͷ���
	*/
	void sendDirection();
	/*
	* @breif ��������Ļص�����
	*/
	void scheduleTask(float at);
public:
	void createMutex();
private:
	std::vector<TaskMsg* > m_Task;
	cocos2d::Menu* m_pMenuVertical = nullptr;
	cocos2d::Menu* m_pMenuHorizontal = nullptr;
	//��Ϸ״̬
	bool m_bState = false;
	SnakeHeadDirection m_SnakeHeadDirection;
	//������
	HANDLE m_hMutex;


};

#endif  //__RetroSnaker_Game_H__