#ifndef __RetroSnaker_H__
#define  __RetroSnaker_H__

#include <string.h>
#include <iostream>
#include <unordered_map>
#include <mutex>
#include "message.h"
#include "D:\\newbie/caiyin/p_caiyin.git/RetroSnakerServer/classess/RetroSnakerSocket/RetroSnakerServer.h"



class RetroSnaker : public RetroSnakerServer
{
public:
	RetroSnaker();
	~RetroSnaker();
	void initGame();
public:
	std::unordered_map<int, SOCKET> m_MapPlayerSock;
	std::vector<SOCKET> m_VecSock;
private:
	/* 
	* @breif 等待连接线程回调函数
	*/
	void AcceptCallBack();
	
	/* 
	* @breif 接收消息回调函数
	*/
	void RecvCallBack();

	int getMessageLen(char chRecBuf[], int nHeadLen);
	/*
	* @breif 删除已经接收的数据包
	* @param chRecBuf接收的数据
	* @param nMsgLen上一个包的长度
	* @param nBufLen 接收的长度（chRecBuf有用的数据）
	*/
	char* DeleteMessage(char chRecBuf[], int nMsgLen, int nBufLen);
private:
	/*
	* @berif 定时器回调函数
	*/
	void TaskCallBack();

private:
	SOCKET m_sock;
	std::mutex m_mutex;
	std::vector<Message::TagMsgHead *> m_Task;
};

#endif // __RetroSnaker_H__