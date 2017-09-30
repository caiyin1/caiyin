#include "cocos2d.h"
#include <string.h>
#include "Snake\\Snake.h"
#include "Classes\\Macro.h"


enum SnakeHeadDirection
{
	SnakeHeadUpper,
	SnakeHeadLower,
	SnakeHeadRight,
	SnakeHeadLeft
};




class BackGroundScene :  public cocos2d::Scene
//	DrawNode
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	
private:
	//��ť�ص�����
   /**
	* @ brief �����ƶ�
	*/
	void menuLeftCallBcak(cocos2d::Ref* Spender);
	void scheduleLeft(float at);
	
	/**
	* @ brief �����ƶ�
	*/
	void menuRightCallBack(cocos2d::Ref* Spender);
	void scheduleRight(float at);
	/**
	* @ brief �����ƶ�
	*/
	void menuUpperCallBack(cocos2d::Ref* Spender);

	void scheduleUpper(float at);
	/**
	* @ brief �����ƶ�
	*/
	void menuLowerCallBack(cocos2d::Ref* Spender);

	void scheduleLower(float at);
	/**
	* @brief ȡ�����Ʒ���ʱ��
	*/
	void unAllDirection();

	/**
	* @brief ������ͷ�Ƿ� �������ײ
	*/
	void scheduleDot(float at);
	/**
	* @brief ��ʱɨ�� ��ͷ�Ƿ���������
	*/
	void scheduleSnakeCollision(float at);

private:
	//void SnakeAction(const Snake& snake);
	/**
	* @brief ��ʼ������ť�ؼ�
	* @parem strDirectionImage ��ťͼƬ·��
	*/
	bool initDirection(const std::string& strDirectionImage);
	/**
	* @brief ��ʼ��Բ��
	*/
	bool initDot();
   /**
	* @brief ��ʼ���÷ֵ�label
	*/
	bool initScore();
	/**
	* @brief ������õķ���
	*/
	bool addScore();
	/**
	* @brief ������λ�ý��
	*/
	void addDot();

	std::string intToString();
	/**
	* @brief �������
	*/
	void addSnakeBody();
	/**
	* @brief ������� ��ͷ�˶�
	*/
	void ActionSnakeBody();
	/**
	* @brief �����ͷ�Ƿ�Ӵ�ǽ��
	*/
	bool isSnakeHeadBorder();
	/**
	* @brief ��������Ƿ�����ͷ����Dot�ص�
	*/
	bool isSnakeBodyOverlap(Snake* snake);
	/**
	* @brief ���Dot�Ƿ�ܷ���ؼ��ص�
	*/
	bool isDotOverlop();
private:
	Snake* m_pSnakeHead = nullptr;
	Snake* m_pSnakeLast = nullptr;
	cocos2d::Menu* m_pMenuVertical = nullptr;
	cocos2d::Menu* m_pMenuHorizontal = nullptr;
	Snake* m_pDot = nullptr;

	cocos2d::Sprite* m_pScoreBg = nullptr;
	float fLevel = LEVEL_THREE;
	int m_nScore = 0;
	// ��ͷ����
	SnakeHeadDirection m_SnakeHeadDirection;

	float m_fTime = 0.4;
};
