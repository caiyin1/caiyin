#ifndef __RetroSnaker_H__
#define  __RetroSnaker_H__

#include <string.h>
#include <iostream>
#include <unordered_map>
#include <mutex>
#include "message.h"
#include "D:\\newbie/caiyin/p_caiyin.git/RetroSnakerServer/classess/RetroSnakerSocket/RetroSnakerServer.h"

struct m_TagPlayerData
{
	SOCKET sock;
	int nColour;
	SnakeHeadDirection SnakeDirection;
	std::string strPlayerName;
	char chRecvBuf[MSG_PACK_LENG];
	int nRecvSize;
};

class RetroSnaker : public RetroSnakerServer
{
public:
	RetroSnaker();
	~RetroSnaker(){};
	void initGame();

public:
	
private:
	/* 
	* @breif 等待连接线程回调函数
	*/
	void AcceptCallBack();
	
	/* 
	* @breif 接收消息回调函数
	*/
	void RecvCallBack();
	/*
	* @breif 拆包回调 
	*/
	void unPackCallBack();

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
	void HandleTask(Message::TagMsgHead *msg);
	int CreatePlayerID(Message::TagMsgHead *msg);
	
private:
	SOCKET m_sock;
	std::mutex m_mutexSock;
	std::mutex m_mutexRecv;
	std::mutex m_mutexTask;
	std::vector<Message::TagMsgHead *> m_Task;
	u_int m_nPlayerID = 1001;
	std::unordered_map<int, m_TagPlayerData* > m_MapPlayerData;
	std::unordered_map<SOCKET, int> m_MapSockPlayerID;
	/*std::unordered_map<SOCKET, m_TagPlayerData* > m_MapPlayerData*/
	/*std::vector<SOCKET> m_VecSock;*/
	int m_nReadyNum = 0;
	fd_set m_fdClientSock;
	int m_nClinetNum = 0;

};

#endif // __RetroSnaker_H__