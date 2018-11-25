#ifndef __SDL_Vector_H__
#define __SDL_Vector_H__
#include<algorithm>
#include<vector>
#include "SEPlatformMarcos.h"
NS_SDL_BEGIN
template<typename T>
class Vector
{
private:
	std::vector<T> _data;
public:
	typedef typename std::vector<T>::iterator iterator;
	typedef typename std::vector<T>::const_iterator const_iterator;
	typedef typename std::vector<T>::reverse_iterator reverse_iterator;
	typedef typename std::vector<T>::const_reverse_iterator const_reverse_iterator;

	Vector<T>():_data()
	{
	}
	//复制构造函数
	Vector<T>(const Vector<T>&other)
	{
		_data = other._data;
		addRefForAllObjects();
	}
	//重载 = 
	Vector& operator=(const Vector&other)
	{
		if(this != &other)
		{
			clear();
			_data = other._data;
			addRefForAllObjects();
		}
		return *this;
	}
	~Vector<T>()
	{
		clear();
	}
	iterator begin(){return _data.begin();}
	iterator end(){return _data.end();}
	const_iterator cbegin()const{return _data.cbegin();}
	const_iterator cend()const{return _data.cend();}
	/*
	reverse_iterator rbegin(){return _data.rbegin();}
	reverse_iterator rend(){return _data.rend();}
	const_reverse_iterator rbegin()const{return _data.rbegin();}
	const_reverse_iterator rend()const{return _data.rend();}*/
	void reserve(unsigned int n)
	{
		_data.reserve(n);
	}
	unsigned int capacity()const
	{
		return _data.capacity();
	}
	unsigned int size()const
	{
		return _data.size();
	}
	bool empty()const
	{
		return _data.empty();
	}
	unsigned int max_size()const
	{
		return _data.max_size();
	}
	//返回该object所在的位置
	int getIndex(T object)const
	{
		auto iter = std::find(_data.cbegin(),_data.cend(),object);
		return iter==_data.end()?-1:iter - _data.begin();
	}
	const_iterator find(T object)const
	{
		return std::find(_data.begin(),_data.end(),object);
	}
	iterator find(const T&object)
	{
		return std::find(_data.begin(),_data.end(),object);
	}
	T at(unsigned int index)const
	{
		SDLASSERT(index>=0&&index<size(),"Invalid index!");
		return _data.at(index);
	}
	T front()const
	{
		return _data.front;
	}
	T back()const
	{
		return _data.back();
	}
	bool contains(const T&object)const
	{
		return std::find(_data.begin(),_data.end(),object) != _data.end();
	}
	bool equals(const Vector<T>&other)
	{
		unsigned int t = other.size();
		if(t != size())
			return false;
		 for(ssize_t i=0;i<t;i++)
		 {
			 if(this->at(i) != other.at(i))
				 return false;
		 }
		 return true;
	}
	void push_back(T object)
	{
		if(object != nullptr)
		{
			_data.push_back(object);
			object->retain();
		}
		else
			LOG("The object should not be nullptr\n");

	}
	void push_back(const Vector<T>&other)
	{
		for(const auto&obj:other)
		{
			_data.push_back(obj);
			obj->retain();
		}
	}
	void pop_back()
	{
		if(!_data.empty())
		{
			auto other = _data.back();
			_data.pop_back();
			other->release();
		}
		else
			LOG("empty Vector\n");
	}
	iterator erase(iterator position)
	{
		if(position >= _data.begin() && position <= _data.end())
		{
			(*position)->release();
			return _data.erase(position);
		
		}else
			LOG("Invalid position\n");
		return _data.end();
	}
	iterator erase(unsigned int index)
	{
		if(!_data.empty() && index >=0 && index < size())
        {
			auto it = std::next( begin(), index );
			(*it)->release();
			return _data.erase(it);
		}
		else
			LOG("Invalid index!\n");
	}
	void eraseObject(T object)
	{
		auto iter = std::find(_data.begin(),_data.end(),object);
		if(iter != _data.end())
		{
			_data.erase(iter);
			object->release();
		}
	}
	//为全部的成员增加引用reference
	void addRefForAllObjects()
	{
		for(auto data:_data)
			data->retain();
	}
	void clear()
	{
		for(auto iter=std::begin(_data);iter!=std::end(_data);++iter)
		{
			(*iter)->release();
		}
		_data.clear();
	}

};
NS_SDL_END
#endif
