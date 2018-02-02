/*
 * =====================================================================================
 *
 *       Filename:  DialogLayer.cpp
 *
 *    Description:
 *
 *        Version:  2.0
 *        Created:  01/17/2015 14:32:41
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jason Tou (), sosoayaen@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */

#include "DialogLayer.h"

USING_NS_CC;

NS_BL_UI_BEGIN
DialogLayer::~DialogLayer()
{
	// 释放动作动画
	CC_SAFE_RELEASE_NULL(m_pBoardOnEnterAction);
	CC_SAFE_RELEASE_NULL(m_pBoardOnExitAction);

	// 移除监听
	_eventDispatcher->removeEventListener(m_pEventListener);
}

bool DialogLayer::isMainBoardContainTouch(Touch* touch)
{
	bool bRet = false;

	if (m_pMainBoardNode && m_pMainBoardNode->getParent())
	{
		Rect rect = m_pMainBoardNode->getBoundingBox();
		rect.origin = m_pMainBoardNode->getParent()->convertToWorldSpace(rect.origin);
		bRet = rect.containsPoint(touch->getLocation());
	}
	return bRet;
}

void DialogLayer::initTouchListener()
{
	// 吞噬
	m_pEventListener = EventListenerTouchOneByOne::create();
	m_pEventListener->setSwallowTouches(true);

	m_pEventListener->onTouchBegan = CC_CALLBACK_2(DialogLayer::onTouchBeganCallback, this);

	m_pEventListener->onTouchMoved = CC_CALLBACK_2(DialogLayer::onTouchMovedCallback, this);

	m_pEventListener->onTouchCancelled = CC_CALLBACK_2(DialogLayer::onTouchCancelledCallback, this);

	m_pEventListener->onTouchEnded = CC_CALLBACK_2(DialogLayer::onTouchEndedCallback, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_pEventListener, this);
}

static bool bailinIsVisible(cocos2d::Node* pNode)
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!pNode || !pNode->isVisible() || !pNode->getParent());

		auto parent = pNode->getParent();
		for (auto c = parent; c != nullptr; c = c->getParent())
		{
			if (!c->isVisible())
			{
				return false;
			}
		}

		bRet = true;

	} while (0);

	return bRet;
}

bool DialogLayer::onTouchBeganCallback(cocos2d::Touch *touch, cocos2d::Event *event)
{
	m_pEventListener->setSwallowTouches(true);

	// 判断下是否在控件内部点击
	if (m_pMainBoardNode && m_pMainBoardNode->getParent())
	{
		if (!this->isMainBoardContainTouch(touch))
		{
			m_bTouchOutMainboard = true;
		}
	}

	// 这里处理穿透性点击事件
	for (auto iter : m_mapTouchNodes)
	{
		// 首先判断对应的控件是否可见，不可见就不用折腾了
		if (!bailinIsVisible(iter.second))
			continue;

		// 首先处理下按钮，因为内部会有额外处理
		auto menu = dynamic_cast<Menu*>(iter.second);
		if (menu)
		{
			for (auto menuitem : menu->getChildren())
			{
				if (menuitem->isVisible())
				{
					Rect r = menuitem->getBoundingBox();
					// 如果点击到了内部的按钮，就穿透响应，把点击事件传递下去，并且本层不响应
					if (r.containsPoint(menu->convertToNodeSpace(touch->getLocation())))
					{
						m_pEventListener->setSwallowTouches(false);
						return false;
					}
				}
			}
		}
		else
		{
			bool bVisible = bailinIsVisible(iter.second);
			if (bVisible)
			{
				Rect r = iter.second->getBoundingBox();
				// 如果点击到了穿透的区域，就穿透响应，把点击事件传递下去，并且本层不响应
				if (r.containsPoint(iter.second->getParent()->convertToNodeSpace(touch->getLocation())))
				{
					m_pEventListener->setSwallowTouches(false);
					return false;
				}
			}
		}
	}

	return true;
}

void DialogLayer::onTouchMovedCallback(cocos2d::Touch *touch, cocos2d::Event *event)
{

}

void DialogLayer::onTouchCancelledCallback(cocos2d::Touch *touch, cocos2d::Event *event)
{
	m_bTouchOutMainboard = false;
}

