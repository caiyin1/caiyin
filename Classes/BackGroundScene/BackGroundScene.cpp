#include "BackGroundScene.h"
#include <stdlib.h>
#include <string.h>

USING_NS_CC;



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
		 m_pSnake = Snake::create(1);
		m_pSnake->setPosition(_contentSize * 0.5f);
		addChild(m_pSnake);
		bRet = initDirection("D:\\material\\Direction.png");
		bRet = initDot();	
		bRet = initScore();
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

		auto LeftButton = MenuItemImage::create(strDirectionImage, strDirectionImage, this, menu_selector(BackGroundScene::menuLeftCallBcak));
		LeftButton->setRotation(90);

		auto RightButton = MenuItemImage::create(strDirectionImage, strDirectionImage, this, menu_selector(BackGroundScene::menuRightCallBack));
		RightButton->setRotation(270);

		m_pMenuHorizontal = Menu::create(LeftButton,RightButton, NULL);
		m_pMenuVertical = Menu::create(UpperButton, LowerButton, NULL);
		//menu->alignItemsVertically();
		m_pMenuVertical->alignItemsVerticallyWithPadding(5);
		m_pMenuVertical->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		
		CCLOG("x = %f, y = %f", _contentSize.width, _contentSize.width);
		m_pMenuVertical->setPosition(Vec2(_contentSize.width - LowerButton->getContentSize().width * 2,
			5 + LowerButton->getContentSize().height));
		addChild(m_pMenuVertical);

		m_pMenuHorizontal->alignItemsHorizontallyWithPadding(RightButton->getContentSize().height + 10);
		m_pMenuHorizontal->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
		m_pMenuHorizontal->setPosition(_contentSize.width + 20 - RightButton->getContentSize().width * 2.5, 22);
		addChild(m_pMenuHorizontal);
		bRet = true;
	} while (0);
	
	return bRet;

}

void BackGroundScene::menuLowerCallBack(cocos2d::Ref* Spender)
{
	m_pMenuVertical->setEnabled(false);
	m_pMenuHorizontal->setEnabled(true);
	m_pSnake->setRotation(180);
	this->unAllDirection();
	this->schedule(schedule_selector(BackGroundScene::scheduleLower), 0.01f);
	
}

void BackGroundScene::scheduleLower(float at)
{
	auto PosY = m_pSnake->getPositionY();
	float fChange = fLevel;
	PosY -= fChange;
	m_pSnake->setPosition(m_pSnake->getPositionX(), PosY);
	//addChild(m_pSnake);
	
}

void BackGroundScene::menuUpperCallBack(cocos2d::Ref* Spender)
{
	m_pMenuVertical->setEnabled(false);
	m_pMenuHorizontal->setEnabled(true);
	m_pSnake->setRotation(0);
	this->unAllDirection();
	this->schedule(schedule_selector(BackGroundScene::scheduleUpper), 0.01f);
}

void BackGroundScene::scheduleUpper(float at)
{
	
	auto PosY = m_pSnake->getPositionY();
	float fChange = fLevel;
	PosY += fChange;
	m_pSnake->setPosition(m_pSnake->getPositionX(), PosY);
}

void BackGroundScene::menuRightCallBack(cocos2d::Ref* Spender)
{
	m_pMenuVertical->setEnabled(true);
	m_pMenuHorizontal->setEnabled(false);
	m_pSnake->setRotation(90);
	this->unAllDirection();
	this->schedule(schedule_selector(BackGroundScene::scheduleRight), 0.01f);
}

void BackGroundScene::scheduleRight(float at)
{
	auto PosX = m_pSnake->getPositionX();
	float fChange = fLevel;
	PosX += fChange;
	m_pSnake->setPosition(PosX, m_pSnake->getPositionY());

}

void BackGroundScene::menuLeftCallBcak(cocos2d::Ref* Spender)
{
	m_pMenuVertical->setEnabled(true);
	m_pMenuHorizontal->setEnabled(false);
	m_pSnake->setRotation(270);
	this->unAllDirection();
	
	this->schedule(schedule_selector(BackGroundScene::scheduleLeft), 0.01f);
}

void BackGroundScene::scheduleLeft(float at)
{
	auto PosX = m_pSnake->getPositionX();
	float fChange = fLevel;
	PosX -= fChange;
	m_pSnake->setPosition(PosX, m_pSnake->getPositionY());

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
	Rect SnakeRect = m_pSnake->getBoundingBox();
	CCLOG("x = %f, y = %f", SnakeRect.getMaxX(), SnakeRect.getMaxY());
	Point DotPos = m_pDot->getPosition();
	if (SnakeRect.containsPoint(DotPos))
	{
		addDot();
		m_nScore++;
		addScore();
		
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
	auto PosX = rand();
	auto PosY = rand();
	m_pDot->setPosition(Vec2(PosX % (int)(_contentSize.width), PosY % (int)_contentSize.height));
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
