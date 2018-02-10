/*
 * =====================================================================================
 *
 *       Filename:  ControlUtil.cpp
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  01/03/2015 15:19:20
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jason Tou (), sosoayaen@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */

#include "ControlUtil.h"
#include "DialogLayer.h"

USING_NS_CC;

NS_BL_UTIL_BEGIN

void ControlUtil::setMenuItemSelectedScale(cocos2d::MenuItemSprite *pMenuItem, float scale /* = 0.95f */)
{
	do
	{
		CC_BREAK_IF(!pMenuItem);

		// get selected sprite
		Node* pNodeSelected = pMenuItem->getSelectedImage();

		if (!pNodeSelected)
		{
			Sprite* pSpriteNormal = dynamic_cast<Sprite*>(pMenuItem->getNormalImage());
			CCAssert(pSpriteNormal, "Normal Sprite must existed!!");
			pNodeSelected = Sprite::createWithSpriteFrame(pSpriteNormal->getSpriteFrame());
			pMenuItem->setSelectedImage(pNodeSelected);
		}

		pNodeSelected->setScale(scale);

		float x = (scale - 1.0f)*pNodeSelected->getContentSize().width*0.5f;
		float y = (scale - 1.0f)*pNodeSelected->getContentSize().height*0.5f;

		pNodeSelected->setPosition(Point::ZERO - Point(x, y));

		pNodeSelected->setScale(scale);

	} while (0);
}

void ControlUtil::setMenuItemSelectedScale(cocos2d::Menu *pMenu, float scale /* = 0.95f */)
{
	do
	{
		CC_BREAK_IF(!pMenu);

		Vector<Node*> child = pMenu->getChildren();
		Vector<Node*>::iterator iter = child.begin();
		for (; iter != child.end(); iter++)
		{
			MenuItemSprite* pMenuItem = dynamic_cast<MenuItemSprite*>(*iter);
			setMenuItemSelectedScale(pMenuItem, scale);
		}

	} while (0);
}

void ControlUtil::setMenuItemSelectedOutline(cocos2d::Menu* pMenu, const cocos2d::Color3B color /*= cocos2d::Color3B::ORANGE*/, float fRadius /*= 0.01f*/, float fThreshold /*= 1.75f*/)
{
	do
	{
		CC_BREAK_IF(!pMenu);

		for (auto& node : pMenu->getChildren())
		{
			MenuItemSprite* pMenuItem = dynamic_cast<MenuItemSprite*>(node);
			setMenuItemSelectedOutline(pMenuItem, color, fRadius, fThreshold);
		}
	} while (0);
}

void ControlUtil::setMenuItemSelectedOutline(cocos2d::MenuItemSprite* pMenuItem, const cocos2d::Color3B color /*= cocos2d::Color3B::ORANGE*/, float fRadius /*= 0.01f*/, float fThreshold /*= 1.75f*/)
{
	do
	{
		CC_BREAK_IF(!pMenuItem);

		// get selected sprite
		Node* pNodeSelected = pMenuItem->getSelectedImage();

		if (!pNodeSelected)
		{
			Sprite* pSpriteNormal = dynamic_cast<Sprite*>(pMenuItem->getNormalImage());
			CCAssert(pSpriteNormal, "Normal Sprite must existed!!");
			pNodeSelected = Sprite::createWithSpriteFrame(pSpriteNormal->getSpriteFrame());
			pMenuItem->setSelectedImage(pNodeSelected);
		}
		Sprite* pSprite = dynamic_cast<Sprite*>(pNodeSelected);
		if (pSprite)
			shaderOutline(pSprite, Vec3(color.r, color.g, color.b), fRadius, fThreshold);

	} while (0);
}

void ControlUtil::setMenuItemDisabledGray(cocos2d::Menu* pMenu)
{
	do
	{
		CC_BREAK_IF(!pMenu);

		for (auto& node : pMenu->getChildren())
		{
			MenuItemSprite* pMenuItem = dynamic_cast<MenuItemSprite*>(node);
			setMenuItemDisabledGray(pMenuItem);
		}
	} while (0);
}


void ControlUtil::setMenuItemDisabledGray(cocos2d::MenuItemSprite* pMenuItem)
{
	do
	{
		CC_BREAK_IF(!pMenuItem);
		// get selected sprite
		Node* pNodeDisabled = pMenuItem->getDisabledImage();

		if (!pNodeDisabled)
		{
			Sprite* pSpriteNormal = dynamic_cast<Sprite*>(pMenuItem->getNormalImage());
			CCAssert(pSpriteNormal, "Normal Sprite must existed!!");
			pNodeDisabled = Sprite::createWithSpriteFrame(pSpriteNormal->getSpriteFrame());
			pMenuItem->setDisabledImage(pNodeDisabled);
		}
		Sprite* pSprite = dynamic_cast<Sprite*>(pNodeDisabled);
		if (pSprite)
			shaderGray(pSprite);

	} while (0);
}

