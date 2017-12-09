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
/* ���Ӵ�С*/
#define LATTICE_SIZE 20
// ��ʽ�ĸ���
#define LATTCE_NUM 16
/*���ٿ�ʼ����*/
#define PLAYER_NUM 4

#define UPPER 1
#define LOWER 2
#define RIGHT 3
#define LEFT 4

// head message
#define HEAD_INVALID       -1 //����Ϣ
#define HEAD_LOGIN		1
#define HEAD_READY		2
#define HEAD_GAME_SCORE		3
#define HEAD_PLAYER_DATA	4
#define HEAD_SNAKE_POSITION 5
#define HEAD_PLAYER_DIE		6
#define HEAD_GAME_OVER		7
#define HEAD_DIRECTION		8
#define HEAD_HEARTBEAT		9 //������
#define HEAD_COUNTDOWN      10 //����ʱ��Ϣ


enum SnakeHeadDirection
{
	SnakeHeadUpper,
	SnakeHeadLower,
	SnakeHeadRight,
	SnakeHeadLeft
};

enum enumGameState
{
	//��Ϸ��
	inGameState,
	//׼���׶�
	readyState
};



/*
* @berif ���ݰ��ṹ��
* @param nMsgId ͷ��Ϣ��ʶ��
* @param clientSock �ͷ��˵��׽���
* @param int nDirection ��ʾ��ͷ��ת��
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
// 	/*�ߵĳ���*/
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