//Copyright (C) 2019 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include"_2dim.hpp"
namespace will{
namespace two_dim{
template<>
struct attribute_traits<::POINT>{
	using tag_type = tag::point;
	using element_type = ::LONG;
	static constexpr element_type x(const ::POINT& po)noexcept{return po.x;}
	static constexpr element_type y(const ::POINT& po)noexcept{return po.y;}
	static constexpr ::POINT create(element_type x, element_type y)noexcept{return {x, y};}
};
template<>
struct attribute_traits<::RECT>{
	using tag_type = tag::point_pair;
	using element_type = ::LONG;
	static constexpr xy<element_type> _1(const ::RECT& t)noexcept{return {t.left, t.top};}
	static constexpr xy<element_type> _2(const ::RECT& t)noexcept{return {t.right, t.bottom};}
	static constexpr element_type x1(const ::RECT& t)noexcept{return t.left;}
	static constexpr element_type y1(const ::RECT& t)noexcept{return t.top;}
	static constexpr element_type x2(const ::RECT& t)noexcept{return t.right;}
	static constexpr element_type y2(const ::RECT& t)noexcept{return t.bottom;}
	static constexpr ::RECT create(element_type x1, element_type y1, element_type x2, element_type y2)noexcept{return {x1, y1, x2, y2};}
	static constexpr ::RECT create(const xy<element_type>& _1, const xy<element_type>& _2)noexcept{return {_1.x, _1.y, _2.x, _2.y};}
};
}
}