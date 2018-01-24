#ifndef __FORT_NODE_H__
#define __FORT_NODE_H__
#include "cocos2d.h"

class FortNode : public cocos2d::Node
{
public:
	static FortNode* create();
	virtual bool init() override;
public:
	/*
	* @brief 添加一个移动炮台的动画
	*/
	void setMoveAction();
	/*
	* @brief 添加一个箭头
	*/
	void addArrow(const cocos2d::Vec2& pos);
	/*
	* @brief 删除箭头
	*/
	void removeArrow();
	/*
	* @brief 修改箭头方向
	*/
	void setArrowRotation(const cocos2d::Vec2& pos);
	/*
	* @brief 获取箭头的弧度
	*/
	float getArrowRadian();
private:
	/*
	* @brief 锁死射击角度
	*/
	void handleShootAngle(float fAngle);
private:
	cocos2d::Sprite* m_pFortSprite = nullptr;
	cocos2d::Sprite* m_pArrowSprite = nullptr;
	cocos2d::Sprite* m_pDottedLineSprite = nullptr;
	cocos2d::Sprite* m_pButtonSprite = nullptr;
	// 记录创建箭头时鼠标的位置
	cocos2d::Point m_beginPos;
	// 计算角度时的对比向量
	cocos2d::Point m_startVector;
	// 箭头的弧度
	float m_fArrowRadian;
};

#endif // !__FORT_NODE_H__
