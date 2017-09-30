#include "BackGroundScene.h"
#include <stdlib.h>
#include <string.h>

USING_NS_CC;

std::vector<Snake* > V_SnakeBody;

Scene* BackGroundScene::createScene()
{
	BackGroundScene* pRet = new BackGroundScene;
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
		////´´½¨Éß
		 m_pSnakeHead = Snake::create(1);
		m_pSnakeHead->setPosition(_contentSize * 0.5f);
		addChild(m_pSnakeHead);
		bRet = initDirection("D:\\material\\Direction.png");
		bRet = initDot();	
		bRet = initScore();
		m_pSnakeLast = m_pSnakeHead;
		this->schedule(schedule_selector(BackGroundScene::scheduleSnakeCollision), 0.01f);
	}while(0);
	return bRet;
}

//void BackGroundScene::SnakeAction(const Snake& snake)
//{
//	return;
//
//}

bool BackGroundScene::initDirection(const std::string& strDirectionImage)
{
	bool bRet = false;
	do 
	{
		auto LowerButton = MenuItemImage::create(strDirectionImage, strDirectionImage, this, menu_selector(BackGroundScene::menuLowerCallBack));
		CC_BREAK_IF(!LowerButton);

		auto UpperButton = MenuItemImage::create(strDirectionImage, strDirectionImage, this, menu_selector(BackGroundScene::menuUpperCallBack));
		UpperButton->setRotation(180);
		CC_BREAK_IF(!UpperButton);

		auto LeftButton = MenuItemImage::create(strDirectionImage, strDirectionImage, this, menu_selector(BackGroundScene::menuLeftCallBcak));
		LeftButton->setRotation(90);
		CC_BREAK_IF(!LeftButton);

		auto RightButton = MenuItemImage::create(strDirectionImage, strDirectionImage, this, menu_selector(BackGroundScene::menuRightCallBack));
		RightButton->setRotation(270);
		CC_BREAK_IF(!RightButton);

		m_pMenuVertical = Menu::create(UpperButton, LowerButton, NULL);
		m_pMenuVertical->alignItemsVerticallyWithPadding(5);
		m_pMenuVertical->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		m_pMenuVertical->setPosition(Vec2(_contentSize.width - LowerButton->getContentSize().width * 2,
			5 + LowerButton->getContentSize().height));
		addChild(m_pMenuVertical);

		m_pMenuHorizontal = Menu::create(LeftButton,RightButton, NULL);
		m_pMenuHorizontal->alignItemsHorizontallyWithPadding(RightButton->getContentSize().height + 10);
//		m_pMenuHorizontal->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
		m_pMenuHorizontal->setPosition(_contentSize.width + 20 - RightButton->getContentSize().width * 2.5, 22);
		addChild(m_pMenuHorizontal);
		bRet = true;
	} while (0);
	return bRet;

}

void BackGroundScene::menuLowerCallBack(cocos2d::Ref* Spender)
{
	m_SnakeHeadDirection = SnakeHeadLower;
	m_pMenuVertical->setEnabled(false);
	m_pMenuHorizontal->setEnabled(true);
	m_pSnakeHead->setRotation(180);
	this->unAllDirection();
	this->schedule(schedule_selector(BackGroundScene::scheduleLower), m_fTime);
}

void BackGroundScene::scheduleLower(float at)
{
	ActionSnakeBody();
	auto PosY = m_pSnakeHead->getPositionY();
	float fChange = fLevel;
	PosY -= fChange;
	m_pSnakeHead->setPosition(m_pSnakeHead->getPositionX(), PosY);
}

void BackGroundScene::menuUpperCallBack(cocos2d::Ref* Spender)
{
	m_SnakeHeadDirection = SnakeHeadUpper;
	m_pMenuVertical->setEnabled(false);
	m_pMenuHorizontal->setEnabled(true);
	m_pSnakeHead->setRotation(0);
	this->unAllDirection();
	this->schedule(schedule_selector(BackGroundScene::scheduleUpper), m_fTime);
}

void BackGroundScene::scheduleUpper(float at)
{
	ActionSnakeBody();
	auto PosY = m_pSnakeHead->getPositionY();
	float fChange = fLevel;
	PosY += fChange;
	m_pSnakeHead->setPosition(m_pSnakeHead->getPositionX(), PosY);
}

void BackGroundScene::menuRightCallBack(cocos2d::Ref* Spender)
{
	m_SnakeHeadDirection = SnakeHeadRight;
	m_pMenuVertical->setEnabled(true);
	m_pMenuHorizontal->setEnabled(false);
	m_pSnakeHead->setRotation(90);
	this->unAllDirection();
	this->schedule(schedule_selector(BackGroundScene::scheduleRight), m_fTime);
}

