#include "HelpDialog.h"
#include "bailinUtils/ControlUtil.h"

USING_NS_CC;

bool HelpDialog::init()
{
	bool bRet = false;

	do
	{
		CC_BREAK_IF(!DialogLayer::init());
        
        // 背景透明度低一点，本身帮助图片是半透明的
        setOpacity(200);

		// 初始化帮助图片
		m_pSpriteHelp = Sprite::create("BB_Bomb/Image/help/help.png");
		m_pSpriteHelp->setPosition(_contentSize*0.5f);
		addChild(m_pSpriteHelp);
        
        auto pSpriteClose = Sprite::create("BB_Bomb/Image/help/help_btn_close.png");
        // 增加关闭按钮
        auto menu = Menu::create();
        menu->setContentSize(Size::ZERO);
        menu->setPosition(m_pSpriteHelp->getContentSize() -
                          pSpriteClose->getContentSize()*0.5f);
        auto pMenuItem = MenuItemImage::create();
        pMenuItem->setNormalImage(pSpriteClose);
        pMenuItem->setCallback([this](Ref*){
            closeDialog();
        });
        menu->addChild(pMenuItem);
        
        m_pSpriteHelp->addChild(menu);
        
        bailin::util::ControlUtil::setMenuItemSelectedScale(pMenuItem, 1.05f);
        
        // 设置主面板，点击外部会关闭对话框
        setMainBoardNode(m_pSpriteHelp);

		bRet = true;

	} while(0);

	return bRet;
	
}
