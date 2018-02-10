#ifndef __READ_RECORD_DIALOG_H__
#define __READ_RECORD_DIALOG_H__
#include "../../bailinUtils/DialogLayer.h"
#include "cocos2d.h"

class ReadRecordDialog : public bailin::ui::DialogLayer
{
public:
	CREATE_FUNC(ReadRecordDialog);
public:
	ReadRecordDialog();
	~ReadRecordDialog();
public:
	void setOkButtonCallBack(const std::function<void()>& fun);
private:
	virtual bool init() override;
	virtual void onEnter()override;
	virtual void onExit() override;
public:
	cocos2d::MenuItemImage* m_pOkButton = nullptr;
	cocos2d::MenuItemImage* m_pCancelButton = nullptr;

private:
	// 点击确定外部回调函数
	std::function<void()> m_pCallbackOK = nullptr;
};
#endif
