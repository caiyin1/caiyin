#ifndef __FORT_DATA_MANAGER_H__
#define __FORT_DATA_MANAGER_H__
#include "cocos2d.h"

class FortDataManager
{
public:
	static FortDataManager* getInstance();
	void init();
public:
	/*
	* @brief 设置子弹速度
	*/
	void setBombSpeed(const float& fAngle);
	void setBombSpeed(const cocos2d::Vec2& speed);
	/*
	* @brief 获取子弹速度
	*/
	const cocos2d::Vec2& getBombSpeed();
	/*
	* @brief 设置炮台位置
	*/
	void setFortPosition(const cocos2d::Point& fortPosition);
	/*
	* @brief 获取炮台位置
	*/
	const cocos2d::Point& getForPosition();
	/*
	* @brief 获取子弹个数
	*/
	int getBombSize();
	/*
	* @brief 子弹数量加一
	*/
	void setBombSize(int nNum = 1);
	/*
	* @brief 记录发射的弧度
	*/
	void setShootAngle(const float& fAngle);
private:
	// 炮台的位置
	cocos2d::Point m_fortPoint;
	// 炮台子弹个数
	int m_nBombSize;
	// 子弹速度
	cocos2d::Vec2 m_bombSpeed;
	// 子弹发射的弧度
	float m_fShootAngle;
};
#endif // !__FORT_DATA_MANAGER_H__
