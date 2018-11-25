#include "SEStringUtils.h"
NS_SDL_BEGIN
namespace StringUtils
{
std::string format(const char*format,...)
{
	std::string ret;
	va_list args;
	va_start(args,format);
	char*buf = (char*)malloc(1024*100);
	if(buf)
	{
		SDL_vsnprintf(buf,1024*100,format,args);
		ret = buf;
		free(buf);
	}
	va_end(args);
	return ret;
}
ValueVector split(const std::string&src,const std::string&token)
{
	ValueVector vect;
/*	std::string str(srcStr);
	ValueVector stringList;
	int size = str.size();

	int startIndex = 0;
	int endIndex = 0;
	endIndex = str.find(sSep);

	std::string lineStr;
	while(endIndex > 0)
	{
		lineStr = str.substr(startIndex,endIndex);
		Value value(lineStr);
		stringList.push_back(value);
		str = str.substr(endIndex+1);

		endIndex = str.find(sSep);
	}
	if(str.compare("") != 0)
		stringList.push_back(Value(str));
	return stringList;*/

	size_t nend = 0;
	size_t nbegin = 0;
	size_t tokenSize = token.size();

	while(nend != std::string::npos)
	{
		nend = src.find(token,nbegin);
		if(nend == std::string::npos)
			vect.push_back(Value(src.substr(nbegin, src.length()-nbegin)));
		else
		{
			vect.push_back(Value(src.substr(nbegin, nend-nbegin)));
			nbegin = nend + tokenSize;
		}
	}
	return vect;
}
ValueVector split(const std::string&src,char ch)
{
	std::string token;
	token = ch;

	return split(src,token);
}
void split(const std::string& src,const std::string& token,const std::function<void (int,Value)>& callback)
{
	size_t nend = 0;
	size_t nbegin = 0;
	size_t tokenSize = token.size();
	size_t index = 0;

	while(nend != std::string::npos)
	{
		nend = src.find(token,nbegin);
		if(nend == std::string::npos)
		{
			//避免最后为空
			auto str = src.substr(nbegin, src.length()-nbegin);
			
			if (!str.empty())
				callback(index,Value(str));
		}
		else
		{
			callback(index,Value(src.substr(nbegin, nend-nbegin)));
		}
		nbegin = nend + tokenSize;
		
		index++;
	}
}
//计算Char对应的UTF8码的掩码和长度  
#define UTF8_COMPUTE(Char, Mask, Len)\
	if (Char < 128)		\
	{                   \
		Len = 1;		\
		Mask = 0x7f;	\
	}					\
	else if ((Char & 0xe0) == 0xc0)\
	{                   \
		Len = 2;		\
		Mask = 0x1f;	\
	}					\
	else if ((Char & 0xf0) == 0xe0) \
	{                   \
		Len = 3;		\
		Mask = 0x0f;	\
	}					\
	else if ((Char & 0xf8) == 0xf0)\
	{                   \
		Len = 4;		\
		Mask = 0x07;	\
	}					\
	else if ((Char & 0xfc) == 0xf8)\
	{                   \
		Len = 5;		\
		Mask = 0x03;	\
	}					\
	else if ((Char & 0xfe) == 0xfc)\
	{                   \
		Len = 6;		\
		Mask = 0x01;	\
	}					\
	else                \
	{                   \
		Len = -1;		\
	}
//取得Char对应的UTF8码 
#define UTF8_GET(Result, Chars, Count, Mask, Len)\
	(Result) = (Chars)[0] & (Mask);				\
	for ((Count) = 1; (Count) < (Len); ++(Count))\
	{											\
		if (((Chars)[(Count)] & 0xc0) != 0x80)  \
		{                                       \
			(Result) = -1;						\
			break;								\
		}										\
		(Result) <<= 6;							\
		(Result) |= ((Chars)[(Count)] & 0x3f);	\
	}

long cc_utf8_strlen (const char * p, int max)  
{  
    long len = 0;  
    const char *start = p;  
  
    if (!(p != NULL || max == 0))  
    {  
        return 0;  
    }  
  
    if (max < 0)  
    {  
        while (*p)  
    {  
        p = cc_utf8_next_char (p);  
        ++len;  
    }  
    }  
    else  
    {  
        if (max == 0 || !*p)  
    return 0;  
  
        p = cc_utf8_next_char (p);  
  
        while (p - start < max && *p)  
    {  
        ++len;  
        p = cc_utf8_next_char (p);  
    }  
  
        /* only do the last len increment if we got a complete 
        * char (don't count partial chars) 
        */  
        if (p - start == max)  
    ++len;  
    }  
  
    return len;  
}
//将Unicode编码字符串p转换为UTF8编码  
unsigned int cc_utf8_get_char (const char * p)  
{
    int i, mask = 0, len;  
    unsigned int result;  
    unsigned char c = (unsigned char) *p;  
  
    UTF8_COMPUTE (c, mask, len);  
    if (len == -1)  
    return (unsigned int) - 1;  
    UTF8_GET (result, p, i, mask, len);  
  
    return result;  
} 

int cc_wcslen(const unsigned short* str)
{
    if (str == nullptr)
        return -1;
    int i=0;
    while(*str++) i++;
    return i;
}
//end namespace StringUtils
}
NS_SDL_END
