#ifndef __PRAISE_ACTION_NODE_H__
#define __PRAISE_ACTION_NODE_H__
#include "cocos2d.h"
#include <string>

class PraiseActionNode : public cocos2d::Node
{
public:
	enum class Type
	{
		// 真棒
		kType_Good,
		// 完美（绝妙）
		kType_Perfect,
		// 超神
		kType_Legendary
	};
public:
	static PraiseActionNode* create(const PraiseActionNode::Type& eType);
	bool initWithData(const PraiseActionNode::Type& eType);
public:
	void setAddedCallBack(const std::function<void(cocos2d::Node*, int)>& fun);
private:
	/*
	* @brief 设置Node的属性
	*/
	void setNode();
	/*
	* @brief 添加动画
	*/
	void addAnction();
	/*
	*  @brief 传入添加分数回调函数
	*/
private:
	/*
	* @brief 回调删除Node
	*/
	void removeCallFun();
	/*
	* @brief 回调添加额外分数
	*/
	void addedScoreCallFun();
private:
	cocos2d::Sprite* m_pSprite = nullptr;
	// 添加额外得分的Label
	// cocos2d::Label* m_pAddedScoreLabel = nullptr;
private:
	std::function<void(cocos2d::Node* pNode, int nAddedScore)> m_pFunCallBack = nullptr;
private:
	// 玩家需要添加的得分
	int m_nAddedScore;
};

#endif // __PRAISE_ACTION_NODE_H__
