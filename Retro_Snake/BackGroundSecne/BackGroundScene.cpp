#include "BackGround.h"
USING_NS_CC;



Scene* BackGroundScene::create()
{
	BackGround* pRet = new BackGround;
	if (pRet && pRet->init())
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool BackGroundScene::init()
{
	bool bRet = false;
	do{
		CC_BREAK_IF(!Scene::init());
				
		bRet = true;
	}while(0);
	return bRet;
}

