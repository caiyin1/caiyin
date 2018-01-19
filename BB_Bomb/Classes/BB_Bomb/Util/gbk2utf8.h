#ifndef __GBK2UTF8_UTILS__
#define __GBK2UTF8_UTILS__

#   ifdef WIN32
#       include <string>
//字符转换，使cocos2d-x在win32平台支持中文显示
int GBKToUTF8(std::string &gbkStr, const char* toCode, const char* formCode);
/**
**在封装一层，直接传入一个string，转换后还回对应的编码给你
*/
// const char* GBKToUTF(std::string &gbkStr);
std::string GBKToUTF(const std::string& strGBK);
#       define bailinText(param) (GBKToUTF(param).c_str())
#   else
#       define bailinText(param) param
#   endif

#endif // __GBK2UTF8_UTILS__
