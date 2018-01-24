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
		// 八边形
		Type_Octagon,
		// 三角形 
		Type_Triangle,
		// 黑洞
		Type_BlackHole
	};
	// 块块生命值
	int m_nHP;
	// 块块属性
	Type m_eType;
};
#endif // !__BLOCK_DATA_H__
