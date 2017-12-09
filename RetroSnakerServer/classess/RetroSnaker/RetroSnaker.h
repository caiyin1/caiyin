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
	// -1 δ׼���� 1 ׼���� 0 ����(���ڹ�ս��ģʽ)�� 2 ���ţ� 
	int nPlayerState;
	std::vector<BodyPosition> vSnakePosition;
	int nRecvSize;
	//�����ж�
	int nHeartBeat;
	char chRecvBuf[MSG_PACK_LENG];
};

/*
* @breif ��ʱ�ص�����
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
	* @breif ����ص�
	*/
	void handlRecvCallBack();
	/*
	* @breif  �����ߵ�λ�ø�������� 
	*/
	void handlGameLogic();
	/*
	* @breif �����ߵ�λ�� 
	* @ return ���ط������ݰ��ĳ���
	*/
	int setSnakePositionPack(char* pSendBuf);
	/*
	* @breif ��Ϸ�����߼��Ĵ���
	*/
	void gameOverLogic();
	/*
	* @breif �������������߼� 
	*/
	void heartLogic();
	/*
	* @breif �ж���Ϸ�Ƿ��п�ʼ������ 
	*/
	void startJudge();

private:

	RetroSnaker() {};
	~RetroSnaker() {};
	/* 
	* @breif �ȴ������̻߳ص�����
	*/
	void acceptCallBack();
	
	/* 
	* @breif ������Ϣ�ص�����
	*/
	void recvCallBack();

	/*
	* @breif ɾ���Ѿ����յ����ݰ�
	* @param chRecBuf���յ�����
	* @param nMsgLen��һ�����ĳ���
	* @param nBufLen ���յĳ��ȣ�chRecBuf���õ����ݣ�
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
	* @breif ��socket����fd_list����ṹ���� 
	*/
	void makefd(fd_set *fd_list);
	int getPlayerID(SOCKET sock);
	/*
	* @berif �ƹ㷢�����״̬ 
	*/
	void sendPlayerState(Message::TagSendState* pMsg);
	/*
	* @breif Ӧ�� ���������Ѿ�׼������Ҹ������
	* @param ����׼�������ID,
	*/
	void answerReadyPlayer(int nPlayerID);
	/*
	* @breif ��ʼ���������;
	* @ return ���ط��͵����ݰ��ĳ���
	*/
	int initPlayerPosData(char* pSendBuf);
	/*
	* @breif ����������� 
	*/
	BodyPosition createPosition();
	/*
	* @breif�����ߵ��˶�λ�� 
	*/
	void setGameData();
	/*
	* @breif ��ײ���
	* @param nPlayerID ���ID
	* return false Ϊ��ײ�� trueΪ��ײ
	*/
	bool isDotImpack(int nPlayerID);
	/*
	* @breif �������������Dot��ײ���
	* return false Ϊ��ײ�� trueΪ��ײ
	*/
	bool isDotImpack();
	/*
	* @breif �ж��ߵ�������ײ
	* @return 2 ������һ��ţ� 0�������������
	*/
	int isSnakeImpack(int nPlayerID);
	/*
	* @ breif ������ҳɼ����ͻ��� 
	*/
	void sendPlayerScore();
	/*
	* @breif ��Ϸ�������³�ʼ����Ϸ������ 
	*/
	void againInitGameData();

	/*
	* @breif ��ʼ����Ϸ�Ļ������� 
	*/
	void initGameData();
	/*
	* @breif ��ȡ����IP 
	*/
	void getIP();
	
private:

	SOCKET m_sock;
	std::mutex m_mutexPlayerData;
	//Ŀǰû���� ���������
	std::mutex m_mutexSock;

	std::vector<Message::TagMsgHead*> m_vTask;
	std::unordered_map<int, g_TagPlayerData*> m_MapPlayerData;
	u_int m_nPlayerID;
	bool m_bInitPlayerPosition;
	int m_nClientNum;
	int m_nColour;
	int m_nReadyNum;
	//�μ���Ϸ����
	int m_nGameNum;
	BodyPosition m_DotPos;
	/*
	* ��ʱ�����
	*/
	UINT m_timerid;
	UINT m_gameLogicTimerId;
	UINT m_gameOverTimerId;
	//��Ϸ��ʼ�ж϶�ʱ�����
	UINT m_gameStartTimerId;
	//��Ϸ״̬
	enumGameState m_gameState;
	//��Ϸ��ʼ�ĵ���ʱֵ 10�뵹��ʱ
	int m_nCountDown;


};
#endif // __RetroSnaker_H__