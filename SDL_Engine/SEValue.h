#ifndef __SDL_Value_H__
#define __SDL_Value_H__
#include<string>
#include<cstring>
#include<vector>
#include<unordered_map>
#include<cstdlib>
#include<sstream>

#include "SEPlatformMarcos.h"
#include "SEObject.h"

NS_SDL_BEGIN

class Value;
typedef std::vector<Value> ValueVector;
typedef std::unordered_map<std::string,Value> ValueMap;
typedef std::unordered_map<int,Value> ValueMapIntKey;

class Value
{
public:
	static const Value Null;
public:
	enum class Type
    {
        NONE = 0,
        BYTE,
        INTEGER,
        FLOAT,
        DOUBLE,
        BOOLEAN,
        STRING,
        VECTOR,
        MAP,
        INT_KEY_MAP
    };
private:
	Type _type;
	union 
	{
		unsigned char byteVal;
		int intVal;
		float floatVal;
		double doubleVal;
		bool boolVal;
		std::string*stringVal;
		ValueVector*vectorVal;
		ValueMap*mapVal;
		ValueMapIntKey*intKeyMapVal;
	}_field;
public:
	Value();
	explicit Value(unsigned char v);
	explicit Value(int v);
	explicit Value(float v);
	explicit Value(double v);
	explicit Value(bool v);
	explicit Value(const char*v);
	explicit Value(const std::string&v);
	explicit Value(const ValueVector&v);
	explicit Value(const ValueMap&v);
	explicit Value(const ValueMapIntKey&v);
	//复制构造函数
	Value(const Value&v);
	~Value();
	//重载=运算符
	Value& operator=(const Value&v);

	Value& operator=(unsigned char v);
	Value& operator=(int v);
	Value& operator=(float v);
	Value& operator=(double v);
	Value& operator=(bool v);
	Value& operator=(const char*v);
	Value& operator=(const std::string& v);
	Value& operator=(const ValueVector& v);
	Value& operator=(const ValueMap& v);
	Value& operator=(const ValueMapIntKey& v);
	// == !=
	bool operator==(const Value&v)const;
	bool operator!=(const Value&v);
	//
	unsigned char asByte()const;
	int asInt()const;
	float asFloat()const;
	double asDouble()const;
	bool asBool()const;
	std::string asString()const;
	ValueVector &asValueVector()const;
	ValueMap &asValueMap()const;
	ValueMapIntKey &asValueIntKey()const;
	//是否为空
	bool isNull()const{return _type == Type::NONE;}
	Type getType()const{return _type;}
private:
	void clear();
	void reset(Type type);
};
NS_SDL_END
#endif
