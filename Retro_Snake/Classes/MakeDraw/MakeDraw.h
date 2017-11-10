#ifndef __MAKEDRAW_H__
#define __MAKEDRAW_H__
#include "cocos2d.h"
USING_NS_CC;

class MakeDraw : public cocos2d::DrawNode
{
public:
   /**
	* @brief 绘制三角形 圆形 正方形
	* @parem nNum 1 表示 蛇头三角形， 2 表示蛇身 正方形， 3 表示金币 圆形
	*/
	static MakeDraw* create(int nNum);
	static MakeDraw* create(int nNum, Color4B color);
	bool init(int nNum);
	bool init(int nNum, Color4B color);

};

#endif //__MAKEDRAW_H__