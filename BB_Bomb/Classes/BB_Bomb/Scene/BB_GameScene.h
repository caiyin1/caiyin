#ifndef __BB_GAME_SCENE_H__
#define __BB_GAME_SCENE_H__
#include "cocos2d.h"
#include "../UI/FortNode.h"
#include "../UI/BombNode.h"
#include "../../bailinUtils/DialogLayer.h"
#include "../Manager/GameStatusManager.h"
#include "json/writer.h"
#include "json/document.h"
#include "json/stringbuffer.h"

// 游戏开始事件名  
#define START_GAME_EVENT  "StartGameEvent" 
// 添加子弹事件名称
#define ADD_BOMB_EVENT "add bomb"
// 屏幕下边界系数
#define SCREEN_BUTTOM_COFFICIENT 0.0968f
// 屏幕上边界系数
#define  SCREEN_TOP_COFFICIENT 0.916f

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
	/**
	 *  @brief 取消事件
	 */
	void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* ev);
	/*
	* @brief 创建触屏监听事件
	*/
	void createTouchListener();
	/*
	* @brief 游戏开始事件消息处理函数
	*/
	void onStartGameEvent(cocos2d::EventCustom* event);
	/*
	* @brief 回收飞行中的子弹
	*/
	void menuRecyclingBombCallBack(cocos2d::Ref* spender);
	/*
	* @brief 动画结束回调隐藏子弹,
	*/
	void hideBombCallBack(cocos2d::Ref* spender, BombNode* pBombNode);
	/*
	* @brief 回调改变游戏状态
	*/
	void setGameStatusCallBack(cocos2d::Ref* spender, const GameStatusManager::GameStatus& eStatus);
	/*
	* @brief 回调保存游戏
	*/
	void onSaveGameScheduleCallBack(cocos2d::Ref* spender);
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
	* @brief 对玩家分数Label进行缩放
	*/
	void handleScaleScoreLabel();
	/*
	* @brief 对玩家子弹Layel进行缩放
	*/
	void handleScaleBombSizeLabel();
	/*
	* @brief 创建背景回调函数
	*/
	void addBackgroundCallBack(cocos2d::Node* pNode);
	/*
	* @brief 添加本回合得分
	*/
	void addRoundScore(int nNum, bool bInitial = false);
private:
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
	* @brief 获取炮口在主界面的位置, 在handleShootBomb()中调用
	*/
	const cocos2d::Vec2& getFortTopPos();
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
	* @brief 处理黑洞Block
	*/
	void handleBlackHoleBlack();
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
	/*
	* @brief 显示复盘的游戏内容
	*/
	void showGameRecordScreen();
	/*
	* @brief 添加复盘界面的块块
	*/
	void addRecordedBlockToScreen(const rapidjson::Value& vBlockDataValue);
	/*
	* @brief 添加食物到界面上
	*/
	void addRecordedFoodToScreen(const rapidjson::Value& vFoodDataValue);
	/*
	* @breif 把飞行中的子弹添加到界面上
	*/
	void addRecordedFlyBombToScreen(const rapidjson::Value& vFlyBombDataValue);
	/*
	* @brief 把数据保存成json文件格式
	*/
	void dataChangeJson();

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
	// 添加黑洞Block的Node
	cocos2d::Node* m_pBlackHoleNode = nullptr;
	// 得分的Label
	cocos2d::Label* m_pScoreLabel = nullptr;
	// 玩家子弹的数量
	cocos2d::Label* m_pBombNumLabel = nullptr;
	// 回收子弹的按钮
	cocos2d::MenuItemImage* m_pRecyclingButton = nullptr;
	// 游戏结算Layer
	bailin::ui::DialogLayer::Layer* m_pGameOverLayer = nullptr;
	// 触摸监听器
	cocos2d::EventListenerTouchOneByOne* m_pEventListenerTouch = nullptr;
private:
	// 触屏点击的X轴位置
	float m_fPosX;
	// 玩家总得分
	int m_nScore;
	// 本回合得分
	int m_nRoundScore;
	// 本回合得分等级
	int m_nRoundLevel;
	// 炮台指针
	FortNode* m_pFortNode = nullptr;
	// 运动状态子弹和飞行中的子弹
	int m_nMoveBombNum;
	int m_nTimer;
    // 一局游戏的时间
    double m_dRoundTime = 0;
	// 已经发送的子弹个数
	int m_nAlreadyShootBombNum;
	// 判断是否要移动炮台
	bool m_bMoveFort;
	// 子弹的数量
	int m_nBombNum;
	// 得分Label的缩放比例
	float m_fScoreLabelScale;
	// 控制炮台是否可以发射
	bool m_bIsPressed;
};


#endif // !__BB_GAME_SCENE_H__
