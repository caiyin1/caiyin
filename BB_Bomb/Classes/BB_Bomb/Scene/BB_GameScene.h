#ifndef __BB_GAME_SCENE_H__
#define __BB_GAME_SCENE_H__
#include "cocos2d.h"
#include "../UI/FortNode.h"

// 游戏开始事件名  
#define START_GAME_EVENT  "StartGameEvent" 
// 添加子弹事件名称
#define ADD_BOMB_EVENT "add bomb"
// 屏幕下边界系数
#define SCREEN_BUTTOM_COFFICIENT 0.1f
// 屏幕上边界系数
#define  SCREEN_TOP_COFFICIENT 0.9f

// 字体
#if defined(CC_PLATFORM_IOS) || defined(CC_PLATFORM_MAC)
#   define FNT_NAME "Helvetica Bold"
#else
#   define FNT_NAME "Arial"
#endif


class BB_GameScene : cocos2d::Scene
{
public:
	static Scene* create();
	virtual bool init()override;
public:
	virtual void onEnter() override;

	virtual void onExit() override;

	virtual void update(float dt) override;
private:
	/*
	* @brief 触屏按下触发事件
	*/
	bool onTouchBegin(cocos2d::Touch* touch, cocos2d::Event* ev);
	/*
	* @brief 触屏滑动事件
	*/
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* ev);
	/*
	* @brief 释放事件
	*/
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* ev);
	/*
	* @brief 创建触屏监听事件
	*/
	void createTouchListener();
	/*
	* @brief 游戏开始事件消息处理函数
	*/
	void onStartGameEvent(cocos2d::EventCustom* event);
	/*
	* @brief 游戏开始按钮回调
	*/
	void menuStartCallBack(cocos2d::Ref* spender);
	/*
	* @brief 返回主界面回调函数
	*/
	void menuHomeCallBack(cocos2d::Ref* spender);
	/*
	* @brief 回收飞行中的子弹
	*/
	void menuRecyclingBombCallBack(cocos2d::Ref* spender);
private:
	/*
	* @brief 游戏开始事件调用 初始话游戏数据
	*/
	void initGameData();
	/*
	* @brief 游戏开始事件回调 初始化游戏Layer
	*/
	void initGameLayer();
	/*
	* @brief 创建回收子弹的按钮， 在初始化游戏界面的时候调用（initGameLayer()）
	*/
	void createRecyclingBombButton();
	/*
	* @brief 设置按钮的是否开启
	*/
	void setRecyclingBombEnable(const bool& b);
	/*
	* @brief 创建物理世界的4条边界
	*/
	void createBroundLine();
	/*
	* @brief 碰撞回调函数
	*/
	bool onContactBegin(cocos2d::PhysicsContact& contac);
	/*
	* @brief 创建背景回调函数
	*/
	void addBackgroundCallBack(cocos2d::Node* pNode);
private:
	/*
	* @brief 初始化首页的Layer
	*/
	void initHomeLayer();
	/*
	* @brief 创建游戏提示sprite
	*/
	cocos2d::Sprite* createPrompt(const std::string& strFileImage);

	/* update 里面的逻辑函数*/
private:
	/*
	* @brief 处理子弹的发射
	*/
	void handleShootBomb();
	/*
	* @brief 处理Block的生成
	*/
	void handleAddBackground();
	/*
	* @brief 处理屏幕滚动
	*/
	void handleRollScreen();
	/*
	* @brief 处理回合结束的子弹添加
	*/
	void handleAddBomb();
	/*
	* @brief 游戏结束调用函数
	*/
	void handleGameOver();
	/*
	* @brief 清除刚体
	*/
	void clearPhysicsBody();
private:
	/*
	* @brief 移动炮台
	* @param fX炮台X轴要移动到的位置
	*/
	void moveFort(const float fX);
private:
	// 首页Layer	
	cocos2d::Layer* m_pHomeLayer = nullptr;
	// 游戏Layer
	cocos2d::Layer* m_pGameLayer = nullptr;
	// 添加子弹的Node
	cocos2d::Node* m_pBombNode = nullptr;
	// 添加Block的Node
	cocos2d::Node* m_pBlockNode = nullptr;
	// 添加食物的Node
	cocos2d::Node* m_pFoodNode = nullptr;
	// 得分的Label
	cocos2d::Label* m_pScoreLabel = nullptr;
	// 回收子弹的按钮
	cocos2d::MenuItemImage* m_pRecyclingButton = nullptr;
	// 游戏结算Layer
	cocos2d::Layer* m_pGameOverLayer = nullptr;
	// 触摸监听器
	cocos2d::EventListenerTouchOneByOne* m_pEventListenerTouch = nullptr;
private:
	// 触屏点击的X轴位置
	float m_fPosX;
	// 玩家得分
	int m_nScore;
	// 炮台指针
	FortNode* m_pFortNode = nullptr;
	// 运动状态的子弹
	int m_nMoveBombNum;
	// 子弹发射间隔积累的时间
	int m_nTimer;
	// 已经发送的子弹个数
	int m_nAlreadyShootBombNum;
	// 判断是否要移动炮台
	bool m_bMoveFort;
};


#endif // !__BB_GAME_SCENE_H__
