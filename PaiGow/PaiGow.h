#ifndef _PAIGOW_H_
#define _PAIGOW_H_
#include "cocos2d.h"

class PaiGows : public cocos2d::Node
{
public:
	// ���ϵĵ������
	class Dot {
	public:
		// X ��ٷֱ�
		float m_fXRate;
		// Y ��ٷֱ�
		float m_fYRate;
		// ɫ�ʣ�0 - ��ɫ��1 - ��ɫ
		int m_nColor;

		Dot(float fXRate, float fYRate, int nColor)
		{
			m_fXRate = fXRate;
			m_fYRate = fYRate;
			m_nColor = nColor;
		}
	};

	// �����ƾŵ��ƶ���
	static PaiGows* create(int nNum);
	/**
	 * @breif xxxx
	 * @param strxxx
	 */
	static PaiGows* create(const std::string& strBackgroundImage, const std::string& strDotRedImage, const std::string& strDotWhiteImage, int nNum);

	static void setImages(const std::string& strBackgroundImage, const std::string& strDotRedImage, const std::string& strDotWhiteImage);
	// ����������

	void setSurface(int nNum);
	
private:
	// ��ʼ�����ݺ���
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
	// �Ʊ���ͼƬ��ַ
	static std::string s_strBackgroundImage;
	// �Ƶĺ���ͼƬ
	static std::string s_strDotRedImage;
	// �Ƶİ׵��ͼƬ
	static std::string s_strDotWhiteImage;

private:
	//Value data;

	// �Ƶ�����
	std::string m_strName;
	// �Ƶĵ������
	std::vector<PaiGows::Dot> m_vPoint;
	// ����
	int m_nNum;

private:
	// �Ƶı���ͼƬ
	cocos2d::Sprite* m_pSpriteBg = nullptr;
};



#endif //_PAIGOW_H_