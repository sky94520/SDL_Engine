#include "SEValue.h"
NS_SDL_BEGIN

const Value Value::Null;

Value::Value()
	:_type(Type::NONE)
{
	memset(&_field,0,sizeof (_field));
}
Value::Value(unsigned char v)
	:_type(Type::BYTE)
{
	_field.byteVal = v;
}
Value::Value(int v)
	:_type(Type::INTEGER)
{
	_field.intVal = v;
}
Value::Value(float v)
	:_type(Type::FLOAT)
{
	_field.floatVal = v;
}
Value::Value(double v)
	:_type(Type::DOUBLE)
{
	//_field.floatVal = (float)v;
	_field.doubleVal = v;
}
Value::Value(bool v)
	:_type(Type::BOOLEAN)
{
	_field.boolVal = v;
}
Value::Value(const char*v)
	:_type(Type::STRING)
{
	_field.stringVal = new std::string();
	if(v)
	{
		*_field.stringVal = v;
	}
}
Value::Value(const std::string&v)
	:_type(Type::STRING)
{
	_field.stringVal = new std::string();
	*_field.stringVal = v;
}
Value::Value(const ValueVector&v)
	:_type(Type::VECTOR)
{
	_field.vectorVal = new ValueVector();
	*_field.vectorVal = v;
}
Value::Value(const ValueMap&v)
	:_type(Type::MAP)
{
	_field.mapVal = new ValueMap();
	*_field.mapVal = v;
}
Value::Value(const ValueMapIntKey&v)
	:_type(Type::INT_KEY_MAP)
{
	_field.intKeyMapVal = new ValueMapIntKey();
	*_field.intKeyMapVal = v;
}
Value::Value(const Value&v)
	:_type(Type::NONE)
{
	*this = v;
}
Value::~Value()
{
	clear();
}
//operator
Value& Value::operator=(const Value&v)
{
	//如果两地址相同，直接返回
	if(this == &v)
		return *this;
	reset(v._type);
	switch(v._type)
	{
	case Type::BYTE:_field.byteVal = v._field.byteVal;break;
	case Type::INTEGER:_field.intVal = v._field.intVal;break;
	case Type::FLOAT:_field.floatVal = v._field.floatVal;break;
	case Type::DOUBLE:_field.doubleVal = v._field.doubleVal;break;
	case Type::BOOLEAN:_field.boolVal = v._field.boolVal;break;
	case Type::STRING:
		{
			if(_field.stringVal == nullptr)
				_field.stringVal = new std::string();

			*_field.stringVal = v._field.stringVal->c_str();
		};break;
	case Type::VECTOR:
		{
			if(_field.vectorVal == nullptr)
				_field.vectorVal = new ValueVector();
			*_field.vectorVal = *(v._field.vectorVal);
		};break;
	case Type::MAP:
		{
			if(_field.mapVal == nullptr)
				_field.mapVal = new ValueMap();
			*_field.mapVal = *v._field.mapVal;
		}break;
	case Type::INT_KEY_MAP:
		{
			if(_field.mapVal == nullptr)
				_field.intKeyMapVal = new ValueMapIntKey();
			*_field.intKeyMapVal = *v._field.intKeyMapVal;
		}break;
	default:break;
	}
	return *this;
}
Value& Value::operator=(unsigned char v)
{
	reset(Type::BYTE);
	_field.byteVal = v;
	return *this;
}
Value& Value::operator=(int v)
{
	reset(Type::INTEGER);
	_field.intVal = v;
	return *this;
}
Value& Value::operator=(float v)
{
	reset(Type::FLOAT);
	_field.floatVal = v;
	return *this;
}
Value& Value::operator=(double v)
{
	reset(Type::DOUBLE);
	_field.doubleVal = v;
	return *this;
}
Value& Value::operator=(bool v)
{
	reset(Type::BYTE);
	_field.byteVal = v;
	return *this;
}
Value& Value::operator=(const char*v)
{
	reset(Type::STRING);
	*_field.stringVal = v;
	return *this;
}
Value& Value::operator=(const std::string&v)
{
	reset(Type::STRING);
	*_field.stringVal = v;
	return *this;
}
Value& Value::operator=(const ValueVector&v)
{
	reset(Type::VECTOR);
	*_field.vectorVal = v;
	return *this;
}
Value& Value::operator=(const ValueMap&v)
{
	reset(Type::MAP);
	*_field.mapVal = v;
	return *this;
}
Value& Value::operator=(const ValueMapIntKey&v)
{
	reset(Type::INT_KEY_MAP);
	*_field.intKeyMapVal = v;
	return *this;
}
bool Value::operator==(const Value&v)const
{
	if(this == &v)
		return true;
	if(_type != v._type)
		return false;
	switch(_type)
	{
	case Type::BYTE:return _field.byteVal == v._field.byteVal;break;
	case Type::INTEGER:return _field.intVal == v._field.intVal;break;
	case Type::FLOAT:return _field.floatVal == v._field.floatVal;break;
	case Type::DOUBLE:return _field.doubleVal == v._field.doubleVal;break;
	case Type::BOOLEAN:return _field.boolVal == v._field.boolVal;break;
	case Type::STRING:return *_field.stringVal == *v._field.stringVal;break;
	case Type::VECTOR:return *_field.vectorVal == *v._field.vectorVal;break;
	case Type::MAP:return *_field.mapVal == *v._field.mapVal;break;
	case Type::INT_KEY_MAP:return *_field.intKeyMapVal == *v._field.intKeyMapVal;break;
	default:break;
	}
	return false;
}
bool Value::operator!=(const Value&v)
{
	if(this == &v)
		return false;
	if(_type != v._type)
		return true;
	switch(_type)
	{
	case Type::BYTE:return _field.byteVal != v._field.byteVal;break;
	case Type::INTEGER:return _field.intVal != v._field.intVal;break;
	case Type::FLOAT:return _field.floatVal != v._field.floatVal;break;
	case Type::DOUBLE:return _field.doubleVal != v._field.doubleVal;break;
	case Type::BOOLEAN:return _field.boolVal != v._field.boolVal;break;
	case Type::STRING:return *_field.stringVal != *v._field.stringVal;break;
	case Type::VECTOR:return *_field.vectorVal != *v._field.vectorVal;break;
	case Type::MAP:return *_field.mapVal != *v._field.mapVal;break;
	case Type::INT_KEY_MAP:return *_field.intKeyMapVal != *v._field.intKeyMapVal;break;

	default:break;
	}
	return true;
}
unsigned char Value::asByte()const
{
	//无法转换
	if(_type == Type::VECTOR)
		return 0;
	//目前来说都能转换
	switch(_type)
	{
	case Type::BYTE:return _field.byteVal;break;
	case Type::INTEGER:return static_cast<unsigned char>(_field.intVal);break;
	case Type::FLOAT:return static_cast<unsigned char>(_field.floatVal);break;
	case Type::DOUBLE:return static_cast<unsigned char>(_field.doubleVal);break;
	case Type::BOOLEAN:return _field.boolVal;
	case Type::STRING:return static_cast<unsigned char>(std::atoi(_field.stringVal->c_str()));break;
	default:break;
	}
	return 0;
}
int Value::asInt()const
{
	//无法转换
	if(_type == Type::VECTOR)
		return 0;
	switch(_type)
	{
	case Type::BYTE:return _field.byteVal;break;
	case Type::INTEGER:return _field.intVal;break;
	case Type::FLOAT:return static_cast<int>(_field.floatVal);break;
	case Type::DOUBLE:return static_cast<int>(_field.doubleVal);break;
	case Type::BOOLEAN:return _field.boolVal;
	case Type::STRING:return static_cast<int>(atoi(_field.stringVal->c_str()));break;
	default:break;
	}
	return 0;
}
float Value::asFloat()const
{
	//无法转换
	if(_type == Type::VECTOR)
		return 0.f;
	switch(_type)
	{
	case Type::BYTE:return static_cast<float>(_field.byteVal);break;
	case Type::INTEGER:return static_cast<float>(_field.intVal);break;
	case Type::FLOAT:return _field.floatVal;break;
	case Type::DOUBLE:return static_cast<float>(_field.doubleVal);break;
	case Type::BOOLEAN:return _field.boolVal;
	case Type::STRING:return static_cast<float>(atof(_field.stringVal->c_str()));break;
	default:break;
	}
	return 0.f;
}
double Value::asDouble()const
{
	//无法转换
	if(_type == Type::VECTOR)
		return 0;
	switch(_type)
	{
	case Type::BYTE:return static_cast<double>(_field.byteVal);break;
	case Type::INTEGER:return static_cast<double>(_field.intVal);break;
	case Type::FLOAT:return static_cast<double>(_field.floatVal);break;
	case Type::DOUBLE:return _field.doubleVal;break;
	case Type::BOOLEAN:return _field.boolVal;
	case Type::STRING:return static_cast<double>(atof(_field.stringVal->c_str()));break;
	default:break;
	}
	return 0.0;
}
bool Value::asBool()const
{
	//无法转换
	if(_type == Type::VECTOR)
		return 0;
	switch(_type)
	{
	case Type::BYTE:return _field.byteVal == 0 ? false:true;break;
	case Type::INTEGER:return _field.intVal == 0 ? false:true;break;
	case Type::FLOAT:return _field.floatVal == 0.f ? false:true;break;
	case Type::DOUBLE:return _field.doubleVal == 0.0 ? false:true;break;
	case Type::BOOLEAN:return _field.boolVal;
	case Type::STRING:return *_field.stringVal == "0" || *_field.stringVal == "false"?false:true;break;
	default:break;
	}
	return false;
}
std::string Value::asString()const
{
	//无法转换
	if(_type == Type::VECTOR)
		return "";
	if(_type == Type::STRING)
		return *_field.stringVal;
	std::stringstream ret;
	switch(_type)
	{
	case Type::BYTE:ret<<_field.byteVal;break;
	case Type::INTEGER:ret<<_field.intVal;break;
	case Type::FLOAT:ret<<_field.floatVal;break;
	case Type::DOUBLE:ret<<_field.doubleVal;break;
	case Type::BOOLEAN:ret<<(_field.boolVal?"true":"false");break;
	}
	return ret.str();
}
ValueVector &Value::asValueVector()const
{
	SDLASSERT(_type == Type::VECTOR,"type不是VECTOR");
	return *_field.vectorVal;
}
ValueMap &Value::asValueMap()const
{
	SDLASSERT(_type == Type::MAP,"type不是MAP");
	return *_field.mapVal;
}
ValueMapIntKey &Value::asValueIntKey()const
{
	SDLASSERT(_type == Type::INT_KEY_MAP,"type不是INT_KEY_MAP");
	return *_field.intKeyMapVal;
}
void Value::clear()
{
	switch(_type)
	{
	case Type::BYTE:_field.byteVal = 0;break;
	case Type::INTEGER:_field.intVal = 0;break;
	case Type::FLOAT:_field.floatVal = 0.f;break;
	case Type::DOUBLE:_field.doubleVal = 0.0;break;
	case Type::BOOLEAN:_field.boolVal = false;break;
	case Type::STRING:SDL_SAFE_DELETE(_field.stringVal);break;
	case Type::VECTOR:SDL_SAFE_DELETE(_field.vectorVal);break;
	case Type::MAP:SDL_SAFE_DELETE(_field.mapVal);break;
	case Type::INT_KEY_MAP:SDL_SAFE_DELETE(_field.intKeyMapVal);break;
	default:break;
	}
	memset(&_field,0,sizeof (_field));
	_type = Type::NONE;
}
void Value::reset(Type type)
{
	if(_type == type)
		return;
	clear();
	//分配空间等
	switch(type)
	{
	case Type::STRING:
		_field.stringVal = new std::string();break;
	case Type::VECTOR:
		_field.vectorVal = new ValueVector();break;
	case Type::MAP:
		_field.mapVal = new ValueMap();break;
	case Type::INT_KEY_MAP:
		_field.intKeyMapVal = new ValueMapIntKey();break;
	default:break;
	}
	_type = type;
}
NS_SDL_END
