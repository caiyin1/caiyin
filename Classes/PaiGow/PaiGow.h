#ifndef _PAIGOW_H_
#define _PAIGOW_H_
#include "cocos2d.h"

class PaiGow : public cocos2d::Node
{
public:
	// pai dao data
	class Dot {
	public:
		// X axis rate
		float m_fXRate;
		// Y axis reate
		float m_fYRate;
		// color£¬0 - white£¬1 - red
		int m_nColor;

		Dot(float fXRate, float fYRate, int nColor)
		{
			m_fXRate = fXRate;
			m_fYRate = fYRate;
			m_nColor = nColor;
		}
	};

	// create PaiGows object
	static PaiGow* create(int nNum, bool bDirection);
	/**
	 * @breif create PaiGows object 
	 * @param strBackgroundImage  Pai Back ground Image  file position
	 * @param strDatRedImage  pai red dot  Image file position
	 * @param strDotwhiteImage pai white dot Image file Position
	 * @param pai Id
	 * @return PaiGows 
	 */
	static PaiGow* create(const std::string& strBackgroundImage, const std::string& strDotRedImage, const std::string& strDotWhiteImage, int nNum, bool bDirecton);

	static void setImages(const std::string& strBackgroundImage, const std::string& strDotRedImage, const std::string& strDotWhiteImage);
   /**
	 * @breif set pai type
	 * @param nNum pai Id
	 * @param bDirection pai bDirection trun is vertical ,false is across
	 */ 

	void setSurface(int nNum, bool bDirection);
	
private:
	// init data fun
	/**
	 * @brief pai background Image and dot Image file Position add PaiGows 
	 * @param strBackgroundImage  Pai Back ground Image  file position
	 * @param strDatRedImage  pai red dot  Image file position
	 * @param strDotwhiteImage pai white dot Image file Position
	 * @return bool xxxxxx
	 */
	bool initWithData(const std::string& strBackgroundImage, const std::string& strDotRedImage, const std::string& strDotWhiteImage, int nNum);

	bool initWithData(int nNum);

private:
	// Back ground Image file position
	static std::string s_strBackgroundImage;
	// red dot image file position
	static std::string s_strDotRedImage;
	// white dot file position
	static std::string s_strDotWhiteImage;

private:
	//Value data;

	// pai name
	std::string m_strName;
	// pai dot information
	std::vector<PaiGow::Dot> m_vPoint;
	// do num
	int m_nNum;

private:
	// pai backGround Image 
	cocos2d::Sprite* m_pSpriteBg = nullptr;
	// pai high Light Image position
	std::string m_strHighLightImage = "PaiGow/White.png";
};



#endif //_PAIGOW_H_