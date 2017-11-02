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
	std::vector<BodyPosition> SnakePosition;
	int nRecvSize;
	char chRecvBuf[MSG_PACK_LENG];
};
/*
* @breif ��ʱ�ص�����
*/
void CALLBACK OnTimer(HWND hwnd, UINT uMsg, UINT IDEvent, DWORD DwTime);
void CALLBACK sendPositionTimer(HWND hwnd, UINT uMsg, UINT IDEvent, DWORD DwTime);
class RetroSnaker : public RetroSnakerServer
{
public:
	static RetroSnaker* getInstance();
	void initGame();

public:
	int m_nReadyNum = 0;
public:
	/*
	* @breif ����ص�
	*/
	void unPackCallBack();
	/*
	* @breif  �����ߵ�λ�ø�������� 
	*/
	void sendPosition();
	/*
	* @breif �����ߵ�λ�� 
	* @ return ���ط������ݰ��ĳ���
	*/
	int getSnakePosition(char* sendBuf);

private:

	RetroSnaker(){};
	~RetroSnaker(){};
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

	
	void TaskCallBack();
	void TimerCallBack();
private:

	void HandleTask(Message::TagMsgHead msg, char* pchMsg, int nPlayerID);
	int CreatePlayerID();
	void initPlayer(SOCKET sock);
	int initColour();
	void PlayerSignIn(Message::TagPlayerData *msg, std::string strPlayerName);
	/*
	* @breif ��socket����fd_list����ṹ���� 
	*/
	void makefd(fd_set *fd_list);
	int getPlayerID(SOCKET sock);
	/*
	* @berif �������״̬ 
	*/
	void sendPlayerState(Message::TagSendState* Msg);
	/*
	* @breif ��ʼ���������;
	* @ return ���ط��͵����ݰ��ĳ���
	*/
	int initPlayerPosition(char* sendBuf);
	/*
	* @breif ����������� 
	*/
	BodyPosition CreatePosition();
	
private:
	SOCKET m_sock;
	std::mutex m_mutexPlayerData;
	std::mutex m_mutexSock;
	std::mutex m_mutexRecv;
	std::mutex m_mutexTask;

	std::vector<Message::TagMsgHead*> m_Task;
	u_int m_nPlayerID = 1001;
	std::unordered_map<int, g_TagPlayerData*> m_MapPlayerData;
	bool m_bInitPlayerPosition = true;
	int m_nClientNum = 0;
	int m_nColour = 0;
	BodyPosition m_DotPos;
};
#endif // __RetroSnaker_H__