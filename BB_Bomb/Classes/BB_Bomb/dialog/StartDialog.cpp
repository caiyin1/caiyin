#include "StartDialog.h"
#include "bailinUtils/ControlUtil.h"
#include "../Manager/UserDataManager.h"
#include "BB_Bomb/Manager/UserDataManager.h"
#include "HelpDialog.h"

USING_NS_CC;
USING_NS_CC_EXT;

// Add bailin::util namespace
USING_NS_BL_UTIL;



using namespace cocos2d::ui;
using namespace cocosbuilder;

// Constructor
StartDialog::StartDialog()
{

}

// Descontructor
StartDialog::~StartDialog()
{

}

bool StartDialog::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!DialogLayer::init());
		NodeLoaderLibrary* pLoaderLib = NodeLoaderLibrary::newDefaultNodeLoaderLibrary();

		CCBReader* pCCBReader = new CCBReader(pLoaderLib);

		std::string strCCBFileName = "StartDialog.ccbi";

		// ccbi files source directory path
        std::string strCCBRootPath = "BB_Bomb/ccbi/";
        
        strCCBFileName = strCCBRootPath + strCCBFileName;
        
        Node* pNode = pCCBReader->readNodeGraphFromFile(strCCBFileName.c_str(), this);
        
        if (pNode != NULL)
        {
            this->addChild(pNode);
        }
        
        pCCBReader->release();

		ControlUtil::fitFontSize(pNode);

		// init menu control
		initMenuControl();
        
        // 读取最高分
        setHighScore(UserDataManager::getInstance()->getHighScore());
        
        // 设置界面动效
        setActionEffect();

		bRet = true;
		
	} while(0);
	
	return bRet;
}

void StartDialog::onEnter()
{
	DialogLayer::onEnter();

	// TODO: Your onEnter code here.
}

void StartDialog::onExit()
{
	// TODO: Your onExit code here.

	DialogLayer::onExit();
}

void StartDialog::initMenuControl()
{
	// TODO: Handle menu init code here.
	/* May be used controls
	 * m_pMenuItemStart
	 * m_pMenuItemHelp
	 * m_pMenuItemRank
	 */
    
    ControlUtil::setMenuItemSelectedScale(m_pMenuItemStart, 1.05f);
    m_pMenuItemStart->setCallback([this](Ref*){
        if (m_pCallbackStart)
            m_pCallbackStart();
        
        closeDialog();
    });
    
    ControlUtil::setMenuItemSelectedScale(m_pMenuItemHelp, 1.05f);
    m_pMenuItemHelp->setCallback([](Ref*){
        // TODO: 展现帮助对话框
        HelpDialog::create()->showModalDialog();
    });
    
    // 先隐藏排行榜按钮
    m_pMenuItemRank->getParent()->setVisible(false);
}

bool StartDialog::onAssignCCBMemberVariable( Ref* pTarget, const char* pMemberVariableName, Node* pNode )
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pS9SBackground", ui::Scale9Sprite*, this->m_pS9SBackground);
	CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pSpriteBlockBlue", Sprite*, this->m_pSpriteBlockBlue);
	CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pSpriteRole", Sprite*, this->m_pSpriteRole);
	CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pSpriteBlockPurple", Sprite*, this->m_pSpriteBlockPurple);
	CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pSpriteFireBall", Sprite*, this->m_pSpriteFireBall);
	CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pSpriteTitle", Sprite*, this->m_pSpriteTitle);
	CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pLabelHighScoreTitle", Label*, this->m_pLabelHighScoreTitle);
	CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pLabelBMFontHIghScore", Label*, this->m_pLabelBMFontHIghScore);
	CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pMenuItemStart", MenuItemImage*, this->m_pMenuItemStart);
	CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pMenuItemHelp", MenuItemImage*, this->m_pMenuItemHelp);
	CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pMenuItemRank", MenuItemImage*, this->m_pMenuItemRank);

	return true;
}

StartDialog* StartDialog::setCallbackStartBtn(const std::function<void()>& callback)
{
    m_pCallbackStart = callback;
    
    return this;
}

void StartDialog::setHighScore(int nScore)
{
    m_pLabelBMFontHIghScore->setString(StringUtils::format("%d", nScore));
    
    const auto& size = m_pLabelBMFontHIghScore->getContentSize();
    const auto& sizeContainer = m_pLabelBMFontHIghScore->getParent()->getContentSize();
    
    if (size.width > sizeContainer.width)
    {
        m_pLabelBMFontHIghScore->setScale(sizeContainer.width / size.width);
    }
}

void StartDialog::setActionEffect()
{
    // 人物浮动
    auto pAction = RepeatForever::create(Sequence::create(MoveBy::create(3.0f, Vec2(0, 10/CC_CONTENT_SCALE_FACTOR())),
                                                          MoveBy::create(3.0f, Vec2(0, -10/CC_CONTENT_SCALE_FACTOR())), NULL));
    m_pSpriteRole->runAction(pAction);
    
    // 火球浮动
    pAction = RepeatForever::create(Sequence::create(MoveBy::create(4.0f, Vec2(0, -20/CC_CONTENT_SCALE_FACTOR())),
                                                     MoveBy::create(4.0f, Vec2(0, 20/CC_CONTENT_SCALE_FACTOR())), NULL));
    m_pSpriteFireBall->runAction(pAction);
    
    // 蓝色块块
    pAction = RepeatForever::create(Sequence::create(MoveBy::create(2.0f, Vec2(0, 7/CC_CONTENT_SCALE_FACTOR())),
                                                     MoveBy::create(2.0f, Vec2(0, -7/CC_CONTENT_SCALE_FACTOR())), NULL));
    m_pSpriteBlockBlue->runAction(pAction);
    
    // 红色块块
    pAction = RepeatForever::create(Sequence::create(MoveBy::create(3.5f, Vec2(0, -15/CC_CONTENT_SCALE_FACTOR())),
                                                     MoveBy::create(3.5f, Vec2(0, 15/CC_CONTENT_SCALE_FACTOR())), NULL));
    m_pSpriteBlockPurple->runAction(pAction);
}
