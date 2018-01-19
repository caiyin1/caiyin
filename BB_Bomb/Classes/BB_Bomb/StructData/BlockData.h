#ifndef __BLOCK_DATA_H__
#define __BLOCK_DATA_H__

class BlockData
{
public:
	// 块块属性
	enum class Type
	{
		// 正常
		Type_Nome,
		// 左半边
		Type_LeftHalf,
		// 右半边
		Type_Triangle
	};
	// 块块生命值
	int m_nHP;
	// 块块属性
	Type m_eType;
};
#endif // !__BLOCK_DATA_H__
