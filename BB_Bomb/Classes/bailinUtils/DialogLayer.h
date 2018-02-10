/*
 * =====================================================================================
 *
 *       Filename:  DialogLayer.h
 *
 *    Description:  对话框类，提供模态和非模态对话框的实现
 *                  本类只适合cocos2d-x 3.x以上版本，并且需要支持C++11标准的编译器
 *
 *        Version:  2.0
 *        Created:  01/17/2015 14:29:28
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
#include "BailinBaseLayer.h"
#include "bailinMacro.h"

NS_BL_UI_BEGIN

class DialogLayer : public BailinBaseLayer
{
public:
    typedef std::function<void()> closeCallback;
    typedef std::function<void()> clickOutSideCallback;
    
	CREATE_FUNC(DialogLayer);
    
    ~DialogLayer();
    
    virtual bool init() override;
    
    virtual void onEnter() override;
    virtual void onExit() override;
    
    virtual void setOpacity(GLubyte opacity) override;
    
private:
    /**
     * @brief 初始化主面板，默认返回为true，子类继承后可以在这里初始化主面板
     */
    virtual bool initMainBoard() {  return true;    };
    
    /**
     * @brief 内部使用，判断是否点击在主面板上
     */
    bool isMainBoardContainTouch(cocos2d::Touch* touch);
    
public:
    
    // 设置进场出场动画
    virtual void setOnEnterAction(cocos2d::ActionInterval* pOnEnterAction);
    virtual void setOnExitAction(cocos2d::ActionInterval* pOnExitAction);
    
    /**
     * @brief 设置主面板，顺便设置面板的进场和出场动画效果。
     *        默认是模态对话框，并且按下主面板外区域即可关闭对话框，如果不需要此功能，请设置点击区域外关闭属性
     * @param pMainBoardNode 主面板节点，会把此节点
     */
    virtual void addMainBoardNode(cocos2d::Node* pMainBoardNode, cocos2d::ActionInterval* pOnEnterAction = nullptr, cocos2d::ActionInterval* pOnExitAction = nullptr);
    
    /**
     * @brief 设置点击到主面板外是否关闭对话框，默认是开启的
     */
    void setClickOutsideCloseDialog(bool bStatus);
    
    // 这里是弱引用，通过此函数设置主面板表示节点是在外面创建的，请不要随意调用此函数，除非你清楚知道你在做什么
    virtual void setMainBoardNode(cocos2d::Node* pMainBoardNode);
    
    // 关闭对话框
    virtual void closeDialog();
    
    /**
     * @brief 显示模态对话框，这个是用来直接把对话框丢到整个场景之上的
     * @param nZOrder Z轴，某些对话框要求在最前面，则可以填入这个值
     */
    virtual void showModalDialog(int nZOrder = 0);
    
    // 结束回调函数
    void setCloseCallback(const closeCallback& callback)
    {
        m_closeCallback = callback;
    }
    
    // 结束回调，在出场动画开始前调用
    void setCloseBeforeCallback(const closeCallback& callback)
    {
        m_closeCallbackBefore = callback;
    }
    
	// 点击到外部的函数
    void setClickOutSideCallback(const clickOutSideCallback& callback)
    {
        m_clickOutSideCallback = callback;
    }

    /**
     * @brief 注册一个在DialogLayer可响应点击的控件，先判断本层的响应，再判断注册进来的对象
     */
    void registerTouchNode(cocos2d::Node* pNode)
    {
        m_mapTouchNodes[pNode] = pNode;
    }
    
    void unregisterTouchNode(cocos2d::Node* pNode)
    {
        auto iter = m_mapTouchNodes.find(pNode);
        if (iter != m_mapTouchNodes.cend())
        {
            m_mapTouchNodes.erase(iter);
        }
	}
    
    void setBackgroundOpacity(GLbyte opacity)
    {
        m_pLayerColorBg->setOpacity(opacity);
    }
    
    void setBackgroundColor(cocos2d::Color3B color)
    {
        m_pLayerColorBg->setColor(color);
    }
    
protected:
    // 初始化当前层的吞噬效果
    void initTouchListener();
    
    bool onTouchBeganCallback(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMovedCallback(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchCancelledCallback(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEndedCallback(cocos2d::Touch* touch, cocos2d::Event* event);

protected:
    // 盖住整个框框的背景
    cocos2d::LayerColor* m_pLayerColorBg = nullptr;
    
    // 主面板
    cocos2d::Node* m_pMainBoardNode = nullptr;

    // 进出场动画
    cocos2d::FiniteTimeAction* m_pBoardOnEnterAction = nullptr;
    cocos2d::FiniteTimeAction* m_pBoardOnExitAction = nullptr;
    
    // 是否点击主面板外的区域关闭对话框
    bool m_bClickOutsideClose = true;
    
    // 为了吞噬事件用的监听
    cocos2d::EventListenerTouchOneByOne* m_pEventListener = nullptr;
    
    // 表明是否在当前层上按下
    bool m_bTouchOutMainboard = false;
    
    // 关闭回调
    closeCallback m_closeCallback = nullptr;
    // 关闭函数开始回调
    closeCallback m_closeCallbackBefore = nullptr;
    // 点击外部区域回调
    clickOutSideCallback m_clickOutSideCallback = nullptr;

	// 注册穿透点击的控件哈希表
	std::unordered_map<cocos2d::Node*, cocos2d::Node*> m_mapTouchNodes;
    
private:

};

NS_BL_UI_END
