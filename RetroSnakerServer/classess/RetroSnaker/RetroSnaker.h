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
	* @breif �ȴ������̻߳ص�����
	*/
	void AcceptCallBack();
	
	/* 
	* @breif ������Ϣ�ص�����
	*/
	void RecvCallBack();
	/*
	* @breif ����ص� 
	*/
	void unPackCallBack();

	int getMessageLen(char chRecBuf[], int nHeadLen);
	/*
	* @breif ɾ���Ѿ����յ����ݰ�
	* @param chRecBuf���յ�����
	* @param nMsgLen��һ�����ĳ���
	* @param nBufLen ���յĳ��ȣ�chRecBuf���õ����ݣ�
	*/
	char* DeleteMessage(char chRecBuf[], int nMsgLen, int nBufLen);
private:
	/*
	* @berif ��ʱ���ص�����
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