#ifndef _PAIGOW_H_
#define _PAIGOW_H_
#include "cocos2d.h"

class PaiGow : public cocos2d::Node
{
public:
	// Dot data
	class Dot {
	public:
		// X axis rate
		float m_fXRate;
		// Y axis rate
		float m_fYRate;
		// color, 0 - white, 1 - red
		int m_nColor;

		Dot(float fXRate, float fYRate, int nColor)
		{
			m_fXRate = fXRate;
			m_fYRate = fYRate;
			m_nColor = nColor;
		}
	};

	// create PaiGows object
	static PaiGow* create(int nNum, bool bVertical = true);
	/**
	 * @brief create PaiGows object 
	 * @param strBackgroundImage  Pai Back ground Image  file position
	 * @param strDatRedImage  pai red dot  Image file position
	 * @param strDotwhiteImage pai white dot Image file Position
	 * @param pai Id
	 * @return PaiGows 
	 */
	static PaiGow* create(const std::string& strBackgroundImage, const std::string& strDotRedImage, const std::string& strDotWhiteImage, int nNum, bool bVertical = true);
	/**
	* @brief create PaiGows object
	* @param strBackgroundImage  Pai Back ground Image  file position
	* @param strDatRedImage  pai red dot  Image file position
	* @param strDotwhiteImage pai white dot Image file Position
	* @param pai Id
	* @return PaiGows
	*/
	//static PaiGow* create(const std::string& strBackgroundImage, const std::string& strDotRedImage, const std::string& strDotWhiteImage, const std::string& strhighLight, int nNum, bool bDirecton = true);

	static void setImages(const std::string& strBackgroundImage, const std::string& strDotRedImage, const std::string& strDotWhiteImage);
   /**
	 * @brief set pai type
	 * @param nNum pai Id
	 * @param bVertical pai bDirection turn is vertical ,false is Horizontal
	 */ 

	void setSurface(int nNum, bool bVertical = true);

	void setHighLight(const std::string& stringHighLightImage);
	
private:
	// init data fun
	/**
	 * @brief pai background Image and dot Image file Position add PaiGows 
	 * @param strBackgroundImage  Pai Back ground Image  file position
	 * @param strDatRedImage  pai red dot  Image file position
	 * @param strDotwhiteImage pai white dot Image file Position
	 * @return bool xxxxxx
	 */
	bool initWithData(const std::string& strBackgroundImage, const std::string& strDotRedImage, const std::string& strDotWhiteImage, int nNum, bool bVertical);

	bool initWithData(int nNum, bool bVertical);

	void setDirection(bool bVertical);

private:
	// Back ground Image file position
	static std::string s_strBackgroundImage;
	// red dot image file position
	static std::string s_strDotRedImage;
	// white dot file position
	static std::string s_strDotWhiteImage;
	// high light Image Position
	static std::string s_strHighLightImage;
private:
	//Value data;

	// pai name
	std::string m_strName;
	// pai dot information
	std::vector<PaiGow::Dot> m_vPoint;
	// do num
	int m_nNum;

private:
	// pai background Image 
	cocos2d::Sprite* m_pSpriteBg = nullptr;
};



#endif //_PAIGOW_H_