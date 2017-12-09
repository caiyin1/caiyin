#ifndef __BACKGROUND_DATA_MAMNAGE_H__
#define __BACKGROUND_DATA_MAMNAGE_H__



class BackgroundDataManage
{
public:
	BackgroundDataManage();
	init();
protected:
private:
	/*
	* @breif 创建块块
	*/
	void initBlock();
	/*
	* @breif 初始化Food
	*/
	void initFood();
	/*
	* @breif
	*/
private:
	enum class Type_Block
	{
		Type_OneBlock,
		Type_TowBlock,
		Type_AllBlock
	};
private:
	int m_nConten[8][5];
	// 控制一个Layer 最多就一行铺满
	int m_nAllBlockNum;
	// 控制不会连续两行 都有Block  
	int m_nControlBlock;
};
#endif  //__BACKGROUND_DATA_MAMNAGE_H__