void ControlUtil::setMenuEnabled(cocos2d::Menu *pMenu, bool bEnable)
{
	do
	{
		CC_BREAK_IF(!pMenu);
		for (auto node : pMenu->getChildren())
		{
			auto pMenuItem = static_cast<MenuItem*>(node);
			pMenuItem->setEnabled(bEnable);
		}

	} while (0);

}

void ControlUtil::setMenuAlignmentHorizontalWithPadding(cocos2d::Menu* pMenu, float fPadding)
{
	do
	{
		CC_BREAK_IF(!pMenu);

		float width = -fPadding;
		for (const auto &child : pMenu->getChildren())
			if (child->isVisible())
				width += child->getContentSize().width * child->getScaleX() + fPadding;

		float x = -width / 2.0f;

		for (const auto &child : pMenu->getChildren())
		{
			if (child->isVisible())
			{
				child->setPosition(x + child->getContentSize().width * child->getScaleX() / 2.0f, pMenu->getContentSize().height*0.5f);
				x += child->getContentSize().width * child->getScaleX() + fPadding;
			}
		}
	} while (0);
}

void ControlUtil::setMenuAlignmentVerticalWithPadding(cocos2d::Menu* pMenu, float fPadding)
{
	do
	{
		CC_BREAK_IF(!pMenu);

		float height = -fPadding;

		for (const auto &child : pMenu->getChildren())
			if (child->isVisible())
				height += child->getContentSize().height * child->getScaleY() + fPadding;

		float y = height / 2.0f;

		for (const auto &child : pMenu->getChildren())
		{
			if (child->isVisible())
			{
				child->setPosition(0, y - child->getContentSize().height * child->getScaleY() / 2.0f);
				y -= child->getContentSize().height * child->getScaleY() + fPadding;
			}
		}
	} while (0);
}

static std::vector<float> getAverageLayout(float fLength, unsigned int nPoints)
{
	std::vector<float> vRet;
	do
	{
		BREAK_IF(nPoints == 0);

		// 得到第一个中心点的距离
		float fStep = fLength / nPoints;
		float fHalfStep = fStep / 2;

		for (unsigned int idx = 0; idx < nPoints; idx++)
		{
			vRet.push_back(fHalfStep + idx*fStep);
		}
	} while (0);

	return vRet;
}

static std::vector<float> getAverageLayout2(float fLength, unsigned int nPoints)
{
	std::vector<float> vRet;
	do
	{
		BREAK_IF(nPoints == 0);
		float fStep = fLength / (nPoints + 1);

		for (unsigned int idx = 1; idx <= nPoints; idx++)
		{
			vRet.push_back(fStep*idx);
		}

	} while (0);

	return vRet;
}

void ControlUtil::setMenuAlignmentHorizontalWithAvarageWidth(cocos2d::Menu* pMenu, float fMargin)
{
	do
	{
		CC_BREAK_IF(!pMenu || pMenu->getChildrenCount() < 1);

		// 总可见数量
		auto nCount = 0;
		for (auto child : pMenu->getChildren())
			if (child->isVisible()) nCount++;

		auto vPosX = getAverageLayout(pMenu->getContentSize().width - 2 * fMargin, nCount);

		int idx = 0;
		for (const auto &child : pMenu->getChildren())
		{
			if (child->isVisible())
				child->setPosition(vPosX[idx++] + fMargin, pMenu->getContentSize().height*0.5f);
		}
	} while (0);
}

/**
 * @brief 专门用来设置按钮上的金额，pContainer是
 * @param pContainer 就是MenuItemImage->getNormalImage()等等的节点
 * @param strValue 设置的值，字符串形式
 */
void ControlUtil::setMenuItemLabel(Node* pContainer, const std::string& strValue, const std::string& strKey, const Point& positionRate, float fScaleWidthRate)
{
	if (!pContainer) return;

	Label* pLabelCost = dynamic_cast<Label*>(pContainer->getChildByName(strKey));
	if (!pLabelCost)
	{
		pLabelCost = Label::createWithSystemFont("", "", pContainer->getContentSize().height*0.5f);
		pLabelCost->setName(strKey);
		pLabelCost->setPosition(Point(pContainer->getContentSize().width*positionRate.x, pContainer->getContentSize().height*positionRate.y));
		pContainer->addChild(pLabelCost);
	}
	pLabelCost->setString(strValue);
	// 设置下最大的长度
	pLabelCost->setScale(1.0f);
	if (pLabelCost->getContentSize().width > pContainer->getContentSize().width*fScaleWidthRate)
	{
		pLabelCost->setScale((pContainer->getContentSize().width*fScaleWidthRate) / pLabelCost->getContentSize().width);
	}
}

