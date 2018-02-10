/**
* Generate by GenerateCppCodeFromCCBFile.lua
* All rights received
* Author: Jason Tou
* Date: 2018-02-07 16:04:18
*/

#ifndef __STARTDIALOG__H__
#define __STARTDIALOG__H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "editor-support/cocosbuilder/CocosBuilder.h"

#include "bailinUtils/DialogLayer.h"

class StartDialog:
	public bailin::ui::DialogLayer,
	public cocosbuilder::CCBMemberVariableAssigner
{
public:
	// Constructor
	StartDialog();
	~StartDialog();

	CREATE_FUNC(StartDialog);

	virtual bool onAssignCCBMemberVariable( cocos2d::Ref* pTarget, const char* pMemberVariableName, cocos2d::Node* pNode ) override;


private:
	// Attributes for CCB
	// CCScale9Sprite背景图片层
	cocos2d::ui::Scale9Sprite* m_pS9SBackground = nullptr;
	// CCSprite蓝色块块
	cocos2d::Sprite* m_pSpriteBlockBlue = nullptr;
	// CCSprite人物角色
	cocos2d::Sprite* m_pSpriteRole = nullptr;
	// CCSprite紫色块块
	cocos2d::Sprite* m_pSpriteBlockPurple = nullptr;
	// CCSprite红色火球
	cocos2d::Sprite* m_pSpriteFireBall = nullptr;
	// CCSprite游戏标题
	cocos2d::Sprite* m_pSpriteTitle = nullptr;
	// CCLabelTTF历史最高分标题
	cocos2d::Label* m_pLabelHighScoreTitle = nullptr;
	// CCLabelBMFont历史最高分
	cocos2d::Label* m_pLabelBMFontHIghScore = nullptr;
	// CCMenuItemImage开始游戏
	cocos2d::MenuItemImage* m_pMenuItemStart = nullptr;
	// CCMenuItemImage帮助
	cocos2d::MenuItemImage* m_pMenuItemHelp = nullptr;
	// CCMenuItemImage排行榜
	cocos2d::MenuItemImage* m_pMenuItemRank = nullptr;


private:
	// Init Menu
	void initMenuControl();
    
    void setHighScore(int nScore);

    // 设置界面动效
    void setActionEffect();

protected:

    // 点击开始游戏的外部回调
    std::function<void()> m_pCallbackStart = nullptr;

public:
	// Virtual Functions
	virtual bool init() override;
	virtual void onEnter() override;
	virtual void onExit() override;

	
public:
	// Funcitons
    StartDialog* setCallbackStartBtn(const std::function<void()>& callback);
};

#endif // __STARTDIALOG__H__
