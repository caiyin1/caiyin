#ifndef __BOMB_NODE_H__
#define __BOMB_NODE_H__
#include "cocos2d.h"
#include "../StructData/BombData.h"

class BombNode : public cocos2d::Node
{
public:
	static BombNode* create();
	virtual bool init() override;
public:
	enum class BombStatus
	{
		Status_Sleep, // 休眠
		Status_MoveTo, // 需要调节子弹位置
		Status_Move		// 在运动
	};
public:
	/*
	* @brief 初始化刚体
	* @param speed 子弹速度
	*/
	void initBombPhysicsBody();
	/*
	* @brief 删除刚体
	*/
	void removePhysicsBody();
	/*
	* @brief 设置子弹的数度
	*/
	void setBombSpeed(const cocos2d::Vec2& speed);
	/*
	* @brief 隐藏刚体效果
	*/
	void hidePhysicsBody();
	/*
	* @brief 设置子弹的状态
	*/
	void setBombStatus(const BombNode::BombStatus& eStatus);
	/*
	* @brief 获取子弹的状态
	*/
	const BombNode::BombStatus& getBombStatus();
private:
	cocos2d::Sprite* m_pBombSprite = nullptr;
	// 子弹刚体
	cocos2d::PhysicsBody* m_pBombBody = nullptr;
	BombStatus m_eBombStatus;
};

#endif // !__BOMB_NODE_H__
