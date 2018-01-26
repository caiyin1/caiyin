#ifndef __BACKGROUND_DATA_MANAGER_H__
#define  __BACKGROUND_DATA_MANAGER_H__
#include <string>
#include "cocos2d.h"


class BackgroundDataManager
{
public:
	static BackgroundDataManager* getInstance();
	void init();
public:
	/*
	* @brief 添加生成块块数据
	*/
	void addBackData();
	/*
	* @biref 添加Block或者Food的回调
	* @param BlockNode* BlockNode或者FoodNode的指针
	*/
	void setAddBackgroundCallBack(const std::function<void(cocos2d::Node*)>& callBack);
	/*
	* @brief 重置数据
	*/
	void clear();
private:
	std::function<void(cocos2d::Node*)> m_callbackAddBackground = nullptr;
	//  已经生成的食物个数
	int m_nFoodSize;
};
#endif // !__BACKGROUND_DATA_MANAGER_H__
