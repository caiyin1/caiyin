/**
* Generate by GenerateCppCodeFromCCBFile.lua
* All rights received
* Author: Jason Tou
* Date: 2018-02-04 22:04:10
*/

#ifndef __RESULTDIALOG__H__
#define __RESULTDIALOG__H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "editor-support/cocosbuilder/CocosBuilder.h"

#include "bailinUtils/DialogLayer.h"

class ResultDialog :
	public bailin::ui::DialogLayer,
	public cocosbuilder::CCBMemberVariableAssigner
{
public:
	// Constructor
	ResultDialog();
	~ResultDialog();

	CREATE_FUNC(ResultDialog);

	virtual bool onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, cocos2d::Node* pNode) override;


private:
	// Attributes for CCB
	// CCMenuItemImage确定按钮
	cocos2d::MenuItemImage* m_pMenuItemOK = nullptr;
	// CCLabelBMFont当前得分
	cocos2d::Label* m_pLabelBMFontScore = nullptr;
	// CCLabelTTF最高分
	cocos2d::Label* m_pLabelHighScore = nullptr;
	// CCLabelTTF最高分标题
	cocos2d::Label* m_pLabelHighScoreTitle = nullptr;
	// CCLayer最高分居中辅助容器，设置分数后需要重新调整容器大小
	cocos2d::Layer* m_pLayerHighScoreAlignContainer = nullptr;
	// CCLayer最高分大小外部容器
	cocos2d::Layer* m_pLayerHighScoreSizeContainer = nullptr;
	// CCSprite背景框
	cocos2d::Sprite* m_pLayerMainboard = nullptr;


private:
	// Init Menu
	void initMenuControl();

	// 从公共存储数据中读取当前结束对话框需要显示的数据

	void initDataAndUI();


protected:
	// 点击确定按钮后的外部回调函数
	std::function<void()> m_pCallbackComfirm = nullptr;

public:
	// Virtual Functions
	virtual bool init() override;
	virtual void onEnter() override;
	virtual void onExit() override;


public:
	// Funcitons
	void setComfirmBtnClickCallback(const std::function<void()>& callback);
};

#endif // __RESULTDIALOG__H__
