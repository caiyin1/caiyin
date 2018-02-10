/*
 * =====================================================================================
 *
 *       Filename:  bailinMacro.h
 *
 *    Description:  百灵的一些公共宏定义
 *					这里必须支持C++11标准
 *
 *        Version:  1.0
 *        Created:  01/03/2015 15:28:17
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jason Tou (), sosoayaen@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#pragma once

#ifdef WIN32
#	include "cocos2d.h"
#endif

// 百灵命名空间
#define NS_BL_BEGIN namespace bailin {
#define NS_BL_END	}
#define USING_NS_BL using namespace bailin

// 定义百灵工具命名空间
#define NS_BL_UTIL_BEGIN namespace bailin { namespace util {
#define NS_BL_UTIL_END }}
#define USING_NS_BL_UTIL using namespace bailin::util

// 定义UI相关的命名空间
#define NS_BL_UI_BEGIN namespace bailin { namespace ui {
#define NS_BL_UI_END }}
#define USING_NS_BL_UI using namespace bailin::ui

// 定义新手引导相关命名空间
#define NS_BL_GUIDE_BEGIN namespace bailin { namespace guide {
#define NS_BL_GUIDE_END }}
#define USING_NS_BL_GUIDE using namespace bailin::guide

#define BREAK_IF(cond) if (cond) break
#define CONTINUE_IF(cond) if (cond) continue
#define RETURN_IF(cond) if (cond) return

// 清除 std 的一些容器
#define CLEAR_REF_ARRAY(container) for (auto pData : container) \
    {\
        if (pData) {delete pData; } \
    }\
    container.clear()

#define CLEAR_REF_MAP(map) for (auto iter : map) \
    {\
        if (iter.second) {delete iter.second; } \
    }\
    map.clear()
