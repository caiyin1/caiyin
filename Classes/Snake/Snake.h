#include "cocos2d.h"

class Snake : public cocos2d::DrawNode
{
public:
   /**
	* @brief ���������� Բ�� ������
	* @parem nNum 1 ��ʾ ��ͷ�����Σ� 2 ��ʾ���� �����Σ� 3 ��ʾ��� Բ��
	*/
	static Snake* create(int nNum);

	bool init(int nNum);
private:



};