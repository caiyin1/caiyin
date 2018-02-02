#ifndef __ACTION_FACTORY_NODE__
#define __ACTION_FACTORY_NODE__
#include "cocos2d.h"

class ActionFactoryNode : public cocos2d::Node
{
public:
	static ActionFactoryNode* create(int nNum);
	bool initWith(int nNum);
private:
	/*
	* @brief 获取食物消失动画得分加一
	*/
	cocos2d::Action* getScorePlusAction(int nNum);


};

#endif // !__ACTION_FACTORY_NODE__
