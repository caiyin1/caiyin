#ifndef __DATA_MANAGER_H__
#define __DATA_MANAGER_H__
#include "cocos2d.h"
#include <iostream>
#include <vector>
#include <string>
#include "GameDeploy.h"
#include "GameStatusManager.h"
#include "../StructData/BlockData.h"
#include "json/writer.h"
#include "json/document.h"
#include "json/stringbuffer.h"


class DataManager
{
public:
	static DataManager* getInstance();
	void init();
public:
	/*
	* @brief 检测是否有游戏
	*/
	bool isGameRecord();
	const rapidjson::Document& getJsonDoc();

public:
	/*
	* @brief 存档
	*/
	void saveRecord(const rapidjson::Value& savejson);
	/*
	* @brief 读取本地文件
	* @retrun 本地有文件返回 true，反之返回false
	*/
	bool readRecord();
	/*
	* @brief 加密数据
	*/
	std::string encryptData(const char* pBuff, int nBufSize);
	/*
	* @brief 解密数据,(调用是需要外部释放返回值delete)
	*/
	std::string decryptData(unsigned char* pChBuf, int nBufLen);
	/*
	* @brief 删除记录
	*/
	void removeGameRecord();
public:
	/*
	* @brief 设置游戏结束回调的函数
	*/
	void setGameOverCallBack(const std::function<void()>& fun);
public:
	/*
	* @brief 安全修改游戏内存数据
	* @param nNum 需要安全修改的值
	* @param strKey 改值装换MD5的可以
	* @param nChangeNum 该值的变化量
	* @param b 是否要安全检测， true 检测， false不检测
	*/
	void safeModifyData(int& nNum, const std::string& strKey, int nChangeNum, bool bCheck = true);
	/*
	* @brief 安全设置历史最高分
	*/
	void safeModifyHighScore(int& nNum, const std::string& strKey, int nChangeNum, bool bCheck = true);
private:
	// json格式对象
	rapidjson::Document m_readDoc;
	// 记录内存数据MD5的值
	std::unordered_map<std::string, std::string> m_mGameBufferMD5;
private:
	std::function<void()> m_pGameVoerCallBack = nullptr;
};
#endif  // __DATA_MANAGER_H__
