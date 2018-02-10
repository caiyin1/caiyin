#include "ResultDialog.h"
#include "bailinUtils/ControlUtil.h"
#include "../Manager/UserDataManager.h"


USING_NS_CC;
USING_NS_CC_EXT;

// Add bailin::util namespace
USING_NS_BL_UTIL;



using namespace cocos2d::ui;
using namespace cocosbuilder;

// Constructor
ResultDialog::ResultDialog()
{

}

// Descontructor
ResultDialog::~ResultDialog()
{

}

bool ResultDialog::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!DialogLayer::init());
		NodeLoaderLibrary* pLoaderLib = NodeLoaderLibrary::newDefaultNodeLoaderLibrary();

		CCBReader* pCCBReader = new CCBReader(pLoaderLib);

		std::string strCCBFileName = "ResultDialog.ccbi";

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

		// 初始化界面数据
		initDataAndUI();

		bRet = true;

	} while (0);

	return bRet;
}

void ResultDialog::initDataAndUI()
{
	// TODO: 读取当前得分
	m_pLabelBMFontScore->setString(StringUtils::format("%d", UserDataManager::getInstance()->getPlayerScore()));

	// 读取历史最高分
	m_pLabelHighScore->setString(StringUtils::format("%d", UserDataManager::getInstance()->getHighScore()));

	// 居中最高分
	Size sizeNew;
	sizeNew.width = m_pLabelHighScoreTitle->getContentSize().width + m_pLabelHighScore->getContentSize().width;
	sizeNew.height = m_pLabelHighScoreTitle->getContentSize().height;
	m_pLayerHighScoreAlignContainer->setContentSize(sizeNew);
	auto maxSize = m_pLayerHighScoreAlignContainer->getParent()->getContentSize();
	m_pLayerHighScoreAlignContainer->setPosition(maxSize * 0.5f);
	// 超长的话缩小下
	if (sizeNew.width > maxSize.width)
		m_pLayerHighScoreAlignContainer->setScale(maxSize.width / sizeNew.width);
}

void ResultDialog::onEnter()
{
	DialogLayer::onEnter();

	// TODO: Your onEnter code here.
}

void ResultDialog::onExit()
{
	// TODO: Your onExit code here.

	DialogLayer::onExit();
}

void ResultDialog::initMenuControl()
{
	// TODO: Handle menu init code here.
	/* May be used controls
	* m_pMenuItemOK
	*/
	m_pMenuItemOK->setCallback([this](Ref*) {
		if (m_pCallbackComfirm)
			m_pCallbackComfirm();

		closeDialog();
	});

	ControlUtil::setMenuItemSelectedScale(m_pMenuItemOK, 1.05f);
}

bool ResultDialog::onAssignCCBMemberVariable(Ref* pTarget, const char* pMemberVariableName, Node* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pMenuItemOK", MenuItemImage*, this->m_pMenuItemOK);
	CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pLabelBMFontScore", Label*, this->m_pLabelBMFontScore);
	CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pLabelHighScoreTitle", Label*, this->m_pLabelHighScoreTitle);
	CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pLabelHighScore", Label*, this->m_pLabelHighScore);
	CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pLayerHighScoreAlignContainer", Layer*, this->m_pLayerHighScoreAlignContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pLayerHighScoreSizeContainer", Layer*, this->m_pLayerHighScoreSizeContainer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pLayerMainboard", Sprite*, this->m_pLayerMainboard);

	return true;
}

void ResultDialog::setComfirmBtnClickCallback(const std::function<void()>& callback)
{
	m_pCallbackComfirm = callback;
}
