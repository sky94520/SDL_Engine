#ifndef __SDL_NS_H__
#define __SDL_NS_H__
#include<cstdlib>
#include<string>
#include<vector>

#include "SEPoint.h"
#include "SESize.h"
#include "SERect.h"
NS_SDL_BEGIN
/**
@brief 返回point
@param str 格式{x,y}的字符串
@return 如果字符串不可用返回Point::ZERO
*/
Point PointFromString(const std::string&str,char token = ',');
/**
@brief 返回Size
@param str {w,h}
@return 如果字符串不可用，返回Size::ZERO
*/
Size SizeFromString(const std::string&str,char token = ',');
/**
@brief 返回Rect
@param str {{x,y},{w,h}}
@return 如果字符串不可用，返回Rect::ZERO
*/
Rect RectFromString(const std::string&str,char token = ',');
NS_SDL_END
#endif
