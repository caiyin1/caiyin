#ifndef __FOOD_NODE_H__
#define __FOOD_NODE_H__
#include "cocos2d.h"
#include "../StructData/FoodData.h"

class FoodNode : public cocos2d::Node
{
public:
	static FoodNode* create(const FoodData& foodData);
	bool initWithData(const FoodData& foodData);
public:
	/*
	* @brief 碰撞删除食物
	*/
	void reomveFoodNode();
private:
	cocos2d::Sprite* m_pFoodSprite = nullptr;
	// 食物的刚体
	cocos2d::PhysicsBody* m_pFoodBody = nullptr;
	// 食物数据
	FoodData m_data;
};

#endif // !__FOOD_NODE_H__
