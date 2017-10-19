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
	* @breif �ȴ������̻߳ص�����
	*/
	void AcceptCallBack();
	
	/* 
	* @breif ������Ϣ�ص�����
	*/
	void RecvCallBack();

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

private:
	SOCKET m_sock;
	std::mutex m_mutex;
	std::vector<Message::TagMsgHead *> m_Task;
};

#endif // __RetroSnaker_H__