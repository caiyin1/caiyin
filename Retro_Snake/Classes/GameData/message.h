#ifndef __MESSAGE_H__
#define __MESSAGE_H__
#include <iostream> 
#include <string.h>
#include "cocos2d.h"






#define MSG_PACK_LENG 1024
#define MAX_POS 16
#define MY_PORT 6666
#define MAX_POSX 24

#define UPPER 1
#define LOWER 2
#define RIGHT 3
#define LEFT 4

// head message
#define HEAD_INVALID       -1 //����Ϣ
#define HEAD_LOGIN		1
#define HEAD_READ		2
#define HEAD_GAME_SCORE		3
#define HEAD_PLAYER_DATA	4
#define HEAD_SNAKE_POSITION 5
#define HEAD_PLAYER_DIE		6
#define HEAD_GAME_OVER		7
#define HEAD_DIRECTION		8
#define HEAD_HEARTBEAT		9 //������
#define HEAD_COUNTDOWN      10 //����ʱ��Ϣ

#define HEAD_DOT 10
#define HEAD_SNAKE 11
enum SnakeHeadDirection
{
	SnakeHeadUpper,
	SnakeHeadLower,
	SnakeHeadRight,
	SnakeHeadLeft
};


/*
* @berif ���ݰ��ṹ��
* @param nMsgId ͷ��Ϣ��ʶ��
* @param clientSock �ͷ��˵��׽���
* @param bDeath ���Ƿ���ײ
* @param int nDirection ��ʾ��ͷ����
* @param vector<cocos2d::Size> Snake �ߵ�λ��
*
*/
struct ServerMessage
{
	int nMsgId;
	int nLen;
	SOCKET clientSock;
	bool bDeath;
	SnakeHeadDirection nDirection;
	cocos2d::Size DotSize;
	std::vector<cocos2d::Size> Snake;
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

typedef struct TagSnakePosition
{
	// nHead 10 ��ʾ����Dot�����ݣ� 11�������ߵ�����
	int nHead;
	int nPlayerID;
	int nDirection;
	std::vector<cocos2d::Size> SnakePosition;
};

class Message
{
public:
	struct TagMsgHead
	{
		//int nMessageLen;
		int nMsgLeng;
		int nMessageHead;
		int nMsgID;
		//char chName[MSG_PACK_LENG];
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
	struct TagRead : public TagMsgHead
	{
		TagRead() 
		{
			TagMsgHead(HEAD_READ);
		}
		int nPlayerID;
		int nRead;
	};
	struct TagClientDirection : public TagMsgHead
	{
		TagClientDirection()
		{
			TagMsgHead(HEAD_DIRECTION);
		};
		int nPlayerID;
		int nDirection;
	};
	struct TagDotPosition : public TagMsgHead
	{
		int nSnakeNum;
		struct TagDotPos
		{
			int nPositionX;
			int nPositionY;
		} DotPos;
	};
};











#endif //__MESSAGE_H__