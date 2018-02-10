#include "ReadRecordDialog.h"
#include "../Manager/GameDeploy.h"
#include "../Util/gbk2utf8.h"
#include "../Manager/DataManager.h"
USING_NS_CC;
using namespace cocos2d::ui;

ReadRecordDialog::ReadRecordDialog()
{

}

ReadRecordDialog::~ReadRecordDialog()
{

}

void ReadRecordDialog::onEnter()
{
	DialogLayer::onEnter();
}

void ReadRecordDialog::onExit()
{
	DialogLayer::onExit();
}

bool ReadRecordDialog::init()
{
	bool bRef = false;
	do
	{
		CC_BREAK_IF(!DialogLayer::init());
		// 创建对话框
		auto pDialogBox = Sprite::create("res/BB_Bomb/Image/bb_current_dialog.png");
		pDialogBox->setPosition(_contentSize * 0.5f);
		auto dialogBoxSize = pDialogBox->getContentSize();
		addChild(pDialogBox);

		// 确认按钮
		m_pOkButton = MenuItemImage::create("res/BB_Bomb/Image/bb_define.png", "res/BB_Bomb/Image/bb_define.png",
			[this](Ref*) {
			if (m_pCallbackOK) m_pCallbackOK();
			closeDialog();
		});
		auto pNodeFrist = m_pOkButton->getNormalImage();
		auto nodeSize = pNodeFrist->getContentSize();
		auto pNodeLast = m_pOkButton->getSelectedImage();
		pNodeLast->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		pNodeLast->setScale(1.05f);
		pNodeLast->setPosition(nodeSize * 0.5f);
		auto pOkMenu = Menu::create(m_pOkButton, nullptr);
		pOkMenu->setContentSize(Size(0, 0));
		pOkMenu->setPosition(dialogBoxSize.width * 0.7219f, dialogBoxSize.height * 0.1892f);
		pDialogBox->addChild(pOkMenu);

		// 取消按钮
		m_pCancelButton = MenuItemImage::create("res/BB_Bomb/Image/bb_cancel.png", "res/BB_Bomb/Image/bb_cancel.png",
			[this](Ref* spender) {
			DataManager::getInstance()->removeGameRecord();
			closeDialog();
		});
		pNodeFrist = m_pCancelButton->getNormalImage();
		nodeSize = pNodeFrist->getContentSize();
		pNodeLast = m_pCancelButton->getSelectedImage();
		pNodeLast->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		pNodeLast->setScale(1.05f);
		pNodeLast->setPosition(nodeSize * 0.5f);
		auto pCancelMenu = Menu::create(m_pCancelButton, nullptr);
		pCancelMenu->setContentSize(Size(0, 0));
		pCancelMenu->setPosition(dialogBoxSize.width * 0.2837f, dialogBoxSize.height * 0.1892f);
		pDialogBox->addChild(pCancelMenu);

		// 添加提示语句
		auto pGameDeploy = GameDeploy::getInstance();
		auto fScalingRatio = pGameDeploy->getScalingRatio();
		auto pLabel = Label::createWithSystemFont(bailinText(StringUtils::format("是否继续?")), FNT_NAME, 50 / fScalingRatio);
		pLabel->setPosition(dialogBoxSize.width * 0.5f, dialogBoxSize.height * 0.6071f);
		pDialogBox->addChild(pLabel);



		bRef = true;
	} while (0);
	return bRef;
}

void ReadRecordDialog::setOkButtonCallBack(const std::function<void()>& fun)
{
	m_pCallbackOK = fun;
}
