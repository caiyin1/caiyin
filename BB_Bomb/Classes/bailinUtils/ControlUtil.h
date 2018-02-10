/*
 * =====================================================================================
 *
 *       Filename:  ControlUtil.h
 *
 *    Description:  一些控件的公共操作方法集合
 *
 *        Version:  1.0
 *        Created:  01/03/2015 15:07:38
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

NS_BL_UTIL_BEGIN

class ControlUtil
{
public:
	/**
	 * @brief 设置Menu内部的所有Item按下后的缩放比例
	 * @param pMenu 对应的按钮对象
	 * @param scale 默认是缩小0.95倍
	 */
	static void setMenuItemSelectedScale(cocos2d::Menu* pMenu, float scale = 0.95f);

	/**
	 * @brief 设置MenuItem按下后的缩放比例
	 * @param pMenuItem 对应的按钮对象
	 * @param scale 默认是缩小0.95倍
	 */
	static void setMenuItemSelectedScale(cocos2d::MenuItemSprite* pMenuItem, float scale = 0.95f);

	/**
	 * @brief 设置MenuItem按下后的残影效果
	 * @param pMenu 对应的按钮组对象
	 * @param color 残影颜色，默认橘红
	 * @param fRadius 判断的半径，默认0.01
	 * @param fThreshold 判断是否晕染的阙值，默认1.75，越小表示判断透明区域越精细
	 */
	static void setMenuItemSelectedOutline(cocos2d::Menu* pMenu, const cocos2d::Color3B color = cocos2d::Color3B::ORANGE, float fRadius = 0.01f, float fThreshold = 1.75f);

	/**
	 * @brief 设置MenuItem按下后的残影效果
	 * @param pMenuItem 对应的按钮对象
	 * @param color 残影颜色，默认橘红
	 * @param fRadius 判断的半径，默认0.01
	 * @param fThreshold 判断是否晕染的阙值，默认1.75，越小表示判断透明区域越精细
	 */
	static void setMenuItemSelectedOutline(cocos2d::MenuItemSprite* pMenuItem, const cocos2d::Color3B color = cocos2d::Color3B::ORANGE, float fRadius = 0.01f, float fThreshold = 1.75f);

	/**
	 * @brief 设置Menu的不可按下为灰色系
	 * @param pMenu 对应的按钮组对象
	 */
	static void setMenuItemDisabledGray(cocos2d::Menu* pMenu);

	/**
	 * @brief 设置MenuItem的不可按下为灰色系
	 * @param pMenuItem 对应的按钮对象
	 */
	static void setMenuItemDisabledGray(cocos2d::MenuItemSprite* pMenuItem);

	/**
	 * @brief 设置Menu是否可用
	 * @param pMenu 对应的按钮对象
	 * @param bEnable 是否可用
	 */
	static void setMenuEnabled(cocos2d::Menu* pMenu, bool bEnable);

	/**
	 * @brief 设置按钮内部把隐藏的MenuItem不参与排列布局
	 */
	static void setMenuAlignmentVerticalWithPadding(cocos2d::Menu* pMenu, float fPadding);
	static void setMenuAlignmentHorizontalWithPadding(cocos2d::Menu* pMenu, float fPadding);

	/**
	 * @brief 设置按钮内部的自按钮水平均分排布
	 * @param pMenu 按钮容器
	 * @param fMargin 左右缩进
	 */
	static void setMenuAlignmentHorizontalWithAvarageWidth(cocos2d::Menu* pMenu, float fMargin = 0);

	/**
	 * @brief 专门用来设置按钮上的金额，pContainer是MenuItem的NormalSprite或者SelectedSprite
	 * @param pContainer 就是MenuItemImage->getNormalImage()等等的节点
	 * @param strValue 设置的值，字符串形式
	 */
	static void setMenuItemLabel(cocos2d::Node* pContainer, const std::string& strValue, const std::string& strKey, const cocos2d::Point& positionRate = cocos2d::Point::ANCHOR_MIDDLE, float fScaleWidthRate = 0.8f);
	static void setMenuItemLabel(cocos2d::Node* pContainer, const std::string& strBMFontPath, const std::string& strValue, const std::string& strKey, const cocos2d::Point& positionRate = cocos2d::Point::ANCHOR_MIDDLE, float fScaleWidthRate = 0.8f, float fFontScale = 1.0f);
	static void setMenuItemLabel(cocos2d::MenuItemImage* pMenuItem, const std::string& strBMFontPath, const std::string& strValue, const std::string& strKey, const cocos2d::Point& positionRate = cocos2d::Point::ANCHOR_MIDDLE, float fScaleWidthRate = 0.8f, float fFontScale = 1.0f);

	static void addSpriteWithPostionAndAnchor(cocos2d::Node* pParent, const std::string& strFileName, const cocos2d::Vec2& position = cocos2d::Vec2::ZERO, const cocos2d::Vec2& anchor = cocos2d::Vec2::ANCHOR_MIDDLE);
	/**
	 * @brief 根据几个已经定义的锚点方位，得到对应的的位置
	 * @param ratePoint 取值锚点的位置，比如输入是Vec2::ANCHOR_TOP_LEFT，返回的就是控件左上角的位置
	 */
	static cocos2d::Point getPointWithRatePoint(const cocos2d::Node* pNodeTarget, const cocos2d::Point& ratePoint);


	/**
	 * @brief 在pNodeParent节点上加上圆圈简介,指定 theChild 在被剪切范围内.
	 */
	static void addRoundClipForNode(cocos2d::Node*pNodeParent, cocos2d::Node* theChild);

	/**
	 * @brief 查找内部的节点
	 */
	static cocos2d::Node* getNodeByName(cocos2d::Node* pRootNode, const std::string& strName);

	/**
	 * @brief 父节点有一个是不可见的，就表明节点不可见
	 * @param pNode
	 */
	static bool isVisible(cocos2d::Node* pNode);

	/**
	 * @brief 递归设置节点下所有LabelTTF的字体大小
	 * @param pRootNode     需要设置的根节点
	 */
	static void fitFontSize(cocos2d::Node* pRootNode);

	/**
	 * @brief 把对应的图像变灰
	 * @param pSprite 待变灰的节点
	 */
	static void shaderGray(cocos2d::Node* pNode, bool bCascaded = false);

	/**
	 * @brief 把对应的图使用外描绘边缘shader
	 * @param pSprite
	 */
	static void shaderOutline(cocos2d::Sprite *pSprite, cocos2d::Vec3 color = cocos2d::Vec3(1.0f, 1.0f, 1.0f), float fRadius = 0.01f, float fThreshold = 1.75f);

	/**
	 * @brief 设置对象为暗色调
	 * @param pNode 需要变灰的对象
	 * @param bCascaded 是否递归
	 */
	static void shaderDark(cocos2d::Node* pNode, bool bCascaded = false);

	/**
	 * @brief 恢复精灵原有的shader
	 */
	static void shaderReset(cocos2d::Node* pNode, bool bCascaded = false);

	/**
	 * @brief 强制关闭当前节点的父类对话框，回溯寻找，只要找到一个DialogLayer，就调用closeDialog
	 */
	static void forceCloseDialog(cocos2d::Node* pNode);

	/**
	 * @breif 设置引导的 TagName，仅仅用作统计引导标签设置，和 setName 一模一样的功能
	 */
	static void setGuideTagName(cocos2d::Node* pNode, const std::string& strTagName)
	{
		if (pNode) pNode->setName(strTagName);
	}

	/**
	 * @brief 提供字符串到颜色值的转换
	 */
	static cocos2d::Color3B getColor3BWithStringRGB(const std::string& strColor);
	static cocos2d::Color4B getColor4BWithStringRGBA(const std::string& strColor);

	/**
	 * @brief 提供从 Color3B 和 Color3B 到 RGB 和 RGBA 的字符串转换
	 */
	static std::string getStringRGBWithColor3B(const cocos2d::Color3B& color);
	static std::string getStringRGBAWithColor4B(const cocos2d::Color4B& color);

	/**
	 * @brief 把 Label 设置为来回可看全数据的跑马灯模式
	 */
	static cocos2d::Node* getShowFullLabelContentNode(cocos2d::Label* pLabel, const cocos2d::Size& size);

};

NS_BL_UTIL_END
