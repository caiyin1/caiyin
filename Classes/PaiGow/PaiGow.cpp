#include "PaiGow.h"
#include <string>
#include <iostream>
using namespace std;
using namespace cocos2d;


#ifndef MakeDot
#define MakeDot(x, y, c)  new PaiGow::Dot(x, y, c)
#endif //MakeDot

string PaiGow::s_strBackgroundImage = "";
string PaiGow::s_strDotRedImage = "";
string PaiGow::s_strDotWhiteImage = "";

//global value£¬set pai  information
const static std::unordered_map<int, std::vector<PaiGow::Dot* >* > g_s_paigow_Data{
	//tian 
	std::make_pair(0, new std::vector<PaiGow::Dot* >{
		MakeDot(0.25f, 0.10f, 0), MakeDot(0.75f, 0.10f, 1),
		MakeDot(0.25f, 0.26f, 0), MakeDot(0.75f, 0.26f, 1),
		MakeDot(0.25f, 0.42f, 0), MakeDot(0.75f, 0.42f, 1),
		MakeDot(0.25f, 0.58f, 1), MakeDot(0.75f, 0.58f, 0),
		MakeDot(0.25f, 0.74f, 1), MakeDot(0.75f, 0.74f, 0),
		MakeDot(0.25f, 0.90f, 1), MakeDot(0.75f, 0.90f, 0)
	}),
	//di
	std::make_pair(1, new std::vector<PaiGow::Dot*>{
		MakeDot(0.50f, 0.90f, 1),
		MakeDot(0.50f, 0.10f, 1)
	}),
	//people
	std::make_pair(2, new std::vector<PaiGow::Dot*>{
			MakeDot(0.25f, 0.88f, 1), MakeDot(0.75f, 0.88f, 1),
			MakeDot(0.25f, 0.62f, 1), MakeDot(0.75f, 0.62f, 1),
			MakeDot(0.25f, 0.37f, 1), MakeDot(0.75f, 0.37f, 1),
			MakeDot(0.25f, 0.11f, 1), MakeDot(0.75f, 0.11f, 1)
	}),
	//goose
	std::make_pair(3, new std::vector<PaiGow::Dot*>{
				MakeDot(0.75f, 0.90f, 0),
				MakeDot(0.50f, 0.74f, 0),
				MakeDot(0.25f, 0.58f, 0),
				MakeDot(0.50f, 0.10f, 1)
		}),
	//mei
	std::make_pair(4, new std::vector<PaiGow::Dot*>{
		MakeDot(0.25f, 0.90f, 0), MakeDot(0.75f, 0.90f, 0),
		MakeDot(0.50f, 0.74f, 0),
		MakeDot(0.25f, 0.58f, 0), MakeDot(0.75f, 0.58f, 0),
		MakeDot(0.25f, 0.42f, 0), MakeDot(0.75f, 0.42f, 0),
		MakeDot(0.50f, 0.26f, 0),
		MakeDot(0.25f, 0.10f, 0), MakeDot(0.75f, 0.10f, 0)
	}),
	//long  three
	std::make_pair(5, new std::vector<PaiGow::Dot*>{
		MakeDot(0.50f, 0.90f, 0),
		MakeDot(0.50f, 0.70f, 0),
		MakeDot(0.25f, 0.50f, 0), MakeDot(0.75f, 0.50f, 0),
		MakeDot(0.50f, 0.30f, 0),
		MakeDot(0.50f, 0.10f, 0)
	}),
	
	//bench
	std::make_pair(6, new std::vector<PaiGow::Dot*>{
		MakeDot(0.25f, 0.90f, 0), MakeDot(0.75f, 0.90f, 0),
		MakeDot(0.25f, 0.10f, 0), MakeDot(0.75f, 0.10f, 0)
	}),
	
	//Axc
	std::make_pair(7, new std::vector<PaiGow::Dot*>{
		MakeDot(0.25f, 0.90f, 0), MakeDot(0.75f, 0.90f, 0),
		MakeDot(0.50f, 0.74f, 0),
		MakeDot(0.25f, 0.58f, 0), MakeDot(0.75f, 0.58f, 0),
		MakeDot(0.25f, 0.42f, 0), MakeDot(0.75f, 0.42f, 0),
		MakeDot(0.25f, 0.26f, 0), MakeDot(0.75f, 0.26f, 0),
		MakeDot(0.25f, 0.10f, 0), MakeDot(0.75f, 0.10f, 0)
	}),

	//red head ten
	std::make_pair(8, new std::vector<PaiGow::Dot*>{
		MakeDot(0.50f, 0.90f, 1), 	
		MakeDot(0.25f, 0.42f, 0), MakeDot(0.75f, 0.42f, 0),
		MakeDot(0.25f, 0.26f, 0), MakeDot(0.75f ,0.26f, 0),
		MakeDot(0.25f, 0.10f, 0), MakeDot(0.75f, 0.10f, 0)
	}),

	//heigh foot seven
	std::make_pair(9, new std::vector<PaiGow::Dot*>{
		MakeDot(0.25f, 0.90f, 1), MakeDot(0.75f, 0.90f, 1),
		MakeDot(0.25f, 0.74f, 1), MakeDot(0.75f, 0.74f, 1),
		MakeDot(0.25f, 0.42f, 0), MakeDot(0.75f, 0.42f, 0),
		MakeDot(0.25f, 0.26f, 0), MakeDot(0.75f, 0.26f, 0),
		MakeDot(0.25f, 0.10f, 0), MakeDot(0.75f, 0.10f, 0)
	}),

	//Pratia six
	std::make_pair(10, new std::vector<PaiGow::Dot*>{
		MakeDot(0.50f, 0.90f, 1),
		MakeDot(0.25f, 0.42f, 0), MakeDot(0.75f, 0.42f, 0),
		MakeDot(0.50f, 0.26f, 0),
		MakeDot(0.25f, 0.10f, 0), MakeDot(0.75f, 0.10f, 0)
	}),

	//nine frist
	std::make_pair(11, new std::vector<PaiGow::Dot*>{
		MakeDot(0.75f, 0.90f, 0),
		MakeDot(0.50f, 0.74f, 0),
		MakeDot(0.25f, 0.58f, 0),
		MakeDot(0.25f, 0.42f, 0), MakeDot(0.75f, 0.42f, 0),
		MakeDot(0.25f, 0.26f, 0), MakeDot(0.75f, 0.26f, 0),
		MakeDot(0.25f, 0.10f, 0), MakeDot(0.75f, 0.10f, 0)
	}),

	//nine Second
	std::make_pair(12, new std::vector<PaiGow::Dot*>{
		MakeDot(0.25f, 0.90f, 1), MakeDot(0.75f, 0.90f, 1),
		MakeDot(0.25f, 0.74f, 1), MakeDot(0.75f, 0.74f, 1),
		MakeDot(0.25f, 0.42f, 0), MakeDot(0.75f, 0.42f, 0),
		MakeDot(0.50f, 0.26f, 0),
		MakeDot(0.25f, 0.10f, 0), MakeDot(0.75f, 0.10f, 0)
	}),
	
	//eight frist
	std::make_pair(13, new std::vector<PaiGow::Dot*>{
		MakeDot(0.75f, 0.90f, 0),
		MakeDot(0.50f, 0.74f, 0),
		MakeDot(0.25f, 0.58f, 0),
		MakeDot(0.25f, 0.42f, 0), MakeDot(0.75f, 0.42f, 0),
		MakeDot(0.50f, 0.26f, 0),
		MakeDot(0.25f, 0.10f, 0), MakeDot(0.75f, 0.10f, 0)
	}),

	//eight Second 
	std::make_pair(14, new std::vector<PaiGow::Dot*>{
		MakeDot(0.25f, 0.90f, 0), MakeDot(0.75f, 0.90f, 0),
		MakeDot(0.25f, 0.42f, 0), MakeDot(0.75f, 0.42f, 0),
		MakeDot(0.25f, 0.26f, 0), MakeDot(0.75f, 0.26f, 0),
		MakeDot(0.25f, 0.10f, 0), MakeDot(0.75f, 0.10f, 0)
	}),
	
	//seven frist 
	std::make_pair(15, new std::vector<PaiGow::Dot*>{
		MakeDot(0.75f, 0.90f, 0),
		MakeDot(0.50f, 0.74f, 0),
		MakeDot(0.25f, 0.58f, 0),
		MakeDot(0.25f, 0.26f, 1), MakeDot(0.75f, 0.26f, 1),
		MakeDot(0.25f, 0.10f, 1), MakeDot(0.75f, 0.10f, 1)
	
	}),

	//seven Second
	std::make_pair(16, new std::vector<PaiGow::Dot*>{
		MakeDot(0.25f, 0.90f, 0), MakeDot(0.75f, 0.90f, 0),
		MakeDot(0.25f, 0.42f, 0), MakeDot(0.75f, 0.42f, 0),
		MakeDot(0.50f, 0.26f, 0),
		MakeDot(0.25f, 0.10f, 0), MakeDot(0.75f, 0.10f, 0)
	}),

	//six
	std::make_pair(17, new std::vector<PaiGow::Dot*>{
		MakeDot(0.25f, 0.90f, 0), MakeDot(0.75f, 0.90f, 0),
		MakeDot(0.25f, 0.26f, 1), MakeDot(0.75f, 0.26f, 1),
		MakeDot(0.25f, 0.10f, 1), MakeDot(0.75f, 0.10f, 1)
	}),

	//five frist
	std::make_pair(18, new std::vector<PaiGow::Dot*>{
		MakeDot(0.50f, 0.90f, 1),
		MakeDot(0.25f, 0.26f, 1), MakeDot(0.75f, 0.26f, 1),
		MakeDot(0.25f, 0.10f, 1), MakeDot(0.75f, 0.10f, 1)
	}),

	//five Second
	std::make_pair(19, new std::vector<PaiGow::Dot*>{
		MakeDot(0.75f, 0.90f, 0),
		MakeDot(0.50f, 0.74f, 0),
		MakeDot(0.25f, 0.58f, 0),
		MakeDot(0.25f, 0.10f, 0), MakeDot(0.75f, 0.10f, 0)
	}),

	//Three
	std::make_pair(20, new std::vector<PaiGow::Dot*>{
		MakeDot(0.50f, 0.90f, 1),
		MakeDot(0.25f, 0.10f, 0), MakeDot(0.75f, 0.10f, 0)
	})
};

