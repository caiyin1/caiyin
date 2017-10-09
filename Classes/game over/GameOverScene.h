#include "cocos2d.h"

//#ifdef  __GAMEOVER_SCENE_H__
#define __GAMEOVER_SCENE_H__


class GameOverScene : public cocos2d::Scene
{
public: 

	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(GameOverScene);
	void MenuStartCallBack(cocos2d::Ref* pSender);
	void MenuCloseCallback(cocos2d::Ref* pSender);
};








//#endif	//__GAMEOVER_SCENE_H__