//#ifndef __DATA_MANAGER_H__
//#define __DATA_MANAGER_H__
//#include "cocos2d.h"
//// #include <vector.h>
//#include <string>
//#include "GameDeploy.h"
//#include "GameStatusManager.h"
//#include "../StructData/BlockData.h"
//
//
//class DataManager
//{
//public:
//	static DataManager* getInstance();
//	void init();
//public:
//	// 子弹数度和位置
//	class SpeedAndPoint
//	{
//	public:
//		cocos2d::Vec2 m_speed;
//		cocos2d::Vec2 m_point;
//	};
//public:
//	// 块块保存的数据
//	class BlockSaveData
//	{
//	public:
//		// 块块属性
//		BlockData::Type m_eType;
//		// 块块位置
//		cocos2d::Vec2 m_pos;
//		// 块块生命值
//		int m_nHp;
//	};
//public:
//	/*
//	* @brief 存档
//	*/
//	void saveRecord();
//	/*
//	* @brief 读取本地文件
//	* @retrun 本地有文件返回 true，反之返回false
//	*/
//	bool readingFiles();
//public:
//	/*
//	* @brief 获取子弹数
//	*/
//	const int& getBombSize();
//	/*
//	* @brief 获取需要发射的子弹数
//	*/
//	const int& getNeedShootBombSize();
//	/*
//	* @brief 获取当前得分
//	*/
//	const int& getScore();
//	/*
//	* @brief 获取子弹速度位置
//	*/
//	const std::vector<DataManager::SpeedAndPoint>& getBombSpeedAndPoint();
//	/*
//	* @brief 获取炮台x轴位置
//	*/
//	const float& getFortPosX();
//	/*
//	* @brief 获取食物位置
//	*/
//	const std::vector<cocos2d::Vec2>& getFoodPos();
//	/*
//	* @brief 获取块块数据
//	*/
//	const std::vector<DataManager::BlockSaveData>& getBlockSaveData();
//private:
//	// 子弹总数
//	int m_nBombSize;
//	// 需要发射的子弹数
//	int m_nNeedShootBomb;
//	// 当前得分
//	int m_nScore;
//	// 子弹的速度和位置
//	std::vector<SpeedAndPoint >& m_vBombSpeedAndPoint;
//	// 炮台x轴位置
//	float m_fFortPosX;
//	// 食物位置
//	std::vector<cocos2d::Vec2>& m_vFoodPoint;
//	// 块块数据
//	std::vector<BlockSaveData>& m_vBlockSaveData;
//	// 黑洞块块概率
//	int m_nBlackHoleBlockProbaility;
//	// 三角形块块概率
//	int m_nTriangleBlockProbaility;
//	// 八边形块块概率
//	int m_nOctagonBlockGenerateRate;
//	// 正方形块块概率
//	int m_nCubeBlockAddProbaility;
//	// 块块生命值
//	int m_nHp;
//	// 游戏状态
//	GameStatusManager::GameStatus m_eGameStatus;
//
//};
//
//#endif  __DATA_MANAGER_H__