static bool addPoint(Node* parent, int nNum, const std::string& strDotRed, const std::string& strDotWhite)
{
	bool bRet = true;
	auto iter = g_s_paigow_Data.find(nNum);
	if (iter == g_s_paigow_Data.end()) return false;
	do
	{
		//get BackGround size
		auto parentSize = parent->getContentSize();
		for (auto pDotData : *iter->second)
		{
			auto pDotNode = Sprite::create(pDotData->m_nColor == 0 ? strDotWhite : strDotRed);
			if (!pDotData)
			{
				return false;
			}
			pDotNode->setPosition(pDotData->m_fXRate * parentSize.width, pDotData->m_fYRate * parentSize.height);
			parent->addChild(pDotNode);
		}

	} while (0);
	return bRet;

}



PaiGow* PaiGow::create(int nNum, bool bVertical)
{
	//return create(s_strBackgroundImage, s_strDotRedImage, s_strDotWhiteImage, nNum, bVertical);
	PaiGow* pRet = new PaiGow;
	if (pRet && pRet->initWithData(nNum, bVertical))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

PaiGow* PaiGow::create(const std::string& strBackgroundImage, const std::string& strDotRedImage, const std::string& strDotWhiteImage, int nNum, bool bVertical)
{
	setImages(strBackgroundImage, strDotRedImage, strDotWhiteImage);
	PaiGow* pRet = new PaiGow;
	if (pRet && pRet->initWithData(s_strBackgroundImage, s_strDotRedImage, s_strDotWhiteImage, nNum, bVertical))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}
	
	return pRet;
}



