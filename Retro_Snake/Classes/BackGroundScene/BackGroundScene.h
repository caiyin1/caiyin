#include "cocos2d.h"
#include "GameData\\message.h"


DWORD WINAPI ThreadRecv(LPVOID param);


class BackGroundScene : public cocos2d::Scene
{
public:
	
	static cocos2d::Scene* createScene();
	bool init();
	CREATE_FUNC(BackGroundScene);
	void schedulexxx(float at);

public:
	
	SOCKET m_clientSock;
	DWORD m_nThreadRecv;
	SnakeHeadDirection m_CurrentSnakeHeadDirection;
	SnakeHeadDirection m_RecvSnakeHeadDirection;

private:
	
	bool AccpetServer();
	/*
	* @breif 接收SignIn Scene中的clientSock
	*/
	void PassSock();
	
	/*
	* @breif 初始化方向按钮
	*/
	bool initDirection(const std::string& strDirectionImage);
	
	/**
	* @ brief 向左移动
	*/
	void menuLeftCallBcak(cocos2d::Ref* Spender);

	/**
	* @ brief 向右移动
	*/
	void menuRightCallBack(cocos2d::Ref* Spender);
	
	/**
	* @ brief 向上移动
	*/
	void menuUpperCallBack(cocos2d::Ref* Spender);

	/**
	* @ brief 向下移动
	*/
	void menuLowerCallBack(cocos2d::Ref* Spender);

	/**
	* @brief 取消控制方向定时器
	*/
	void unAllDirection();
	/*
	* @brief 检测是否接收到改变方向信息
	*/
	void scheduleDirection(float at);
private:

	std::string m_strDirectionImage = "D:\\material\\Direction.png";
	cocos2d::Menu* m_pMenuVertical = nullptr;
	cocos2d::Menu* m_pMenuHorizontal = nullptr;
};