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
	* @breif ����SignIn Scene�е�clientSock
	*/
	void PassSock();
	
	/*
	* @breif ��ʼ������ť
	*/
	bool initDirection(const std::string& strDirectionImage);
	
	/**
	* @ brief �����ƶ�
	*/
	void menuLeftCallBcak(cocos2d::Ref* Spender);

	/**
	* @ brief �����ƶ�
	*/
	void menuRightCallBack(cocos2d::Ref* Spender);
	
	/**
	* @ brief �����ƶ�
	*/
	void menuUpperCallBack(cocos2d::Ref* Spender);

	/**
	* @ brief �����ƶ�
	*/
	void menuLowerCallBack(cocos2d::Ref* Spender);

	/**
	* @brief ȡ�����Ʒ���ʱ��
	*/
	void unAllDirection();
	/*
	* @brief ����Ƿ���յ��ı䷽����Ϣ
	*/
	void scheduleDirection(float at);
private:

	std::string m_strDirectionImage = "D:\\material\\Direction.png";
	cocos2d::Menu* m_pMenuVertical = nullptr;
	cocos2d::Menu* m_pMenuHorizontal = nullptr;
};