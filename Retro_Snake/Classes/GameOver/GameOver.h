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
	* @breif ��ʼ��GameOver ��label 
	*/
	bool initGameOverLabel();
	bool initLabelButton();
	/*
	* @breif ������Ϸ�Ļص����� 
	*/
	void exitCallback(Ref* pSende);
	/*
	* @breif ׼���ص��ص����� 
	*/
	void recurCallback(Ref* pSende);
	/*
	* @breif ��ʾ������� 
	*/
	bool initPlayerScore();
private:
	cocos2d::Label* m_pGameOverLabel;


};

#endif // __GAME_OVER_H__ 