void BackGroundScene::scheduleRight(float at)
{
	ActionSnakeBody();
	auto PosX = m_pSnakeHead->getPositionX();
	float fChange = fLevel;
	PosX += fChange;
	m_pSnakeHead->setPosition(PosX, m_pSnakeHead->getPositionY());
}

void BackGroundScene::menuLeftCallBcak(cocos2d::Ref* Spender)
{
	m_SnakeHeadDirection = SnakeHeadLeft;
	m_pMenuVertical->setEnabled(true);
	m_pMenuHorizontal->setEnabled(false);
	m_pSnakeHead->setRotation(270);
	this->unAllDirection();
	this->schedule(schedule_selector(BackGroundScene::scheduleLeft), m_fTime);
}

void BackGroundScene::scheduleLeft(float at)
{
	ActionSnakeBody();
	auto PosX = m_pSnakeHead->getPositionX();
	float fChange = fLevel;
	PosX -= fChange;
	m_pSnakeHead->setPosition(PosX, m_pSnakeHead->getPositionY());

}

bool BackGroundScene::initDot()
{
	bool bRet = false;
	do 
	{
		m_pDot = Snake::create(3);
		this->schedule(schedule_selector(BackGroundScene::scheduleDot), 0.01f);
		addDot();
		addChild(m_pDot);
		bRet = true;
	} while (0);
	return bRet;
}

void BackGroundScene::scheduleDot(float at)
{
	Rect SnakeRect = m_pSnakeHead->getBoundingBox();
	CCLOG("x = %f, y = %f", SnakeRect.getMaxX(), SnakeRect.getMaxY());
	auto DotPos = m_pDot->getPosition();
	if (SnakeRect.intersectsCircle(DotPos, 5))
	{
		addDot();
		m_nScore++;
		addScore();
		addSnakeBody();
	}
}

void BackGroundScene::unAllDirection()
{
	this->unschedule(schedule_selector(BackGroundScene::scheduleLeft));
	this->unschedule(schedule_selector(BackGroundScene::scheduleUpper));
	this->unschedule(schedule_selector(BackGroundScene::scheduleLower));
	this->unschedule(schedule_selector(BackGroundScene::scheduleRight));
}

void BackGroundScene::addDot()
{
	while (1)
	{
		auto PosX = rand();
		auto PosY = rand();
		m_pDot->setPosition(Vec2(PosX % (int)(_contentSize.width), PosY % (int)_contentSize.height));
		if (!isSnakeBodyOverlap(m_pDot) && !isDotOverlop())
		{
			break;
		}
	}
}

bool BackGroundScene::initScore()
{
	bool bRet = false;
	do 
	{
		m_pScoreBg = Sprite::create();
		m_pScoreBg->setContentSize(_contentSize);
		m_pScoreBg->setPosition(_contentSize * 0.5f);
		addChild(m_pScoreBg);
		bRet = addScore();
	} while (0);
	return bRet;
}

std::string BackGroundScene::intToString()
{
	char buf[32] = { 0 };
	snprintf(buf, sizeof(buf), "%u", m_nScore);

	std::string str = "Score : ";
	str.append(buf);
	return str;
}

bool BackGroundScene::addScore()
{
	bool bRet = false;
	do 
	{
		m_pScoreBg->removeAllChildren();
		std::string strScore = intToString();
		auto labelScore = Label::createWithTTF(strScore, "fonts/Marker Felt.ttf", 20);
		CCLOG("%d", m_nScore);
		CC_BREAK_IF(!labelScore);
		labelScore->setPosition(_contentSize.width - labelScore->getContentSize().width * 0.5f - 10,
			_contentSize.height - labelScore->getContentSize().height);
		m_pScoreBg->addChild(labelScore);
		bRet = true;
	} while (0);
	return bRet;
}

