#ifndef __MAKEDRAW_H__
#define __MAKEDRAW_H__
#include "cocos2d.h"
USING_NS_CC;

class MakeDraw : public cocos2d::DrawNode
{
public:
   /**
	* @brief ���������� Բ�� ������
	* @parem nNum 1 ��ʾ ��ͷ�����Σ� 2 ��ʾ���� �����Σ� 3 ��ʾ��� Բ��
	*/
	static MakeDraw* create(int nNum);
	static MakeDraw* create(int nNum, Color4B color);
	bool init(int nNum);
	bool init(int nNum, Color4B color);

};

#endif //__MAKEDRAW_H__