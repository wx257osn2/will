#pragma once
#include"com.hpp"

namespace will{

namespace detail{

template<typename T>
class resource{
protected:
	com_ptr<T> res;
public:
	explicit resource(com_ptr<T>&& resource):res(std::move(resource)){}
	explicit resource(T* resource):res(resource){}
	resource(const resource&) = delete;
	resource(resource&&) = default;
	resource& operator=(const resource&) = delete;
	resource& operator=(resource&&) = default;
	~resource() = default;
	T* get()const{return res.get();}
	T* operator->()const{return res.get();}
	explicit operator bool()const{return static_cast<bool>(res);}
};

template<typename T>
class property{
protected:
	T prop;
public:
	property():prop(){}
	property(const property&) = default;
	property(property&&) = default;
	property& operator=(const property&) = default;
	property& operator=(property&&) = default;
	~property() = default;
	operator T&(){return prop;}
	T& get(){return prop;}
};

}

}
