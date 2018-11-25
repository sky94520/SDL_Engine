#ifndef __SDL_Math_H__
#define __SDL_Math_H__
/*SDL_Engine Math*/
#include<cstdlib>
#include<cmath>
#include "SEPlatformMarcos.h"
NS_SDL_BEGIN
#ifndef FLT_EPSILON
#define FLT_EPSILON 1.192092896e-07F        /* smallest such that 1.0+FLT_EPSILON != 1.0 */
#endif
#define MAX_UINT 0xffffffff
#define SDL_REPEAT_FOREVER (MAX_UINT - 1)
#define MAX(x,y) ((x)>(y)?(x):(y))
#define MIN(x,y) ((x)>(y)?(y):(x))
#define RANDOM(start,end) (fmodf((float)rand(),(end)-(start))+(start))/*[start,end) 不需要自己手动调用srand，已经在Director中调用*/
#define RANDOM_0_1() (rand()/double(RAND_MAX))
//返回-1～1之间的随机值
#define RANDOM_MINUS1_1() ((2.f * rand()/(double)RAND_MAX) -1.f)
#define SDL_ANGLE_TO_DEGREE(angle) (3.1415926/180 * (angle))/*角度转弧度*/
#define SDL_DEGREE_TO_ANGLE(degree) (180.0/3.1415926 *(degree))/*弧度转角度*/
NS_SDL_END
#endif
