#ifndef __GAME_STATUS_MANAGER_H__
#define __GAME_STATUS_MANAGER_H__

class GameStatusManager
{
public:
	// 游戏状态
	enum class GameStatus
	{
		kStatus_None,				// 未知状态
		kStatus_Shoot,				// 发射状态
		kStatus_ReadyShoot,			// 准备发射状态
		kStatus_Runnning,			// 运行状态
		kStatus_Result,				// 结束状态
		kStatus_AddBack,			// 创建背景数据状态
		kStatus_RollScreen,			// 滚动屏幕状态
		kStatus_WaitMoveEnd,		// 等待动画结束状态
		kStatus_AddBomb,			// 添加子弹状态
		kStatus_Pause				// 暂停状态
	};
public:
	static GameStatusManager* getInstance();
	void init();
	/*
	* @brief 设置游戏状态
	*/
	void setGameStatus(GameStatus eStatus);
	/*
	* @brief 获取游戏状态
	*/
	GameStatus getGameStatus();
private:
	GameStatus m_eStatus;
};

#endif // !__GAME_STATUS_MANAGER_H__
