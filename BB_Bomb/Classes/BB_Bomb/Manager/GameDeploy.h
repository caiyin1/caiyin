#ifndef __GAME_DEPLOY_H__
#define __GAME_DEPLOY_H__

#define USER_NAME "NAME"
// 子弹编号
#define BOMB_TAG_NUM 1
// 块块编号
#define BLOCK_TAG_NUM 2
// 碰撞底边编号
#define BOUNDING_TAG_NUM 3
// 食物的编号
#define FOOD_TAG_NUM 4
// 格子的标准大小
#define BLOCK_SIZE 135
// 爆炸效果事件名称
#define BLOCK_EVENT "ParticleRatio_Block"


class GameDeploy
{
public:
	static GameDeploy* getInstance();
	void init();
public:
	/*
	* @brief 设置屏幕缩放比
	*/
	void setScalingRatio(float fScalingRatio);
	/*
	* @brief 获取屏幕缩放比
	*/
	float getScalingRatio();
	/*
	* @brief 获取块块的生成概率
	*/
	float getAddNomeBlockProbaility();
	/*
	* @brief 获取块块的生命值
	*/
	int getBlockHP();
	/*
	* @brief 增加Block的HP
	* @Param nNum 块块HP的提高量 默认提高一
	*/
	void additionBlockHP(int nNum = 1);
	/*
	* @brief 获取食物的生成概率
	*/
	float getAddFoodProbaility();
	/*
	* @brief 获取子弹发送事件间隔
	*/
	int getBombShootTimer();
	/*
	* @brief 获取偏移放大量
	*/
	float getMoveScalingRatio();
	/*
	* @brief 获取子原始弹速度
	*/
	float getBombSpeed();
	/*
	* @brief 获取三角形Block的生成概率
	*/
	float getRightBlockAddProbaility();
	/*
	* @brief 增加游戏难度
	*/
	void addGameDifficult();
	/*
	* @brief 设置游戏的列数
	*/
	void setGameColumnNun(float fSize);
	/*
	* @brief 获取游戏的列数
	*/
	int getGameColumnNum();
private:
	// 游戏缩放比
	float m_fScalingRatio;
	// 游戏块块的列数
	int m_nGameColumnNum;
	// 正常块块生成的概率
	float m_fNomeBlockAddProbaility;
	// 左半边块块的生成概率
	float m_fRightBlockAddProBaility;
	// 块块的Hp
	int m_nBlockHP;
	// 获取食物的生成概率
	float m_fAddFoodProbaility;
	// 子弹发射时间间隔
	int m_nBombLaunchTimer;
	// 偏移放大量
	float m_fMoveScalingRatio;
	// 子弹的速度
	float m_fBombSpeed;
};
#endif // !__GAME_DEPLOY_H__
