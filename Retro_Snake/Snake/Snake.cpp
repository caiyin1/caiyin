#include "Snake.h"
USING_NS_CC;



Snake* Snake::create()
{
	Snake* pRet = new Snake;
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

bool Snake::init()
{
	auto snakeHead = Dr

}