bool PaiGow::initWithData(const std::string& strBackgroundImage, const std::string& strDotRedImage, const std::string& strDotWhiteImage, int nNum, bool bVertical)
{
	bool bRet = false;
	do
	{
		m_pSpriteBg = Sprite::create(strBackgroundImage);
		CC_BREAK_IF(!m_pSpriteBg);

		auto BgSize = m_pSpriteBg->getContentSize();
		setContentSize(BgSize);
		//set Anchor
		setAnchorPoint(Vec2(0.5, 0.5));
		m_pSpriteBg->setPosition(_contentSize.width * 0.5, _contentSize.height * 0.5);
		addChild(m_pSpriteBg);
		// add dot information
		bRet = addPoint(m_pSpriteBg, nNum, strDotRedImage, strDotWhiteImage);
		setDirection(bVertical);
	} while (0);
	return bRet;
}

bool PaiGow::initWithData(int nNum, bool bVertical)
{
	bool bRet = false;
	do
	{
		m_pSpriteBg = Sprite::create(s_strBackgroundImage);
		CC_BREAK_IF(!m_pSpriteBg);

		auto BgSize = m_pSpriteBg->getContentSize();
		setContentSize(BgSize);
		//set Anchor
			setAnchorPoint(Vec2(0.5, 0.5));
		m_pSpriteBg->setPosition(_contentSize.width * 0.5, _contentSize.height * 0.5);
		addChild(m_pSpriteBg);
		//add dot information
		bRet = addPoint(m_pSpriteBg, nNum, s_strDotRedImage, s_strDotWhiteImage);
		setDirection(bVertical);
	} while (0);
	return bRet;

}

