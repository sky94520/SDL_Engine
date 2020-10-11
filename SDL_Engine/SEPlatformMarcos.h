#ifndef __Platform_H__
#define __Platform_H__
#include<iostream>
#include<assert.h>
#include<functional>
#include<cstdio>
#include <cfloat>

#define LOG(format, ...) \
	do{ 		\
		printf("FILE:%s,LINE:%d\n", __FILE__, __LINE__);\
		printf((format), ##__VA_ARGS__);	\
	}while(0)

#define IS_FLOAT_ZERO(number) ((number) > -FLT_EPSILON && (number) < FLT_EPSILON)

#define NS_SDL_BEGIN namespace SDL{
#define NS_SDL_END }
#define USING_NS_SDL using namespace SDL

#define SDLASSERT(cond,msg)\
	if(!(cond))\
	{\
	printf("Assert failed:%s",msg);\
	assert(cond);\
	}

#define CREATE_FUNC(__TYPE__)    \
static __TYPE__*create()         \
{                                \
	__TYPE__*pRet=new __TYPE__();\
	if(pRet&&pRet->init())       \
	{                            \
         pRet->autorelease();   \
		 return pRet;            \
	}                            \
	delete pRet;                 \
	pRet=NULL;                   \
	return pRet;                 \
}

#define AnimationFrameDisplayedNotification "SDLAnimationFrameDisplayedNotification"


#define SDL_SAFE_DELETE(p)do{if(p) delete p;p=nullptr;}while(0)
#define SDL_SAFE_RELEASE(p)do{if(p) p->release();}while(0)
#define SDL_SAFE_RELEASE_NULL(p)do{if(p) p->release(); p = nullptr;}while(0)
#define SDL_SAFE_RETAIN(p) do { if(p) { (p)->retain(); } } while(0)
#define SDL_BREAK_IF(cond) if(cond) break
//c++11
#define SDL_CALLBACK_0(__selector__,__target__,...) std::bind(&__selector__,__target__,##__VA_ARGS__)
#define SDL_CALLBACK_1(__selector__,__target__,...) std::bind(&__selector__,__target__,std::placeholders::_1,##__VA_ARGS__)
#define SDL_CALLBACK_2(__selector__,__target__,...) std::bind(&__selector__,__target__,std::placeholders::_1,std::placeholders::_2,##__VA_ARGS__)
#define SDL_CALLBACK_3(__selector__,__target__,...) std::bind(&__selector__,__target__,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,##__VA_ARGS__)
//自定义宏 主要用于简化开发
#define SDL_SYNTHESIZE(varType,varName,funName)\
protected:varType varName;\
public:virtual varType get##funName()const{return varName;}\
public:virtual void set##funName(const varType& var) {varName = var;}

#define SDL_SYNTHESIZE_PASS_BY_REF(varType, varName, funName) \
protected: varType varName;				      \
public: virtual varType& get##funName() const {return varName;}\
public: virtual void set##funName(const varType& var) {varName = var;}

#define SDL_BOOL_SYNTHESIZE(varName,funName)\
protected: bool varName;\
public:virtual bool is##funName()const{return varName;}\
public:virtual void set##funName(bool var){varName = var;}

#define SDL_SYNTHESIZE_RETAIN(varType,varName,funName)\
private:varType varName;\
public:virtual varType get##funName()const{return varName;}\
public:virtual void set##funName(varType var)\
{\
	if(var != varName)\
	{\
		SDL_SAFE_RETAIN(var);\
		SDL_SAFE_RELEASE(varName);\
		varName = var;\
	}\
}
/*只读*/
#define SDL_SYNTHESIZE_READONLY(varType,varName,funcName) \
protected:varType varName;                                \
public: virtual varType get##funcName(void)const{return varName;}

#define SDL_SYNTHESIZE_READONLY_PASS_BY_REF(varType, varName, funName) \
protected: varType varName;					       \
public: virtual const varType& get##funcName() const {return varName;}

//声明一个保护变量和相应的函数声明，需要自行定义
#define SDL_PROPERTY(varType, varName, funName) \
protected: varType varName;			\
public: virtual varType get##funName();		\
public: virtual void set##funName(varType var);

#define SDL_PROPERTY_PASS_BY_REF(varType, varName, funName) \
protected: varType varName;				    \
public: virtual const varType& get##funName();		    \
public: virtual void set##funName(const varType& var);

#define SDL_PROPERTY_READONLY(varType, varName, funName) \
protected: varType varName;				 \
public: virtual varType get##funName()const;

#define SDL_PROPERTY_READONLY_PASS_BY_REF(varType, varName, funName) \
protected: varType varName;					     \
public: virtual const varType& get##funName() const;


#endif
