#include "Snake.h"
USING_NS_CC;



Snake* Snake::create(int nNum)
{
	Snake* pRet = new Snake;
	if (pRet && pRet->init(nNum))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}

bool Snake::init(int nNum)
{
	bool bRet = false;
	do{
		if (!DrawNode::init())
		{
			return false;
		}
		switch (nNum)
		{
		case 1:
			drawTriangle(Vec2(0, 0), Vec2(20, 0), Vec2(10, 27), Color4F(1, 0, 0, 1));
			setContentSize(Size(20, 27));
			setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			break;
		case 2:
			drawSolidRect(Vec2(0, 0), Vec2(20, 20), Color4F(1, 0, 0, 1));
			setContentSize(Size(20, 20));
			setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			break;
		case 3:
			drawDot(Vec2(0, 0), 5, Color4F(1, 0, 0, 1));
			setContentSize(Size(10, 10));
			setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			break;
		}
		
		CCLOG("Snake : x = %f , y = %f", _contentSize.width, _contentSize.height);
		bRet = true;
	} while (0);
	return bRet;
}

