#ifndef __BOMB_DATA_MANAGER_H__
#define __BOMB_DATA_MANAFER_H__
#include "../UI/BombNode.h"
#include <string>
#include <list>

class BombDataManager
{
public:
	static BombDataManager* getInstance();
	void init();
public:
	// 获取子弹数组
	const std::vector<BombNode* >& getBombVector();
	/*
	* @brief 生成子弹
	*/
	BombNode* addBombNode();
	/*
	* @brief 添加飞行的子弹
	*/
	void addFlyBombToVector(BombNode* pBombNode);
	/*
	* @brief 删除飞行的子弹
	*/
	void removeFlyBombToVector(BombNode* pBombNode);
	/*
	* @brief 重置飞行子弹数组的数据
	*/
	void clearFlyBomb();
	/*
	* @brief 获取飞行中Bomb的数组
	*/
	const std::vector<BombNode* >& getFlyBombVector();
	/*
	* @brief 删除子弹
	* @param 删除子弹的个数
	*/
	void removeBomb(int nNum = 1);
private:
	// 所有的子弹
	std::vector<BombNode* > m_vBombNode;
	// 飞行中的子弹
	std::vector<BombNode* > m_vFlyBombNode;
};

#endif // !__BOMB_DATA_MANAGER_H__
