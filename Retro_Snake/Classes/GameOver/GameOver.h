#ifndef __GAME_OVER_H__
#define  __GAME_OVER_H__

#include "cocos2d.h"

class GameOver : public cocos2d::Scene
{
public:
	static Scene* CreateScene();
	bool init();
	CREATE_FUNC(GameOver);
private:
	/*
	* @breif 初始化GameOver 的label 
	*/
	bool initGameOverLabel();
	bool initLabelButton();
	/*
	* @breif 结束游戏的回调函数 
	*/
	void exitCallback(Ref* pSende);
	/*
	* @breif 准备回调回调函数 
	*/
	void recurCallback(Ref* pSende);
	/*
	* @breif 显示玩家数据 
	*/
	bool initPlayerScore();
private:
	cocos2d::Label* m_pGameOverLabel;


};

#endif // __GAME_OVER_H__ 