/*
 * =====================================================================================
 *
 *       Filename:  TalkingDataAnalysis.h
 *
 *    Description:  统计分析库头文件
 *					统计分析模块，主要负责游戏的统计分析
 *
 *        Version:  1.0
 *        Created:  02/06/2018 11:21:12
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jason Tou (), sosoayaen@126.com
 *   Organization:  
 *
 * =====================================================================================
 */

#pragma once

#include <string>
#include <unordered_map>

class TalkingDataAnalysis
{
public:
    enum class AccountType
    {
        kType_Anonymous = 0,// 匿名用户，游客
        kType_Registered,   // 自己注册的账号
        kType_SinaWeibo,    // 新浪微博
        kType_QQ,           // QQ
        kType_TencentWeibo, // 腾讯微博
        kType_ND91,         // 网龙91
        kType_1,            // 自定义
        kType_2,            // 自定义
        kType_3,            // 自定义
        kType_4,            // 自定义
        kType_5,            // 自定义
        kType_6,            // 自定义
        kType_7,            // 自定义
        kType_8,            // 自定义
        kType_9,            // 自定义
        kType_10,           // 自定义
    };
    
	// 获取对应的静态对象
	static TalkingDataAnalysis* getInstance()
    {
        if (!_instance)
            _instance = new TalkingDataAnalysis;
        return _instance;
    }

	/**
	 * @brief 初始化
	 */
	void startWithAppKey(const std::string& strAppKey);
    void startWithAppKeyAndChannel(const std::string& strAppKey, const std::string& strChannel);
    
    /**
     * @brief 设置玩家的账号，无账号游戏直接取设备号即可
     */
    void setAccount(const std::string& strAccount);
    void setAccountWithDeviceID();
    
    /**
     * @brief 设置账号类型
     */
    void setAccountType(AccountType eType);
    
    void setAccountName(const std::string& strAccountName);
    void setAccountNameWithDeviceName();
    
    void setLevel(int nLevel);
    
    /**
     * @brief 性别，0 - 未知，1 - 男，2 - 女
     */
    void setGender(int nGender);
    
    void setAge(int nAge);
    
    void setGameServer(const std::string& strServerName);
    
	/**
	 * @brief 自定义事件
	 */
    void event(const std::string& strEventId, const std::unordered_map<std::string, std::string>& eventData);
    void event(const std::string& strEventId, const std::unordered_map<std::string, long>& eventData);

private:
	// AppKey
	std::string m_strAppKey;

	// AppChannel
	std::string m_strChannel;
    
    static TalkingDataAnalysis* _instance;
};
