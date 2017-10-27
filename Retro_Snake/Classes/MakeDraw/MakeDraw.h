#ifndef __MAKEDRAW_H__
#define __MAKEDRAW_H__
#include "cocos2d.h"

class MakeDraw : public cocos2d::DrawNode
{
public:
   /**
	* @brief ���������� Բ�� ������
	* @parem nNum 1 ��ʾ ��ͷ�����Σ� 2 ��ʾ���� �����Σ� 3 ��ʾ��� Բ��
	*/
	static MakeDraw* create(int nNum);

	bool init(int nNum);

};

#endif //__MAKEDRAW_H__