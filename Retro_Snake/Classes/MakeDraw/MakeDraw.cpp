#include "MakeDraw.h"
USING_NS_CC;


MakeDraw* MakeDraw::create(int nNum)
{
	MakeDraw* pRet = new MakeDraw;
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

bool MakeDraw::init(int nNum)
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
			drawTriangle(Vec2(0, 0), Vec2(19, 0), Vec2(10, 19), Color4F(1, 0, 0, 1));
			setContentSize(Size(19, 19));
			setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			break;
		case 2:
			drawSolidRect(Vec2(0, 0), Vec2(19, 19), Color4F(1, 0, 0, 1));
			setContentSize(Size(19, 19));
			setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			break;
		case 3:
			drawDot(Vec2(0, 0), 5, Color4F(1, 0, 0, 1));
			setContentSize(Size(10, 10));
			setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			break;
		}
		bRet = true;
	} while (0);
	return bRet;
}

