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
	//按钮回调函数
   /**
	* @ brief 向左移动
	*/
	void menuLeftCallBcak(cocos2d::Ref* Spender);
	void scheduleLeft(float at);
	
	/**
	* @ brief 向右移动
	*/
	void menuRightCallBack(cocos2d::Ref* Spender);
	void scheduleRight(float at);
	/**
	* @ brief 向上移动
	*/
	void menuUpperCallBack(cocos2d::Ref* Spender);

	void scheduleUpper(float at);
	/**
	* @ brief 向下移动
	*/
	void menuLowerCallBack(cocos2d::Ref* Spender);

	void scheduleLower(float at);
	/**
	* @brief 取消控制方向定时器
	*/
	void unAllDirection();

	/**
	* @brief 监听蛇头是否 跟金币碰撞
	*/
	void scheduleDot(float at);

private:
	//void SnakeAction(const Snake& snake);

	bool initDirection(const std::string& strDirectionImage);
	/**
	* @brief 初始化圆点
	*/
	bool initDot();

	bool initScore();

	bool addScore();
	/**
	* @brief 添加随机位置金币
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
