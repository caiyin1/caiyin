#ifndef __GameData_H__
#define __GameData_H__
#include <WinSock2.h>
#pragma comment(lib, "WS2_32.lib")
#include "cocos2d.h"
#include <string>
#include <unordered_map> 
#include "GameData/message.h"

class GameData : public cocos2d::Object
{
public:
	//���û���õ�
	static GameData* shareGlobal();
	static GameData* getInstance();
	bool init();

	/*
	* @breif �������ӳɹ���sock
	* @param sock ���ӳɹ���sock
	*/
	void setSockServer(SOCKET sock);
	SOCKET getSockServer();

	/*
	* @breif ��¼���ID ���������
	*/
	void setPlayerName(int nPlayerID, const std::string& strPlayerName);
	std::string getPlayerName(int nPlayerID);
	
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

public:
	int m_MsgID;

private:
	std::unordered_map<int, std::string> m_PlayerName;
	std::unordered_map<int, int> m_PlayerColour;
	char m_recBuf[MSG_PACK_LENG];
	int m_nPlayerID;
	SOCKET m_sockServer;
private:
	GameData(){};
	~GameData(){};
};

#endif //__GameData_H__