#ifndef _PAIGOW_H_
#define _PAIGOW_H_
#include "cocos2d.h"

class PaiGows : public cocos2d::Node
{
public:
	// 牌上的点的数据
	class Dot {
	public:
		// X 轴百分比
		float m_fXRate;
		// Y 轴百分比
		float m_fYRate;
		// 色彩，0 - 白色，1 - 红色
		int m_nColor;

		Dot(float fXRate, float fYRate, int nColor)
		{
			m_fXRate = fXRate;
			m_fYRate = fYRate;
			m_nColor = nColor;
		}
	};

	// 创建牌九的牌对象
	static PaiGows* create(int nNum);
	/**
	 * @breif xxxx
	 * @param strxxx
	 */
	static PaiGows* create(const std::string& strBackgroundImage, const std::string& strDotRedImage, const std::string& strDotWhiteImage, int nNum);

	static void setImages(const std::string& strBackgroundImage, const std::string& strDotRedImage, const std::string& strDotWhiteImage);
	// 设置牌内容

	void setSurface(int nNum);
	
private:
	// 初始化数据函数
	/**
	 * @brief xxxxxx
	 * @param strBackgroundImage xxxxx
	 * @param strDotRedImage xxxx
	 * @param strDotWhiteImage xxx
	 * @return bool xxxxxx
	 */
	bool initWithData(const std::string& strBackgroundImage, const std::string& strDotRedImage, const std::string& strDotWhiteImage, int nNum);

	bool initWithData(int nNum);

private:
	// 牌背的图片地址
	static std::string s_strBackgroundImage;
	// 牌的红点的图片
	static std::string s_strDotRedImage;
	// 牌的白点的图片
	static std::string s_strDotWhiteImage;

private:
	//Value data;

	// 牌的名称
	std::string m_strName;
	// 牌的点的数据
	std::vector<PaiGows::Dot> m_vPoint;
	// 点数
	int m_nNum;

private:
	// 牌的背景图片
	cocos2d::Sprite* m_pSpriteBg = nullptr;
};



#endif //_PAIGOW_H_