void ControlUtil::setMenuItemLabel(Node* pContainer, const std::string& strBMFontPath, const std::string& strValue, const std::string& strKey, const Point& positionRate, float fScaleWidthRate, float fFontScale)
{
	if (!pContainer) return;

	Label* pLabelCost = dynamic_cast<Label*>(pContainer->getChildByName(strKey));
	if (!pLabelCost)
	{
		pLabelCost = Label::createWithBMFont(strBMFontPath, "");
		pLabelCost->setName(strKey);
		pLabelCost->setPosition(Point(pContainer->getContentSize().width*positionRate.x, pContainer->getContentSize().height*positionRate.y));
		pContainer->addChild(pLabelCost);
	}
	// 再设置一下
	pLabelCost->setBMFontFilePath(strBMFontPath);
	pLabelCost->setString(strValue);
	// 设置下最大的长度
	pLabelCost->setScale(fFontScale);
	if (pLabelCost->getBoundingBox().size.width > pContainer->getContentSize().width*fScaleWidthRate)
	{
		pLabelCost->setScale((pContainer->getContentSize().width*fScaleWidthRate) / pLabelCost->getContentSize().width);
	}
}

void ControlUtil::setMenuItemLabel(cocos2d::MenuItemImage *pMenuItem, const std::string &strBMFontPath, const std::string &strValue, const std::string &strKey, const Point& positionRate, float fScaleWidthRate, float fFontScale)
{
	setMenuItemLabel(pMenuItem->getNormalImage(), strBMFontPath, strValue, strKey, positionRate, fScaleWidthRate, fFontScale);
	setMenuItemLabel(pMenuItem->getSelectedImage(), strBMFontPath, strValue, strKey, positionRate, fScaleWidthRate, fFontScale);
}

void ControlUtil::addSpriteWithPostionAndAnchor(cocos2d::Node* pParent, const std::string& strFileName, const cocos2d::Vec2& position, const cocos2d::Vec2& anchor)
{
	do
	{
		BREAK_IF(!pParent);
		auto pSprite = Sprite::create(strFileName);
		BREAK_IF(!pSprite);
		pSprite->setPosition(position);
		pSprite->setAnchorPoint(anchor);
		pParent->addChild(pSprite);

	} while (0);
}

/**
 * @brief 在pNodeParent节点上加上圆圈简介,指定 theChild 在被剪切范围内.
 */
void ControlUtil::addRoundClipForNode(cocos2d::Node*pNodeParent, cocos2d::Node* theChild)
{
	if (pNodeParent == NULL || theChild == NULL)
	{
		return;
	}
	float maxWidth = MIN(pNodeParent->getContentSize().width - 2, pNodeParent->getContentSize().height - 2);

	ClippingNode* clipper = dynamic_cast<ClippingNode*>(theChild->getParent());
	if (clipper == NULL)//没有剪切过
	{
		clipper = ClippingNode::create();
		clipper->setContentSize(Size(maxWidth, maxWidth));
		clipper->setPosition(Vec2(1, 1));
		clipper->setAnchorPoint(Vec2(0, 0));



		DrawNode *shape = DrawNode::create();
		Size size = clipper->getContentSize();

		//绘制区域
		static Color4F green = { 1, 1, 1, 1 };//顶点颜色,这里我们没有实质上没有绘制,所以看不出颜色

		float fRadius = maxWidth / 2;
		const int nCount = 300;//圆形其实可以看做正多边形,
		const float coef = 2.0f * (float)M_PI / nCount;//计算每两个相邻顶点与中心的夹角
		static Point circle[nCount];//顶点数组
		for (unsigned int i = 0; i < nCount; i++) {
			float rads = i*coef;//弧度
			circle[i].x = maxWidth / 2 + fRadius * cosf(rads);//对应顶点的x
			circle[i].y = maxWidth / 2 + fRadius * sinf(rads);//对应顶点的y
		}

		shape->drawPolygon(circle, nCount, green, 0, green);
		clipper->setStencil(shape);

		pNodeParent->addChild(clipper);

		theChild->retain();
		theChild->autorelease();
		theChild->removeFromParent();

		clipper->addChild(theChild);

	}
}


