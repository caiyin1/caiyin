#ifndef __GAME_DEPLOY_H__
#define __GAME_DEPLOY_H__

#define USER_NAME "NAME"
// 子弹编号
#define BOMB_TAG_NUM 1
// 块块编号
#define BLOCK_TAG_NUM 2
// 黑洞块块编号
#define BLACK_HOLE_TAG_NUM 5
// 碰撞底边编号
#define BOUNDING_TAG_NUM 3
// 食物的编号
#define FOOD_TAG_NUM 4
// 格子的标准大小
#define BLOCK_SIZE 135
// 爆炸效果事件名称
#define BLOCK_EVENT "ParticleRatio_Block"

// 字体
#if defined(CC_PLATFORM_IOS) || defined(CC_PLATFORM_MAC)
#   define FNT_NAME "Helvetica Bold"
#else
#   define FNT_NAME "Arial"
#endif


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
	float getTriangleBlockAddProbaility();
	/*
	* @brief 获取黑洞的生成概率
	*/
	float getBlackHoleBlockAddProbaility();
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
	/*
	* @brief 获取箭头提示缩放比
	*/
	float getArrowScalingRatio();
	/*
	* @brief 获取箭头的极限角度
	*/
	float getArrowLimitAngle();
private:
	// 游戏缩放比
	float m_fScalingRatio;
	// 游戏块块的列数
	int m_nGameColumnNum;
	// 正常块块生成的概率
	float m_fNomeBlockAddProbaility;
	// 三角形块块的生成概率
	float m_fTriangleBlockAddProBaility;
	// 黑洞块块的生成概率
	float m_fBlackHoleBlockAddProBaility;
	// 黑洞块块的Hp
	int m_nBlackHoleBlockHp;
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
private:
	// 箭头提示缩放比
	float m_fArrowScalingRatio;
	// 箭头的极限角度
	float m_fArrowLimitAngle;
};
#endif // !__GAME_DEPLOY_H__
