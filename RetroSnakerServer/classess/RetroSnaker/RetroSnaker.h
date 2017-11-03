#ifndef __RetroSnaker_H__
#define  __RetroSnaker_H__

#include <string.h>
#include <iostream>
#include <unordered_map>
#include <mutex>
#include "message.h"
#include "D:\\newbie/caiyin/p_caiyin.git/RetroSnakerServer/classess/RetroSnakerSocket/RetroSnakerServer.h"
#include "D:\\newbie/caiyin/p_caiyin.git/RetroSnakerServer/classess/RetroSnaker/GameData/GameData.h"

typedef struct BodyPosition
{
	int nPositionX;
	int nPositionY;
};

typedef struct g_TagPlayerData
{
	int nPlayerID;
	SOCKET sock;
	int nColour;
	int nSnakeDirecotion;
	std::string strPlayerName;
	int nRead;
	std::vector<BodyPosition> vSnakePosition;
	int nRecvSize;
	char chRecvBuf[MSG_PACK_LENG];
};

/*
* @breif 定时回调函数
*/
void CALLBACK onTimer(HWND hwnd, UINT uMsg, UINT IDEvent, DWORD DwTime);
void CALLBACK sendPositionTimer(HWND hwnd, UINT uMsg, UINT IDEvent, DWORD DwTime);

class RetroSnaker : public RetroSnakerServer
{
public:
	static RetroSnaker* GetInstance();
	void initGame();
public:
	/*
	* @breif 拆包回调
	*/
	void unPackCallBack();
	/*
	* @breif  发送蛇的位置给各个玩家 
	*/
	void sendPosition();
	/*
	* @breif 计算蛇的位置 
	* @ return 返回发送数据包的长度
	*/
	int getSnakePosition(char* pSendBuf);

private:

	RetroSnaker() {};
	~RetroSnaker() {};
	/* 
	* @breif 等待连接线程回调函数
	*/
	void acceptCallBack();
	
	/* 
	* @breif 接收消息回调函数
	*/
	void recvCallBack();

	/*
	* @breif 删除已经接收的数据包
	* @param chRecBuf接收的数据
	* @param nMsgLen上一个包的长度
	* @param nBufLen 接收的长度（chRecBuf有用的数据）
	*/
	char* DeleteMessage(char chRecBuf[], int nMsgLen, int nBufLen);

	void timerCallBack();
private:

	void handleTask(Message::TagMsgHead msg, char* pChMsg, int nPlayerID);
	int createPlayerID();
	void initPlayer(SOCKET sock);
	int initColour();
	void responseSignIn(Message::TagPlayerData *pMsg, std::string strPlayerName);
	/*
	* @breif 将socket放入fd_list这个结构体中 
	*/
	void makefd(fd_set *fd_list);
	int getPlayerID(SOCKET sock);
	/*
	* @berif 发送玩家状态 
	*/
	void sendPlayerState(Message::TagSendState* pMsg);
	/*
	* @breif 初始化玩家坐标;
	* @ return 返回发送的数据包的长度
	*/
	int initPlayerPosition(char* pSendBuf);
	/*
	* @breif 创建随机坐标 
	*/
	BodyPosition createPosition();
	
private:

	SOCKET m_sock;
	std::mutex m_mutexPlayerData;
	std::mutex m_mutexSock;
	std::mutex m_mutexRecv;
	std::mutex m_mutexTask;

	std::vector<Message::TagMsgHead*> m_vTask;
	std::unordered_map<int, g_TagPlayerData*> m_MapPlayerData;
	u_int m_nPlayerID = 1001;
	bool m_bInitPlayerPosition = true;
	int m_nClientNum = 0;
	int m_nColour = 0;
	int m_nReadyNum = 0;
	BodyPosition m_DotPos;
};
#endif // __RetroSnaker_H__