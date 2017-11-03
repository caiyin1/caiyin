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
	void threadRecv();
	/*
	* @breif 发送方向
	*/
	void sendDirection();

	bool initRead();
	/*
	* @breif 处理玩家ID名称的回调函数
	*/
	void schedulePlayerStateTask(float at);
	/* 
	* @breif 处理蛇位置的回掉函数 
	*/
	void scheduleSnakePostionTask(float at);
	/*
	* @breif 发送准备请求按钮的回调函数
	*/
	void readCallBack(Ref* Spende);
public:
	int getHeadLen(char recBuf[], int nlen);
	/*
	* @breif 删除已经接收的数据包
	* @param chRecBuf接收的数据
	* @param nMsgLen上一个包的长度
	* @param nBufLen 接收的长度（chRecBuf有用的数据）
	*/
	char* deleteMessage(char chRecBuf[], int nMsgLen, int nBufLen);
	/*
	* @breif 拆包
	* @param msgHead 头消息
	* @param pChMsg 待解析的数据
	*/
	void unPack(Message::TagMsgHead *pMsgHead, char* pChMsg);

	/*
	* @breif 添加玩家状态数据到界面 
	*/
	void addPlayerData(TagPlayerStateData& Task);
	/*
	* @breif 设置蛇头方向 
	*/
	void setSnakeDirection(int nPlayerID, int nDirection);
	void setDotPosition(Message::TagDotPosition* pMsg);
	void setDirectionMem(int nDireciton);
private:
	std::vector<TaskMsg* > m_Task;
	cocos2d::Menu* m_pMenuVertical = nullptr;
	cocos2d::Menu* m_pMenuHorizontal = nullptr;
	//游戏状态
	bool m_bState = false;
	SnakeHeadDirection m_SnakeHeadDirection;
	//互斥锁
	std::mutex m_mutexDirection;
	int m_nPlayerNum = 1;
	cocos2d::Menu* m_pMenuRead;
	MakeDraw* m_pDotDraw = nullptr;
};

#endif  //__RetroSnaker_Game_H__