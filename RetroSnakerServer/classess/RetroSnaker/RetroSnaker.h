#ifndef __RetroSnaker_H__
#define  __RetroSnaker_H__

#include <string.h>
#include <iostream>
#include <unordered_map>
#include <mutex>
#include "message.h"
#include "D:\\caiyin.git/RetroSnakerServer/classess/RetroSnakerSocket/RetroSnakerServer.h"
#include "GameData/GameData.h"

typedef struct BodyPosition
{
	int nPositionX;
	int nPositionY;
};

typedef struct Color4B
{
	unsigned char _r;
	unsigned char _g;
	unsigned char _b;
	unsigned char _a;
	Color4B(unsigned char r, unsigned char g, unsigned char b): _r(r), _g(g), _b(b), _a(255){};
};

typedef struct g_TagPlayerData
{
	int nPlayerID;
	SOCKET sock;
	int nColour;
	int nSnakeDirecotion;
	std::string strPlayerName;
	// -1 未准备， 1 准备， 0 死亡(属于观战者模式)， 2 活着； 
	int nPlayerState;
	std::vector<BodyPosition> vSnakePosition;
	int nRecvSize;
	//在线判断
	int nHeartBeat;
	char chRecvBuf[MSG_PACK_LENG];
};

/*
* @breif 定时回调函数
*/
void CALLBACK onTimer(HWND hwnd, UINT uMsg, UINT IDEvent, DWORD DwTime);
void CALLBACK gameLogicTimer(HWND hwnd, UINT uMsg, UINT IDEvent, DWORD DwTime);
void CALLBACK isGameOver(HWND hwnd, UINT uMsg, UINT IDEvent, DWORD DwTime);
void CALLBACK handleHeartBeatTimer(HWND hwnd, UINT uMsg, UINT IDEvent, DWORD DwTime);
void CALLBACK gameStartTimer(HWND hwnd, UINT uMsg, UINT IDEvent, DWORD DwTime);

class RetroSnaker : public RetroSnakerServer
{
public:
	static RetroSnaker* GetInstance();
	void initGame();
public:
	/*
	* @breif 拆包回调
	*/
	void handlRecvCallBack();
	/*
	* @breif  发送蛇的位置给各个玩家 
	*/
	void handlGameLogic();
	/*
	* @breif 计算蛇的位置 
	* @ return 返回发送数据包的长度
	*/
	int setSnakePositionPack(char* pSendBuf);
	/*
	* @breif 游戏结束逻辑的处理
	*/
	void gameOverLogic();
	/*
	* @breif 处理心跳包的逻辑 
	*/
	void heartLogic();
	/*
	* @breif 判断游戏是否有开始的条件 
	*/
	void startJudge();

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
	void initColor();
	int createColour();
	void responseSignIn(Message::TagPlayerData *pMsg, std::string strPlayerName);
	/*
	* @breif 将socket放入fd_list这个结构体中 
	*/
	void makefd(fd_set *fd_list);
	int getPlayerID(SOCKET sock);
	/*
	* @berif 推广发送玩家状态 
	*/
	void sendPlayerState(Message::TagSendState* pMsg);
	/*
	* @breif 应答 迭代发送已经准备的玩家给该玩家
	* @param 申请准备的玩家ID,
	*/
	void answerReadyPlayer(int nPlayerID);
	/*
	* @breif 初始化玩家坐标;
	* @ return 返回发送的数据包的长度
	*/
	int initPlayerPosData(char* pSendBuf);
	/*
	* @breif 创建随机坐标 
	*/
	BodyPosition createPosition();
	/*
	* @breif设置蛇的运动位置 
	*/
	void setGameData();
	/*
	* @breif 碰撞检测
	* @param nPlayerID 玩家ID
	* return false 为碰撞， true为碰撞
	*/
	bool isDotImpack(int nPlayerID);
	/*
	* @breif 迭代所有玩家与Dot碰撞检测
	* return false 为碰撞， true为碰撞
	*/
	bool isDotImpack();
	/*
	* @breif 判断蛇的死亡碰撞
	* @return 2 代表玩家活着， 0代表玩家死亡；
	*/
	int isSnakeImpack(int nPlayerID);
	/*
	* @ breif 发送玩家成绩给客户端 
	*/
	void sendPlayerScore();
	/*
	* @breif 游戏结束重新初始化游戏的数据 
	*/
	void againInitGameData();

	/*
	* @breif 初始化游戏的基础数据 
	*/
	void initGameData();
	/*
	* @breif 获取本机IP 
	*/
	void getIP();
	
private:

	SOCKET m_sock;
	std::mutex m_mutexPlayerData;
	//目前没函数 调用这个锁
	std::mutex m_mutexSock;

	std::vector<Message::TagMsgHead*> m_vTask;
	std::unordered_map<int, g_TagPlayerData*> m_MapPlayerData;
	u_int m_nPlayerID;
	bool m_bInitPlayerPosition;
	int m_nClientNum;
	int m_nColour;
	int m_nReadyNum;
	//参加游戏人数
	int m_nGameNum;
	BodyPosition m_DotPos;
	/*
	* 定时器句柄
	*/
	UINT m_timerid;
	UINT m_gameLogicTimerId;
	UINT m_gameOverTimerId;
	//游戏开始判断定时器句柄
	UINT m_gameStartTimerId;
	//游戏状态
	enumGameState m_gameState;
	//游戏开始的倒计时值 10秒倒计时
	int m_nCountDown;


};
#endif // __RetroSnaker_H__