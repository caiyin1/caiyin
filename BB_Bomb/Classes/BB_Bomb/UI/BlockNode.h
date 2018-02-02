#ifndef __BLOCK_NODE_H__
#define __BLOCK_NODE_H__
#include "cocos2d.h"
#include "../StructData/BlockData.h"

class BlockNode : public cocos2d::Node
{
public:
	static BlockNode* create(const BlockData& blockData);
	bool initWithData(const BlockData& blockData);
public:
	/*
	* @brief 撞击HP减少
	* @param nNum HP减少的值 默认减1
	*/
	void hitted(int nNum = 1);
	/*
	* @brief 获取Block的HP
	*/
	int getBLockHp();
	/*
	* @brief 获取Block的类别
	*/
	BlockData::Type getBlockType();
private:
	/*
	* @brief 创建黑洞
	*/
	void initBlackHole();
	/*
	* @brief 设置Block的tag
	*/
	void setBlockTag(int nNum);
private:
	BlockData m_BlockData;
	cocos2d::Sprite* m_pBlockSprite = nullptr;
	// 刚体
	cocos2d::PhysicsBody* m_pBlockBody = nullptr;
	// Block 的Hp
	cocos2d::Label* m_pHPLabel = nullptr;
};
#endif // !__BLOCK_NODE_H__
