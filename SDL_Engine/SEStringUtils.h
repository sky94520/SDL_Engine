#ifndef __SDL_StringUtils_H__
#define __SDL_StringUtils_H__

#include<cstdarg>
#include<string>
#include<cstring>
#include<sstream>
#include<functional>

#include "SDL.h"

#include "SEPlatformMarcos.h"
#include "SEValue.h"

NS_SDL_BEGIN

//用于获取UTF8下个字符的编码偏移。  
static const char utf8_skip_data[256] = {  
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  
    1, 1, 1, 1, 1, 1, 1,  
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  
    1, 1, 1, 1, 1, 1, 1,  
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  
    1, 1, 1, 1, 1, 1, 1,  
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  
    1, 1, 1, 1, 1, 1, 1,  
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  
    1, 1, 1, 1, 1, 1, 1,  
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  
    1, 1, 1, 1, 1, 1, 1,  
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,  
    2, 2, 2, 2, 2, 2, 2,  
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5,  
    5, 5, 5, 6, 6, 1, 1  
};  

static const char *const g_utf8_skip = utf8_skip_data;
//取得UTP8编码字符的下一个字符。
#define cc_utf8_next_char(p) (char *)((p) + g_utf8_skip[*(unsigned char *)(p)])

namespace StringUtils
{
std::string format(const char*format,...);
//拆分字符串
ValueVector split(const std::string&src,const std::string&token);
ValueVector split(const std::string&src,char ch);
void split(const std::string& src,const std::string& token,const std::function<void (int,const Value&)>& callback);

template<typename T>
std::string toString(T t)
{
	std::stringstream ss;
	ss<<t;
	return ss.str();
}
//返回取得字符串转换为UTF8后的字符长度
long cc_utf8_strlen (const char * p, int max);
//将Unicode编码字符串p转换为UTF8编码
unsigned int cc_utf8_get_char (const char * p);
//可以使用std::u16str length()代替
int cc_wcslen(const unsigned short* str);
//end the StringUtils
}
NS_SDL_END
#endif
