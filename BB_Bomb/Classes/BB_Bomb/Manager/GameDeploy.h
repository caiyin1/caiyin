#ifndef __GAME_DEPLOY_H__
#define __GAME_DEPLOY_H__

#include "cocos2d.h"

#define USER_HIGH_SCORE "34764165c60806e099c6b6740927df7a"
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
#define BLOCK_SIZE (80 / cocos2d::Director::getInstance()->getContentScaleFactor())
// 爆炸效果事件名称
#define BLOCK_EVENT "ParticleRatio_Block"
// 保存游戏事件
#define  SAVE_GAME_SCHEDULE_EVENT "save_game_SCHEDULE_EVENT"
// 块块字体路径
#define BLOCK_NUM_BMFNT "res/BB_Bomb/bmFnt/block_hp.fnt"
// 分数字体路径
#define GOLDEN_SCORE_BMFNT "res/BB_Bomb/bmFnt/golden_score.fnt"

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
	* @brief 获取正方形块块的生成概率
	*/
	int getAddCubeBlockProbaility();
	/*
	* @brief 设置正方形块块的生成概率
	*/
	void setAddCubeBlockProbaility(int nProbabilty);
	/*
	* @brief 获取块块的生命值
	*/
	int getBlockHp();
	/*
	 * @brief 设置块块生命值
	*/
	void setBlockHp(int nHp);
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
	int getTriangleBlockAddProbaility();
	/*
	* @brief 设置三角形Block的生成概率
	*/
	void setTriangleBlockAddProbaility(int nProbabilty);
	/*
	* @brief 获取无block的概率
	*/
	int getNormalBlockProbaility();
	/*
	* @brief 获取八边形块块生成概率
	*/
	int getOctagonBlockProbaility();
	/*
	* @brief 设置八边形块块生成概率
	*/
	void setOctagonBlockProbaility(int nProbability);
	/*
	* @brief 获取黑洞的生成概率
	*/
	int getBlackHoleBlockAddProbaility();
	/*
	* @brief 设置黑洞的生成概率
	*/
	void setBlackHoleBlockAddProbaility(int nProbability);
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
	/*
	* @brief 设置游戏界面尺寸
	*/
	void setGameLayerSize(const cocos2d::Size& gameLayerSize);
	/*
	* @brief 获取界面尺寸
	*/
	const cocos2d::Size& getInterfaceSize();

	// 得到所有概率配置总和，可能超过100%
	int getTotalRatio();
private:
	// 游戏缩放比
	float m_fScalingRatio;
	// 游戏块块的列数
	int m_nGameColumnNum;
	// 正常块块生成的概率
	int m_nCubeBlockAddProbaility;
	// 八角形块块
	int m_nOctagonBlockGenerateRate;
	// 三角形块块的生成概率
	int m_nTriangleBlockAddProBaility;
	// 无块块的概率
	int m_nNormalBlockProBaility;
	// 黑洞块块的生成概率
	int m_nBlackHoleBlockAddProBaility;
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
private:
	// 游戏界面的尺寸
	cocos2d::Size m_gameLayerSize;

};
#endif // !__GAME_DEPLOY_H__
