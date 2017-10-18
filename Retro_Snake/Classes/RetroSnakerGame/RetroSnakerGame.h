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
	//按钮回调函数
	/**
	* @ brief 向左移动
	*/
	void menuLeftCallBcak(cocos2d::Ref* Spender);

	/**
	* @ brief 向右移动
	*/
	void menuRightCallBack(cocos2d::Ref* Spender);

	/**
	* @ brief 向上移动
	*/
	void menuUpperCallBack(cocos2d::Ref* Spender);

	/**
	* @ brief 向下移动
	*/
	void menuLowerCallBack(cocos2d::Ref* Spender);

	/**
	* @brief 取消控制方向定时器
	*/
	void unAllDirection();

	/*
	* @breif 接收消息线程回调函数
	*/
	void ThreadRecv();
	/*
	* @breif 发送方向
	*/
	void sendDirection();
	/*
	* @breif 处理任务的回调函数
	*/
	void scheduleTask(float at);
public:
	void createMutex();
private:
	std::vector<TaskMsg* > m_Task;
	cocos2d::Menu* m_pMenuVertical = nullptr;
	cocos2d::Menu* m_pMenuHorizontal = nullptr;
	//游戏状态
	bool m_bState = false;
	SnakeHeadDirection m_SnakeHeadDirection;
	//互斥锁
	HANDLE m_hMutex;


};

#endif  //__RetroSnaker_Game_H__