bool ControlUtil::isVisible(cocos2d::Node* pNode)
{
	bool bRet = false;

	do
	{
		CC_BREAK_IF(!pNode || !pNode->isVisible() || !pNode->getParent());

		auto parent = pNode->getParent();
		for (auto c = parent; c != nullptr; c = c->getParent())
		{
			if (!c->isVisible())
			{
				return false;
			}
		}

		bRet = true;

	} while (0);

	return bRet;
}

Point ControlUtil::getPointWithRatePoint(const Node* pNodeTarget, const cocos2d::Point& ratePoint)
{
	Point pt = Point::ZERO;
	do
	{
		CC_BREAK_IF(!pNodeTarget || !pNodeTarget->getParent());
		// 得到锚点
		const Point& anchorPoint = pNodeTarget->getAnchorPoint();
		// 得到坐标
		const Point& pos = pNodeTarget->getPosition();
		// 得到长宽
		const Size& size = pNodeTarget->getContentSize();

		// pt = pos - Vec2(size.width*anchorPoint.x, size.height*anchorPoint.y) + Vec2(size.width*anchor.x, size.height*anchor.y);
		Point origin = pos - Point(anchorPoint.x * size.width, anchorPoint.y * size.height);
		pt = origin + Point(ratePoint.x * size.width, ratePoint.y * size.height);

	} while (0);

	return pt;
}

void ControlUtil::fitFontSize(cocos2d::Node *pRootNode)
{
	float fScale = CC_CONTENT_SCALE_FACTOR();

	for (auto& node : pRootNode->getChildren())
	{
		if (node->getChildrenCount() > 0)
		{
			fitFontSize(node);
		}
		else
		{
			Label* pLabel = dynamic_cast<Label*>(node);
			if (pLabel && pLabel->getBMFontFilePath().empty())
			{
				/*
				TTFConfig config = pLabel->getTTFConfig();
				config.fontSize = config.fontSize/fScale;
				pLabel->setTTFConfig(config);
				*/
				pLabel->setSystemFontSize(pLabel->getSystemFontSize() / fScale);
			}
		}
	}
}

void ControlUtil::shaderGray(cocos2d::Node *pNode, bool bCascaded)
{
	if (bCascaded)
	{
		for (auto child : pNode->getChildren())
		{
			shaderGray(child, true);
		}
	}

	if (pNode)
	{
		auto glProgram = GLProgram::createWithFilenames("shader/gray.vsh", "shader/gray.fsh");
		if (glProgram)
		{
			auto glProgramstate = GLProgramState::getOrCreateWithGLProgram(glProgram);
			if (glProgramstate) pNode->setGLProgramState(glProgramstate);
		}
	}
}

void ControlUtil::shaderOutline(cocos2d::Sprite *pSprite, Vec3 color /* = Vec3(1.0f, 1.0f, 1.0f) */, float fRadius /* = 0.01f */, float fThreshold /* = 1.75f */)
{
	if (pSprite)
	{
		std::string fragFilePath = FileUtils::getInstance()->fullPathForFilename("shader/outline.fsh");
		std::string fragSource = FileUtils::getInstance()->getStringFromFile(fragFilePath);
		auto glProgram = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, fragSource.c_str());
		if (glProgram)
		{
			auto glProgramstate = GLProgramState::getOrCreateWithGLProgram(glProgram);
			if (glProgramstate)
			{
				Vec3 color_s = Vec3(color.x, color.y, color.z); // (0, 1.0f, 0);
				GLfloat radius_s = fRadius; //0.01f;
				GLfloat threshold_s = fThreshold; // 1.75;

				glProgramstate->setUniformVec3("u_outlineColor", color_s);
				glProgramstate->setUniformFloat("u_radius", radius_s);
				glProgramstate->setUniformFloat("u_threshold", threshold_s);
				pSprite->setGLProgramState(glProgramstate);
			}
		}
	}
}

void ControlUtil::shaderDark(cocos2d::Node *pNode, bool bCascaded)
{
	if (bCascaded)
	{
		for (auto child : pNode->getChildren())
		{
			shaderDark(child, true);
		}
	}

	if (pNode)
	{
		auto glProgram = GLProgram::createWithFilenames("shader/dark.vsh", "shader/dark.fsh");
		if (glProgram)
		{
			auto glProgramstate = GLProgramState::getOrCreateWithGLProgram(glProgram);
			if (glProgramstate) pNode->setGLProgramState(glProgramstate);
		}
	}
}

