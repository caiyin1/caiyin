#include "cocos2d.h"

class Snake : public cocos2d::DrawNode
{
public:
   /**
	* @brief 绘制三角形 圆形 正方形
	* @parem nNum 1 表示 蛇头三角形， 2 表示蛇身 正方形， 3 表示金币 圆形
	*/
	static Snake* create(int nNum);

	bool init(int nNum);
private:



};