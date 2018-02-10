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
	/*
	* @brief 设置块块上一回合的位置
	*/
	void setLastBlockPosition(const cocos2d::Vec2& pos);
	/*
	* @brief 获取块块上一回合位置
	*/
	const cocos2d::Vec2& getLastBlockPosition();
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
	// 块块的上一回合位置
	cocos2d::Vec2 m_LastPosition;
};
#endif // !__BLOCK_NODE_H__
