#ifndef __GameData_H__
#define __GameData_H__
#include <WinSock2.h>
#pragma comment(lib, "WS2_32.lib")
#include <string>
#include <mutex>
#include <unordered_map> 

#include "D:\\newbie/caiyin/p_caiyin.git/RetroSnakerServer/classess/RetroSnaker/RetroSnaker.h"
#include "D:\\newbie/caiyin/p_caiyin.git/RetroSnakerServer/classess/RetroSnaker/message.h"

class  GameData
{
public:
	static GameData* getInstance();
	bool init();
	/*
	* @breif 检测任务队列是否为空 
	*/
	bool isTask();
	void addTask(Message::TagMsgHead* msg);
	Message::TagMsgHead* getTask();

	void addPlayerData(int nPlayerID,struct g_TagPlayerData *playeData);
	void deletePlayerData(int nPlayerID);
	g_TagPlayerData* getPlayerData(int nPlayerID);
private:
	GameData(){ };
	~GameData(){ };

private:
	std::vector<Message::TagMsgHead*> m_vecTask;
	std::unordered_map<int, g_TagPlayerData*> m_MapPlayerData;
};




#endif //__GameData_H__