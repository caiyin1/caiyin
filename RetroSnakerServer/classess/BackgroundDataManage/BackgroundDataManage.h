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
	* @breif �������
	*/
	void initBlock();
	/*
	* @breif ��ʼ��Food
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
	// ����һ��Layer ����һ������
	int m_nAllBlockNum;
	// ���Ʋ����������� ����Block  
	int m_nControlBlock;
};
#endif  //__BACKGROUND_DATA_MAMNAGE_H__