#ifndef __MESSAGE_H__
#define __MESSAGE_H__
#include <iostream> 
#include <string.h>
#include <WinSock2.h>







#define MSG_PACK_LENG 1024
#define MAX_POSX 24
#define MAX_POSY 16
#define MY_PORT 6000

#define UPPER 1
#define LOWER 2
#define RIGHT 3
#define LEFT 4

// head message
#define HEAD_SIGN_IN		1
#define HEAD_READY		2
#define HEAD_GAME_DATA		3
#define HEAD_PLAYER_DATA	4
#define HEAD_SNAKE_POSITION 5
#define HEAD_PLAYER_DIE		6
#define HEAD_GAME_OVER		7
#define HEAD_DIRECTION		8


enum SnakeHeadDirection
{
	SnakeHeadUpper,
	SnakeHeadLower,
	SnakeHeadRight,
	SnakeHeadLeft
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


class Message
{
public:
	struct TagMsgHead
	{
		int nMessageLen;
		int nMessageHead;
		int nMsgID;
		char *pChMessage;
	};
	struct TagPlayerData
	{
		int nPlayerID;
		int nColour;
		std::string strPlayerName;
	};
	struct TagClientMsg
	{
		

	};
};











#endif //__MESSAGE_H__