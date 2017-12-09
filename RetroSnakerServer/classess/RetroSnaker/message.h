#ifndef __MESSAGE_H__
#define __MESSAGE_H__
#include <iostream> 
#include <string.h>
#include <WinSock2.h>
#include "RetroSnaker.h"








#define MSG_PACK_LENG 1024
#define MAX_POSX 24
#define MAX_POSY 16
#define MY_PORT 6666
/* 格子大小*/
#define LATTICE_SIZE 20
// 格式的个数
#define LATTCE_NUM 16
/*最少开始人数*/
#define PLAYER_NUM 4

#define UPPER 1
#define LOWER 2
#define RIGHT 3
#define LEFT 4

// head message
#define HEAD_INVALID       -1 //无消息
#define HEAD_LOGIN		1
#define HEAD_READY		2
#define HEAD_GAME_SCORE		3
#define HEAD_PLAYER_DATA	4
#define HEAD_SNAKE_POSITION 5
#define HEAD_PLAYER_DIE		6
#define HEAD_GAME_OVER		7
#define HEAD_DIRECTION		8
#define HEAD_HEARTBEAT		9 //心跳包
#define HEAD_COUNTDOWN      10 //倒计时消息


enum SnakeHeadDirection
{
	SnakeHeadUpper,
	SnakeHeadLower,
	SnakeHeadRight,
	SnakeHeadLeft
};

enum enumGameState
{
	//游戏中
	inGameState,
	//准备阶段
	readyState
};



/*
* @berif 数据包结构体
* @param nMsgId 头消息标识符
* @param clientSock 客服端的套接字
* @param int nDirection 表示蛇头的转向
*/
struct ClientMessage
{
	int nLen;
	int nMsgId;
	SOCKET clientSock;
	int nDirection;

};
struct TaskMsg
{
	int nMsgHead;
	char* pChMeg;
};

// typedef struct TagSnakePosition 
// {
// 	/*蛇的长度*/
// 	int nPlayerLeng;
// 	int nPlayerID;
// 	int nDirection;
// 	std::vector<struct BodyPosition> snakePos;
// };


class Message
{
public:
	struct TagMsgHead
	{
		int nMsgLeng;
		int nMessageHead;
		int nMsgID;
		TagMsgHead(int msg = HEAD_INVALID)
		{
			nMessageHead = msg;
		}
	};
	struct TagPlayerData : public  TagMsgHead
	{
		TagPlayerData()
		{
			TagMsgHead(HEAD_LOGIN);
		}
		int nPlayerID;
		int nColour;
		int nState;
		char chStart;
	};
	struct TagSnakeHeadDirection : public TagMsgHead
	{
		TagSnakeHeadDirection()
		{
			TagMsgHead(HEAD_DIRECTION);
		};
		int nPlayerID;
		int nDirection;
	};
	struct TagSendState : public TagMsgHead
	{
		TagSendState()
		{
			TagMsgHead(HEAD_READY);
		}
		int nPlayerID;
		int nState;
	};

	struct TagSignIn
	{
		int nMessageLen;
		int nMsgID;
		int nMessageHead;
		int nPlayerID;
		int nColour;
		std::string strPlayerName;
	};
};











#endif //__MESSAGE_H__