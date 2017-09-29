#include "cocos2d.h"
#include <string.h>
#include "Snake\\Snake.h"
#include "Classes\\Macro.h"

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

private:
	//void SnakeAction(const Snake& snake);

	bool initDirection(const std::string& strDirectionImage);
	/**
	* @brief ��ʼ��Բ��
	*/
	bool initDot();

	bool initScore();

	bool addScore();
	/**
	* @brief ������λ�ý��
	*/
	void addDot();

	std::string intToString();
private:
	Snake* m_pSnake = nullptr;
	cocos2d::Menu* m_pMenuVertical = nullptr;
	cocos2d::Menu* m_pMenuHorizontal = nullptr;
	Snake* m_pDot = nullptr;

	cocos2d::Sprite* m_pScoreBg = nullptr;
	float fLevel = LEVEL_TWE;
	int m_nScore = 0;
};