void PaiGow::setSurface(int nNum, bool bVertical)
{
	do
	{
		CC_BREAK_IF(!m_pSpriteBg);
		m_pSpriteBg->removeAllChildren();
		addPoint(m_pSpriteBg, nNum, s_strDotRedImage, s_strDotWhiteImage);
		setDirection(bVertical);
		
	} while (0);
		
}


void PaiGow::setImages(const std::string& strBackgroundImage, const std::string& strDotRedImage, const std::string& strDotWhiteImage)
{
	s_strBackgroundImage = strBackgroundImage;

	s_strDotRedImage = strDotRedImage;

	s_strDotWhiteImage = strDotWhiteImage;
}

void PaiGow::setHighLight(const std::string& stringHighLightImage)
{
	if (_contentSize.height > _contentSize.width)
	{
		auto highLightLeft = Sprite::create(stringHighLightImage);
		auto highLightRight = Sprite::create(stringHighLightImage);
		if (!highLightLeft && !highLightRight)
		{
			return;
		}
		highLightLeft->setScale(0.2f);
		
		highLightLeft->setPosition(20, m_pSpriteBg->getContentSize().height - 20);
		m_pSpriteBg->addChild(highLightLeft);

		highLightRight->setScale(0.2f);

		highLightRight->setPosition(m_pSpriteBg->getContentSize().width - 20, m_pSpriteBg->getContentSize().height - 20);
		m_pSpriteBg->addChild(highLightRight);

		
	}
	else
	{
		
		auto highLightLeft = Sprite::create(stringHighLightImage);
		auto highLightRight = Sprite::create(stringHighLightImage);
		if (!highLightLeft && !highLightRight)
		{
			return;
		}
		highLightLeft->setScale(0.2f);

		highLightLeft->setPosition(20, m_pSpriteBg->getContentSize().height - 20);
		m_pSpriteBg->addChild(highLightLeft);

		highLightRight->setScale(0.2f);

		highLightRight->setPosition(20, 20);
		m_pSpriteBg->addChild(highLightRight);
	}

}

void PaiGow::setDirection(bool bVertical /*= true*/)
{
	if (bVertical)
	{
		m_pSpriteBg->setRotation(0);
		setContentSize(m_pSpriteBg->getContentSize());
		setAnchorPoint(Vec2::ANCHOR_MIDDLE);

		m_pSpriteBg->setPosition(_contentSize * 0.5f);
	}
	else
	{
		m_pSpriteBg->setRotation(90);
		setContentSize(Size(m_pSpriteBg->getContentSize().height, m_pSpriteBg->getContentSize().width));
		setAnchorPoint(Vec2::ANCHOR_MIDDLE);

		m_pSpriteBg->setPosition(_contentSize * 0.5f);
	}
}
