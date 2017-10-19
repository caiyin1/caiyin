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
	//这个没有用的
	static GameData* shareGlobal();
	static GameData* getInstance();
	bool init();

	/*
	* @breif 储存连接成功的sock
	* @param sock 连接成功的sock
	*/
	void setSockServer(SOCKET sock);
	SOCKET getSockServer();

	/*
	* @breif 记录玩家ID 和玩家名字
	*/
	void setPlayerName(int nPlayerID, const std::string& strPlayerName);
	std::string getPlayerName(int nPlayerID);
	
	/*
	* @breif 记录player colour 
	*/
	void setPlayerColour(int nPlayerID, int nPlayerColour);
	int getPlayerColour(int nPlayerID);

	/*
	* @breif 记录当前玩家ID；
	*/
	void setPlayerID(int nPlayerID);
	int getPlayerID();

	/* 
	* @breif 记录SignIn Scene多接收的RecBuf;
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