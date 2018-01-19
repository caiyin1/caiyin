#pragma once
#include "cocos2d.h"

namespace blutil {
	void RGB2HSL(const cocos2d::Color3B& color, double &H, double &S, double &L);
	cocos2d::Color3B HSL2RGB(double H, double S, double L);
}