void ControlUtil::shaderReset(cocos2d::Node *pNode, bool bCascaded)
{
	// 恢复成平常的特效
	if (bCascaded)
	{
		for (auto child : pNode->getChildren())
		{
			shaderReset(child, true);
		}
	}

	if (pNode)
	{
		pNode->setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));
	}
}

void ControlUtil::forceCloseDialog(cocos2d::Node *pNode)
{
	for (auto parent = pNode; parent != nullptr; parent = parent->getParent())
	{
		auto pDlg = dynamic_cast<bailin::ui::DialogLayer*>(parent);
		if (pDlg)
		{
			pDlg->closeDialog();
			break;
		}
	}
}

cocos2d::Node* ControlUtil::getNodeByName(cocos2d::Node* pRootNode, const std::string& strName)
{
	Node* pNode = nullptr;
	if (pRootNode)
	{
		for (auto pChild : pRootNode->getChildren())
		{
			if (pChild->getName() == strName)
			{
				pNode = pChild;
			}
			else if (pChild->getChildren().size() > 0)
			{
				pNode = getNodeByName(pChild, strName);
			}

			if (pNode) break;
		}
	}

	return pNode;
}

Color3B ControlUtil::getColor3BWithStringRGB(const std::string& strColor)
{
	Color3B color = Color3B::WHITE;

	if (strColor.length() >= 6)
	{
		color.r = strtoul(strColor.substr(0, 2).c_str(), nullptr, 16);
		color.g = strtoul(strColor.substr(2, 2).c_str(), nullptr, 16);
		color.b = strtoul(strColor.substr(4, 2).c_str(), nullptr, 16);
	}

	return color;
}

Color4B ControlUtil::getColor4BWithStringRGBA(const std::string &strColor)
{
	Color4B color = Color4B::WHITE;

	if (strColor.length() >= 6)
	{
		color.r = strtoul(strColor.substr(0, 2).c_str(), nullptr, 16);
		color.g = strtoul(strColor.substr(2, 2).c_str(), nullptr, 16);
		color.b = strtoul(strColor.substr(4, 2).c_str(), nullptr, 16);

		if (strColor.length() >= 8)
			color.a = strtoul(strColor.substr(6, 2).c_str(), nullptr, 16);
	}

	return color;
}

std::string ControlUtil::getStringRGBWithColor3B(const cocos2d::Color3B &color)
{
	std::string strColor = "";
	strColor.append(StringUtils::format("%02X", color.r));
	strColor.append(StringUtils::format("%02X", color.g));
	strColor.append(StringUtils::format("%02X", color.b));
	return strColor;
}

std::string ControlUtil::getStringRGBAWithColor4B(const cocos2d::Color4B &color)
{
	std::string strColor = "";
	strColor.append(StringUtils::format("%02X", color.r));
	strColor.append(StringUtils::format("%02X", color.g));
	strColor.append(StringUtils::format("%02X", color.b));
	strColor.append(StringUtils::format("%02X", color.a));
	return strColor;
}

cocos2d::Node* ControlUtil::getShowFullLabelContentNode(cocos2d::Label *pLabel, const cocos2d::Size &size)
{
	Node* pNode = Node::create();
	pNode->setContentSize(size);

	if (pLabel)
	{
		float fDelta = pLabel->getContentSize().width - size.width;
		if (fDelta > 0)
		{
			// 创建遮罩层
			auto pStencil = DrawNode::create();
			pStencil->drawSolidRect(Vec2::ZERO, size, Color4F::BLACK);
			auto pClipping = ClippingNode::create(pStencil);
			pClipping->setContentSize(size);

			// 设置待添加文字的从属关系
			if (pLabel->getParent())
			{
				pLabel->retain();
				pLabel->removeFromParent();
				pClipping->addChild(pLabel);
				pLabel->release();
			}
			else
			{
				pClipping->addChild(pLabel);
			}

			pLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
			// 初始化位置
			pLabel->setPosition(0, size.height*0.5f);

			float fSpeed = 20 / CC_CONTENT_SCALE_FACTOR();
			float t = fDelta / fSpeed;
			// 开始移动动作
			pLabel->runAction(RepeatForever::create(Sequence::create(MoveTo::create(t, Vec2(-fDelta, pLabel->getPositionY())),
				DelayTime::create(1.0f),
				MoveTo::create(t, Vec2(0, pLabel->getPositionY())),
				DelayTime::create(1.0f),
				NULL)));
			pNode->addChild(pClipping);
		}
		else
		{
			pLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
			// 初始化位置
			pLabel->setPosition(0, size.height*0.5f);
			pNode->addChild(pLabel);
		}
	}

	return pNode;
}

NS_BL_UTIL_END
