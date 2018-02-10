/*
 * =====================================================================================
 *
 *       Filename:  HelpDialog.h
 *
 *    Description:  帮助对话框
 *
 *        Version:  1.0
 *        Created:  02/08/2018 13:51:21
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jason Tou (JT), sosoayaen@126.com
 *   Organization:  
 *
 * =====================================================================================
 */

#pragma once

#include "bailinUtils/DialogLayer.h"

class HelpDialog : public bailin::ui::DialogLayer
{
public:
	CREATE_FUNC(HelpDialog);

public:
	virtual bool init() override;

private:
	cocos2d::Sprite* m_pSpriteHelp = nullptr;


};
