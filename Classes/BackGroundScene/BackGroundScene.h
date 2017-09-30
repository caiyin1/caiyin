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
	/**
	* @brief 定时扫描 蛇头是否与蛇身触碰
	*/
	void scheduleSnakeCollision(float at);

private:
	//void SnakeAction(const Snake& snake);
	/**
	* @brief 初始化方向按钮控件
	* @parem strDirectionImage 按钮图片路径
	*/
	bool initDirection(const std::string& strDirectionImage);
	/**
	* @brief 初始化圆点
	*/
	bool initDot();
   /**
	* @brief 初始化得分的label
	*/
	bool initScore();
	/**
	* @brief 添加所得的分数
	*/
	bool addScore();
	/**
	* @brief 添加随机位置金币
	*/
	void addDot();

	std::string intToString();
	/**
	* @brief 添加蛇身
	*/
	void addSnakeBody();
	/**
	* @brief 蛇身跟着 蛇头运动
	*/
	void ActionSnakeBody();
	/**
	* @brief 检查蛇头是否接触墙壁
	*/
	bool isSnakeHeadBorder();
	/**
	* @brief 检测蛇身是否与蛇头或者Dot重叠
	*/
	bool isSnakeBodyOverlap(Snake* snake);
	/**
	* @brief 检测Dot是否很方向控件重叠
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
	// 蛇头方向
	SnakeHeadDirection m_SnakeHeadDirection;

	float m_fTime = 0.4;
};
