#ifndef __MESSAGE_H__
#define __MESSAGE_H__
#include "cocos2d.h"
#include <iostream> 
#include <string.h>


#define MAX_POSX 24
#define MAX_POSY 16

#define UPPER 1
#define LOWER 2
#define RIGHT 3
#define LEFT 4

// head message
#define HEAD_SIGN_IN		1
#define HEAD_PERPARE		2
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
	int nMsgId;
	int nLen;
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
	struct MsgHead
	{
		int nMessageLen;
		int nMsgID;
		int nMessageHead;
		char *pChMessage;
	};
};











#endif //__MESSAGE_H__