void DialogLayer::onTouchEndedCallback(cocos2d::Touch *touch, cocos2d::Event *event)
{
	retain();
	// 判断下是否是点击到外面去了
	if (m_bTouchOutMainboard  && m_pMainBoardNode && m_pMainBoardNode->getParent())
	{
		if (!this->isMainBoardContainTouch(touch))
		{
			// 点击到外部释放的回调
			if (m_clickOutSideCallback)
				m_clickOutSideCallback();

			if (m_bClickOutsideClose)
				// 点击到外面去了，关掉对话框
				closeDialog();
		}
	}
	m_bTouchOutMainboard = false;
	release();
}

bool DialogLayer::init()
{
	bool bRet = false;

	do
	{
		CC_BREAK_IF(!BailinBaseLayer::init());

		CC_BREAK_IF(!initMainBoard());

		// 把默认颜色设置为半透明的黑色
		m_pLayerColorBg = LayerColor::create(Color4B(0, 0, 0, 128), _contentSize.width, _contentSize.height);
		addChild(m_pLayerColorBg);

		initTouchListener();

		bRet = true;
	} while (0);

	return bRet;
}

void DialogLayer::setOpacity(GLubyte opacity)
{
	BailinBaseLayer::setOpacity(opacity);

	m_pLayerColorBg->setOpacity(opacity);
}

void DialogLayer::setOnEnterAction(cocos2d::ActionInterval *pOnEnterAction)
{
	CC_SAFE_RELEASE_NULL(m_pBoardOnEnterAction);
	m_pBoardOnEnterAction = pOnEnterAction;
	CC_SAFE_RETAIN(m_pBoardOnEnterAction);
}

void DialogLayer::setOnExitAction(cocos2d::ActionInterval *pOnExitAction)
{
	CC_SAFE_RELEASE_NULL(m_pBoardOnExitAction);
	m_pBoardOnExitAction = pOnExitAction;
	CC_SAFE_RETAIN(m_pBoardOnExitAction);
}

void DialogLayer::closeDialog()
{
	if (m_closeCallbackBefore)
		m_closeCallbackBefore();

	// 有出场动画就播放一下
	if (m_pMainBoardNode && m_pBoardOnExitAction)
	{
		m_pMainBoardNode->runAction(Sequence::create(m_pBoardOnExitAction, CallFunc::create([=]() {
			if (m_closeCallback)
				m_closeCallback();
			// 先移除所有子节点
			removeAllChildren();
			// 把整个Dialog层移除
			removeFromParent();
		}), NULL));
	}
	else
	{
		if (m_closeCallback)
			m_closeCallback();
		// 先移除所有子节点
		removeAllChildren();
		// 直接移除即可
		removeFromParent();
	}
}

void DialogLayer::setMainBoardNode(cocos2d::Node *pMainBoardNode)
{
	// 这里是弱引用，通过此函数设置主面板表示节点是在外面创建的
	m_pMainBoardNode = pMainBoardNode;
}

void DialogLayer::addMainBoardNode(cocos2d::Node *pMainBoardNode, cocos2d::ActionInterval *pOnEnterAction, cocos2d::ActionInterval *pOnExitAction)
{
	if (pMainBoardNode)
	{
		m_pMainBoardNode = pMainBoardNode;
		// 默认当前主面板即为可视面板
		if (pMainBoardNode->getParent() == NULL)
		{
			addChild(pMainBoardNode);
		}
	}

	setOnEnterAction(pOnEnterAction);
	setOnExitAction(pOnExitAction);
}

void DialogLayer::setClickOutsideCloseDialog(bool bStatus)
{
	m_bClickOutsideClose = bStatus;
}

void DialogLayer::onEnter()
{
	BailinBaseLayer::onEnter();

	// 这里如果有进场动画就播放一下
	if (m_pMainBoardNode && m_pBoardOnEnterAction)
	{
		m_pMainBoardNode->stopAllActions();
		m_pMainBoardNode->runAction(m_pBoardOnEnterAction);
	}
}

void DialogLayer::onExit()
{

	BailinBaseLayer::onExit();
}

void DialogLayer::showModalDialog(int nZOrder)
{
	this->setLocalZOrder(nZOrder);
	Director::getInstance()->getRunningScene()->addChild(this);
}

NS_BL_UI_END
