#ifndef __GameData_H__
#define __GameData_H__
#include <WinSock2.h>
#pragma comment(lib, "WS2_32.lib")
#include "cocos2d.h"
#include <string>
#include <unordered_map> 

class GameData : public cocos2d::Object
{
public:
	
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
	void setPlayerData(int nPlayerID, const std::string& strPlayerName);
	std::string getPlayerName(int nPlayerID);
	
	/*
	* @breif 记录当前玩家ID；
	*/
	void setPlayerID(int nPlayerID);
	
	int getPlayerID();
public:
	int m_MsgID;
	

	std::unordered_map<int, std::string> m_PlayerData;

private:

	int m_nPlayerID;
	SOCKET m_sockServer;
	
private:
	GameData(){};
	~GameData(){};
};

#endif //__GameData_H__