void BackGroundScene::addSnakeBody()
{
	auto SnakeBody = Snake::create(2);
	if (V_SnakeBody.empty())
	{
		V_SnakeBody.push_back(SnakeBody);
	}
	else
	{
		V_SnakeBody.insert(V_SnakeBody.begin(), SnakeBody);
	}
	
	switch (m_SnakeHeadDirection)
	{
	case SnakeHeadUpper:
		SnakeBody->setPosition(Vec2(m_pSnakeHead->getPositionX(), m_pSnakeHead->getPositionY() - SnakeBody->getContentSize().height - 10));
		addChild(SnakeBody);
		break;
	case SnakeHeadLower:
		SnakeBody->setPosition(Vec2(m_pSnakeHead->getPositionX(), m_pSnakeHead->getPositionY() + SnakeBody->getContentSize().height + 10));
		addChild(SnakeBody);
		break;
	case SnakeHeadLeft:
		SnakeBody->setPosition(Vec2(m_pSnakeHead->getPositionX() + SnakeBody->getContentSize().width + 10, m_pSnakeHead->getPositionY()));
		addChild(SnakeBody);
		break;
	case SnakeHeadRight:
		SnakeBody->setPosition(Vec2(m_pSnakeHead->getPositionX() - SnakeBody->getContentSize().width - 10, m_pSnakeHead->getPositionY()));
		addChild(SnakeBody);
		break;
	}
	m_pSnakeLast = SnakeBody;

}

void BackGroundScene::ActionSnakeBody()
{
	if (V_SnakeBody.empty())
	{
		return;
	}
	int nNum = V_SnakeBody.size() - 1;
	for (nNum; nNum > 0; nNum --)
	{
		auto SnakePrevious = V_SnakeBody.at(nNum - 1);
		auto Pos = SnakePrevious->getPosition();
		auto snake = V_SnakeBody.at(nNum);
		snake->setPosition(Pos);
	}

		auto snake = V_SnakeBody.front();
		snake->setPosition(m_pSnakeHead->getPosition());

}

void BackGroundScene::scheduleSnakeCollision(float at)
{
	
	if (isSnakeHeadBorder())
	{
		this->unscheduleAllCallbacks();
	}
	if (V_SnakeBody.empty())
	{
		return;
	}
	if (isSnakeBodyOverlap(m_pSnakeHead))
	{
		this->unscheduleAllCallbacks();
	}
}

bool BackGroundScene::isSnakeHeadBorder()
{
	bool bRet = false;
	auto AnchorPosY = m_pSnakeHead->getPositionY();
	auto AnchorPosX = m_pSnakeHead->getPositionX();
	auto SnakeHeadSize = m_pSnakeHead->getContentSize();
	switch (m_SnakeHeadDirection)
	{
	case SnakeHeadUpper:
		if (AnchorPosY + SnakeHeadSize.height * 0.5f >= _contentSize.height)
		{
			bRet = true;
		}
		break;
	case SnakeHeadLower:
		if (AnchorPosY - SnakeHeadSize.height * 0.5f <= 0)
		{
			bRet = true;
		}
		break;
	case SnakeHeadRight:
		if (AnchorPosX + SnakeHeadSize.width * 0.5f >= _contentSize.width)
		{
			bRet = true;
		}
	case SnakeHeadLeft:
		if (AnchorPosX - SnakeHeadSize.width * 0.5f <= 0)
		{
			bRet = true;
		}
		break;
	}
	return bRet;

}

bool BackGroundScene::isSnakeBodyOverlap(Snake* snake)
{
	bool bRet = false;
	auto SnakeHeadRect = snake->getBoundingBox();
	for (auto snakeBody = V_SnakeBody.begin(); snakeBody != V_SnakeBody.end(); snakeBody++)
	{
		//auto snakeBody = V_SnakeBody.begin();
		auto snakeBodyRect = (*snakeBody)->getBoundingBox();
		if (SnakeHeadRect.intersectsRect(snakeBodyRect))
		{
			bRet = true;
			break;
		}
	}
	return bRet;
}

bool BackGroundScene::isDotOverlop()
{
	bool bRet = false;
	do{
		CC_BREAK_IF(m_pMenuHorizontal);
		CC_BREAK_IF(m_pMenuVertical);
		auto DotRect = m_pDot->getBoundingBox();
		auto MenuVerticalRect = m_pMenuVertical->getBoundingBox();
		auto MenuHorizontalRect = m_pMenuHorizontal->getBoundingBox();
		if (MenuHorizontalRect.intersectsRect(DotRect) || MenuVerticalRect.intersectsRect(DotRect))
		{
			bRet = true;
			return bRet;
		}
		CC_BREAK_IF(m_pScoreBg);
		auto ScoregSize = m_pScoreBg->getBoundingBox();
		if (ScoregSize.intersectsRect(DotRect))
		{
			bRet = true;
			return bRet;
		}
		CC_BREAK_IF(m_pSnakeHead);
		auto SnakeHeadSize = m_pSnakeHead->getBoundingBox();
		if (SnakeHeadSize.intersectsRect(DotRect))
		{
			bRet = true;
			return bRet;
		}
	} while (0);
	return bRet;
}