#ifndef __GameData_H__
#define __GameData_H__
#include <WinSock2.h>
#include <Windows.h>
#pragma comment(lib, "WS2_32.lib")
#include "cocos2d.h"
#include <string>
#include <unordered_map> 
#include "GameData/message.h"
#include "MakeDraw/MakeDraw.h"

typedef struct TagPlayerStateData
{
	int nHead;
	int nColour;
	int nPlayerID;
	int nState;
	int nDirection;
	cocos2d::Label *pLabelName;
	std::string strPlayerName;
	std::vector<MakeDraw* > SnakeBody;
};
typedef struct Colour3b
{
	BYTE r;
	BYTE g;
	BYTE b;
};

class GameData : public cocos2d::Object
{
public:
	static GameData* getInstance();
	bool init();

	/*
	* @breif �������ӳɹ���sock
	* @param sock ���ӳɹ���sock
	*/
	void setSockServer(SOCKET sock);
	SOCKET getSockServer();

	/*
	* @breif ��¼���ID ��������� ���colour
	*/
	void setPlayerData(int nPlayerID, TagPlayerStateData Task);
	TagPlayerStateData* getPlayerData(int nPlayerID);
	void deletePlayerData(int nPlayerID);
	/*
	* @breif ��¼player colour 
	*/
	void setPlayerColour(int nPlayerID, int nPlayerColour);
	int getPlayerColour(int nPlayerID);

	/*
	* @breif ��¼��ǰ���ID��
	*/
	void setPlayerID(int nPlayerID);
	int getPlayerID();

	/* 
	* @breif ��¼SignIn Scene����յ�RecBuf;
	*/
	void setRecBuf(char recBuf[], int nLen);
	char* getRecBuf();
	int getRecvLen();
	void deleteRecvData();
	/*
	* @breif �����ҳ�ʼ��������� 
	*/
	void addPlayerStateTask(TagPlayerStateData);
	bool isPlayerStateTask();
	TagPlayerStateData getPlayerStateTask();
	/*
	* @breif �����λ�õ����� 
	*/
	void setSnakePositionTask(TagSnakePosition tagSnakePosition);
	bool isSnakePositionTask();
	TagSnakePosition* getSnakePositionTask();
public:
	int m_MsgID;
private:
	std::mutex m_PlayerDataMutex;
	
	std::mutex m_PositionMutex;
	std::vector<TagPlayerStateData> m_PlayerState;
	std::unordered_map<int, TagPlayerStateData> m_MapPlayerData;
	std::vector<TagSnakePosition> m_VecSnakePosition;
	std::unordered_map<int, int> m_PlayerColour;
	char m_recBuf[MSG_PACK_LENG];
	int m_nRecvLen ;
	int m_nPlayerID = 0;
	SOCKET m_sockServer;
private:
	GameData(){};
	~GameData(){};
};

#endif //__GameData_H__