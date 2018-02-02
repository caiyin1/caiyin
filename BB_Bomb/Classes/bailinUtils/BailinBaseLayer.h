/*
 * =====================================================================================
 *
 *       Filename:  BailinBaseLayer.h
 *
 *    Description:  百灵库中，UI界面继承的基类，封装了一些简便的函数用以处理一些通用功能
 *
 *        Version:  1.0
 *        Created:  07/02/2015 15:56:55
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jason Tou (), sosoayaen@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */

#pragma once

#include "cocos2d.h"
#include "bailinMacro.h"
 // #include "bailinEvent.h"

NS_BL_UI_BEGIN

class BailinBaseLayer : public cocos2d::Layer
{
public:

	virtual void onExit() override
	{
		////////////////////////
		// onExit中处理，移除CustomListener
		removeAllCustomListener();

		cocos2d::Layer::onExit();
	}

	/**
	 * @brief 添加自定义监听事件
	 */
	virtual cocos2d::EventListenerCustom* addCustomEventListener(const std::string &eventName, const std::function<void(cocos2d::EventCustom*)>& callback)
	{
		auto listener = _eventDispatcher->addCustomEventListener(eventName, callback);
		m_listEventListenerCustom.push_back(listener);
		return listener;
	}

protected:

	// 移除当前层的所有自定义通知
	void removeAllCustomListener()
	{
		// 移除注册的事件
		for (auto l : m_listEventListenerCustom)
		{
			_eventDispatcher->removeEventListener(l);
		}

		// 顺便清空下
		m_listEventListenerCustom.clear();

	}

protected:
	// 保存自定义监听的链表，出场景的时候删除
	std::list<cocos2d::EventListenerCustom*> m_listEventListenerCustom;
};

NS_BL_UI_END
