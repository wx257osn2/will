//Copyright (C) 2014-2017 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include<utility>
#include<type_traits>
#include"com.hpp"

namespace will{

namespace detail{

template<typename T>
class resource{
protected:
	com_ptr<T> res;
public:
	explicit resource()noexcept = default;
	explicit resource(com_ptr<T>&& resource)noexcept:res(std::move(resource)){}
	explicit resource(T*&& resource)noexcept:res(std::move(resource)){}
	resource(const resource&) = delete;
	resource(resource&&)noexcept = default;
	resource& operator=(const resource&) = delete;
	resource& operator=(resource&&)noexcept = default;
	~resource()noexcept = default;
	T* get()const noexcept{return res.get();}
	T* operator->()const noexcept{return res.get();}
	explicit operator bool()const noexcept{return static_cast<bool>(res);}
	void swap(resource<T>& other)noexcept{res.swap(other.res);}
};

struct get_interface_impl{
	template<typename T>static T type(resource<T>);
	template<typename T, std::enable_if_t<std::is_base_of<resource<std::remove_pointer_t<decltype(std::declval<T>().get())>>, T>::value, std::nullptr_t> = nullptr>static std::remove_pointer_t<decltype(std::declval<T>().get())> type(T);
};
template<typename T>
using get_interface = decltype(get_interface_impl::type(std::declval<T>()));
template<typename T, std::enable_if_t<std::is_base_of<resource<std::remove_pointer_t<decltype(std::declval<T>().get())>>, T>::value>* = nullptr>
inline expected::expected<T, hresult_error> convert_to_rich_interface(expected::expected<decltype(std::declval<T>().get()), HRESULT>&& e, const TCHAR* f){
	using I = get_interface<T>;
	return e.map([](I* t){return T{std::move(t)};}).emap([&](HRESULT e){return make_unexpected<hresult_error>(f, e);});
}

template<typename T>
class property{
protected:
	T prop;
public:
	property()noexcept(std::is_nothrow_default_constructible<T>::value):prop(){}
	property(const property&)noexcept(std::is_nothrow_copy_constructible<T>::value) = default;
	property(property&&)noexcept(std::is_nothrow_move_constructible<T>::value) = default;
	property& operator=(const property&)noexcept(std::is_nothrow_copy_assignable<T>::value) = default;
	property& operator=(property&&)noexcept(std::is_nothrow_move_assignable<T>::value) = default;
	~property()noexcept(std::is_nothrow_destructible<T>::value) = default;
	operator T&()&noexcept{return prop;}
	T& get()&noexcept{return prop;}
	operator const T&()const&noexcept{return prop;}
	const T& get()const&noexcept{return prop;}
	operator T&&()&&noexcept{return std::move(prop);}
	T&& get()&&noexcept{return std::move(prop);}
};

}

}
