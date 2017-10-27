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
	* @breif 储存连接成功的sock
	* @param sock 连接成功的sock
	*/
	void setSockServer(SOCKET sock);
	SOCKET getSockServer();

	/*
	* @breif 记录玩家ID 和玩家名字 玩家colour
	*/
	void setPlayerData(int nPlayerID, TagPlayerStateData Task);
	TagPlayerStateData* getPlayerData(int nPlayerID);
	void deletePlayerData(int nPlayerID);
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
	int getRecvLen();
	void deleteRecvData();
	/*
	* @breif 添加玩家初始化任务队列 
	*/
	void addPlayerStateTask(TagPlayerStateData);
	bool isPlayerStateTask();
	TagPlayerStateData getPlayerStateTask();
	/*
	* @breif 添加蛇位置的任务 
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