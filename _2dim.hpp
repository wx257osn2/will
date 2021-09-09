//Copyright (C) 2014-2017, 2019, 2021 I
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include<type_traits>
#pragma once
namespace will{
namespace two_dim{

namespace tag{

struct point{};
struct size{};
struct point_pair{};
struct point_and_size{};

}

template<typename T>struct attribute_traits;

template<typename T>
struct xy{
	T x, y;
	using element_type = T;
	constexpr bool operator==(const xy<T>& other)noexcept{return x == other.x && y == other.y;}
	constexpr bool operator!=(const xy<T>& other)noexcept{return !(*this == other);}
#if 0
	template<typename U, std::enable_if_t<std::is_same<tag::point, typename attribute_traits<std::decay_t<U>>::tag_type>::value, std::nullptr_t> = nullptr>
	constexpr U convert()const noexcept{using et = typename attribute_traits<std::decay_t<U>>::element_type;return attribute_traits<U>::create(static_cast<et>(x), static_cast<et>(y));}
	template<typename U, std::enable_if_t<std::is_same<tag::point, typename attribute_traits<std::decay_t<U>>::tag_type>::value, std::nullptr_t> = nullptr>
	constexpr operator U()const noexcept{return convert();}
#endif
};

template<typename T>
struct attribute_traits<xy<T>>{
	using tag_type = tag::point;
	using element_type = T;
	static constexpr T x(const xy<T>& xy)noexcept{return xy.x;}
	static constexpr T y(const xy<T>& xy)noexcept{return xy.y;}
	static constexpr xy<T> create(T x, T y)noexcept{return {x, y};}
};

template<typename T>
struct wh{
	T w, h;
	using element_type = T;
	constexpr bool operator==(const wh<T>& other)noexcept{return w == other.w && h == other.h;}
	constexpr bool operator!=(const wh<T>& other)noexcept{return !(*this == other);}
#if 0
	template<typename U, std::enable_if_t<std::is_same<tag::size, typename attribute_traits<std::decay_t<U>>::tag_type>::value, std::nullptr_t> = nullptr>
	constexpr U convert()const noexcept{using et = typename attribute_traits<std::decay_t<U>>::element_type;return attribute_traits<U>::create(static_cast<et>(w), static_cast<et>(h));}
	template<typename U, std::enable_if_t<std::is_same<tag::size, typename attribute_traits<std::decay_t<U>>::tag_type>::value, std::nullptr_t> = nullptr>
	constexpr operator U()const noexcept{return convert();}
#endif
};

template<typename T>
struct attribute_traits<wh<T>>{
	using tag_type = tag::size;
	using element_type = T;
	static constexpr T w(const wh<T>& wh)noexcept{return wh.w;}
	static constexpr T h(const wh<T>& wh)noexcept{return wh.h;}
	static constexpr wh<T> create(T w, T h)noexcept{return {w, h};}
};

template<typename T>
struct xyxy{
	xy<T> _1, _2;
	using element_type = T;
#if 0
	template<typename U, std::enable_if_t<std::is_same<tag::point_pair, typename attribute_traits<std::decay_t<U>>::tag_type>::value, std::nullptr_t> = nullptr>
	constexpr U convert()const noexcept{using et = typename attribute_traits<std::decay_t<U>>::element_type;return attribute_traits<U>::create(static_cast<et>(_1.x), static_cast<et>(_1.y), static_cast<et>(_2.x), static_cast<et>(_2.y));}
	template<typename U, std::enable_if_t<std::is_same<tag::point_pair, typename attribute_traits<std::decay_t<U>>::tag_type>::value, std::nullptr_t> = nullptr>
	constexpr operator U()const noexcept{return convert();}
#endif
};

template<typename T>
struct attribute_traits<xyxy<T>>{
	using tag_type = tag::point_pair;
	using element_type = T;
	static constexpr xy<T> _1(const xyxy<T>& xyxy)noexcept{return xyxy._1;}
	static constexpr xy<T> _2(const xyxy<T>& xyxy)noexcept{return xyxy._2;}
	static constexpr T x1(const xyxy<T>& xyxy)noexcept{return xyxy._1.x;}
	static constexpr T y1(const xyxy<T>& xyxy)noexcept{return xyxy._1.y;}
	static constexpr T x2(const xyxy<T>& xyxy)noexcept{return xyxy._2.x;}
	static constexpr T y2(const xyxy<T>& xyxy)noexcept{return xyxy._2.y;}
	static constexpr xyxy<T> create(T x1, T y1, T x2, T y2)noexcept{return {{x1, y1}, {x2, y2}};}
	static constexpr xyxy<T> create(const xy<T>& _1, const xy<T>& _2)noexcept{return {_1, _2};}
	template<typename U, typename V>
	static constexpr xyxy<T> create(const xy<U>& _1, const xy<V>& _2)noexcept{return {{static_cast<T>(_1.x), static_cast<T>(_1.y)}, {static_cast<T>(_2.x), static_cast<T>(_2.y)}};}
};

template<typename T, typename U = T>
struct xywh{
	xy<T> xy;
	wh<U> wh;
	using point_element_type = T;
	using size_element_type = U;
	constexpr bool operator==(const xywh<T, U>& other)noexcept{return xy == other.xy && wh == other.wh;}
	constexpr bool operator!=(const xywh<T, U>& other)noexcept{return !(*this == other);}
#if 0
	template<typename V, std::enable_if_t<std::is_same<tag::point_and_size, typename attribute_traits<std::decay_t<V>>::tag_type>::value, std::nullptr_t> = nullptr>
	constexpr V convert()const noexcept{using pet = typename attribute_traits<std::decay_t<V>>::point_element_type;using set = typename attribute_traits<std::decay_t<V>>::size_element_type;return attribute_traits<V>::create(static_cast<pet>(xy.x), static_cast<pet>(xy.y), static_cast<set>(wh.w), static_cast<set>(wh.h));}
	template<typename V, std::enable_if_t<std::is_same<tag::point_and_size, typename attribute_traits<std::decay_t<V>>::tag_type>::value, std::nullptr_t> = nullptr>
	constexpr operator V()const noexcept{return convert();}
#endif
};

template<typename T, typename U>
struct attribute_traits<xywh<T, U>>{
	using tag_type = tag::point_and_size;
	using point_element_type = T;
	using size_element_type = U;
	static constexpr two_dim::xy<T> xy(const xywh<T, U>& xywh)noexcept{return xywh.xy;}
	static constexpr two_dim::wh<U> wh(const xywh<T, U>& xywh)noexcept{return xywh.wh;}
	static constexpr T x(const xywh<T, U>& xywh)noexcept{return xywh.xy.x;}
	static constexpr T y(const xywh<T, U>& xywh)noexcept{return xywh.xy.y;}
	static constexpr U w(const xywh<T, U>& xywh)noexcept{return xywh.wh.w;}
	static constexpr U h(const xywh<T, U>& xywh)noexcept{return xywh.wh.h;}
	static constexpr xywh<T, U> create(T x, T y, U w, U h)noexcept{return {{x, y}, {w, h}};}
	static constexpr xywh<T, U> create(const two_dim::xy<T>& xy, const two_dim::wh<U>& wh)noexcept{return {xy, wh};}
	template<typename V, typename S>
	static constexpr xywh<T, U> create(const two_dim::xy<V>& xy, const two_dim::wh<S>& wh)noexcept{return {{static_cast<T>(xy.x), static_cast<T>(xy.y)}, {static_cast<U>(wh.w), static_cast<U>(wh.h)}};}
};

namespace detail{

template<typename>struct size_attribute_proxy;

template<typename T>
struct point_attribute_proxy:T{
	using element_type = typename attribute_traits<T>::element_type;
	constexpr element_type x()const noexcept{return attribute_traits<T>::x(*this);}
	constexpr element_type y()const noexcept{return attribute_traits<T>::y(*this);}
	template<typename U, std::enable_if_t<std::is_same<tag::point, typename attribute_traits<std::decay_t<U>>::tag_type>::value, std::nullptr_t> = nullptr>
	constexpr U convert()const noexcept{using et = typename attribute_traits<std::decay_t<U>>::element_type;return attribute_traits<U>::create(static_cast<et>(x()), static_cast<et>(y()));}
	template<typename U, std::enable_if_t<std::is_same<tag::point, typename attribute_traits<std::decay_t<U>>::tag_type>::value, std::nullptr_t> = nullptr>
	constexpr operator U()const noexcept{return convert<U>();}
};

template<typename T>
struct size_attribute_proxy:T{
	using element_type = typename attribute_traits<T>::element_type;
	constexpr element_type w()const noexcept{return attribute_traits<T>::w(*this);}
	constexpr element_type h()const noexcept{return attribute_traits<T>::h(*this);}
	template<typename U, std::enable_if_t<std::is_same<tag::size, typename attribute_traits<std::decay_t<U>>::tag_type>::value, std::nullptr_t> = nullptr>
	constexpr U convert()const noexcept{using et = typename attribute_traits<std::decay_t<U>>::element_type;return attribute_traits<U>::create(static_cast<et>(w()), static_cast<et>(h()));}
	template<typename U, std::enable_if_t<std::is_same<tag::size, typename attribute_traits<std::decay_t<U>>::tag_type>::value, std::nullptr_t> = nullptr>
	constexpr operator U()const noexcept{return convert<U>();}
};

template<typename T>
struct point_pair_attribute_proxy:T{
	using element_type = typename attribute_traits<T>::element_type;
	constexpr decltype(attribute_traits<T>::_1(std::declval<T>())) _1()const noexcept{return attribute_traits<T>::_1(*this);}
	constexpr decltype(attribute_traits<T>::_2(std::declval<T>())) _2()const noexcept{return attribute_traits<T>::_2(*this);}
	constexpr element_type x1()const noexcept{return attribute_traits<T>::x1(*this);}
	constexpr element_type y1()const noexcept{return attribute_traits<T>::y1(*this);}
	constexpr element_type x2()const noexcept{return attribute_traits<T>::x2(*this);}
	constexpr element_type y2()const noexcept{return attribute_traits<T>::y2(*this);}
	template<typename U, std::enable_if_t<std::is_same<tag::point_pair, typename attribute_traits<std::decay_t<U>>::tag_type>::value, std::nullptr_t> = nullptr>
	constexpr U convert()const noexcept{return attribute_traits<U>::create(_1(), _2());}
	template<typename U, std::enable_if_t<std::is_same<tag::point_pair, typename attribute_traits<std::decay_t<U>>::tag_type>::value, std::nullptr_t> = nullptr>
	constexpr operator U()const noexcept{return convert<U>();}
	template<typename U, std::enable_if_t<std::is_same<tag::point_and_size, typename attribute_traits<std::decay_t<U>>::tag_type>::value, std::nullptr_t> = nullptr>
	constexpr U convert()const noexcept{return attribute_traits<U>::create(_1(), _2()-_1());}
	template<typename U, std::enable_if_t<std::is_same<tag::point_and_size, typename attribute_traits<std::decay_t<U>>::tag_type>::value, std::nullptr_t> = nullptr>
	constexpr operator U()const noexcept{return convert<U>();}
};

template<typename T>
struct point_and_size_attribute_proxy:T{
	using point_element_type = typename attribute_traits<T>::point_element_type;
	using size_element_type = typename attribute_traits<T>::size_element_type;
	constexpr decltype(attribute_traits<T>::xy(std::declval<T>())) xy()const noexcept{return attribute_traits<T>::xy(*this);}
	constexpr decltype(attribute_traits<T>::wh(std::declval<T>())) wh()const noexcept{return attribute_traits<T>::wh(*this);}
	constexpr point_element_type x()const noexcept{return attribute_traits<T>::x(*this);}
	constexpr point_element_type y()const noexcept{return attribute_traits<T>::y(*this);}
	constexpr size_element_type w()const noexcept{return attribute_traits<T>::w(*this);}
	constexpr size_element_type h()const noexcept{return attribute_traits<T>::h(*this);}
	template<typename U, std::enable_if_t<std::is_same<tag::point_and_size, typename attribute_traits<std::decay_t<U>>::tag_type>::value, std::nullptr_t> = nullptr>
	constexpr U convert()const noexcept{return attribute_traits<U>::create(xy(), wh());}
	template<typename U, std::enable_if_t<std::is_same<tag::point_and_size, typename attribute_traits<std::decay_t<U>>::tag_type>::value, std::nullptr_t> = nullptr>
	constexpr operator U()const noexcept{return convert<U>();}
	template<typename U, std::enable_if_t<std::is_same<tag::point_pair, typename attribute_traits<std::decay_t<U>>::tag_type>::value, std::nullptr_t> = nullptr>
	constexpr U convert()const noexcept{return attribute_traits<U>::create(xy(), xy()+wh());}
	template<typename U, std::enable_if_t<std::is_same<tag::point_pair, typename attribute_traits<std::decay_t<U>>::tag_type>::value, std::nullptr_t> = nullptr>
	constexpr operator U()const noexcept{return convert<U>();}
};

}

template<typename T>
class attribute_proxy: public
	std::conditional_t<
		std::is_same<tag::point, typename attribute_traits<T>::tag_type>::value,
		detail::point_attribute_proxy<T>,
	std::conditional_t<
		std::is_same<tag::size, typename attribute_traits<T>::tag_type>::value,
		detail::size_attribute_proxy<T>,
	std::conditional_t<
		std::is_same<tag::point_pair, typename attribute_traits<T>::tag_type>::value,
		detail::point_pair_attribute_proxy<T>,
	std::conditional_t<
		std::is_same<tag::point_and_size, typename attribute_traits<T>::tag_type>::value,
		detail::point_and_size_attribute_proxy<T>,
		void
	>>>>{
	using parent_type = std::conditional_t<
			std::is_same<tag::point, typename attribute_traits<T>::tag_type>::value,
			detail::point_attribute_proxy<T>,
		std::conditional_t<
			std::is_same<tag::size, typename attribute_traits<T>::tag_type>::value,
			detail::size_attribute_proxy<T>,
		std::conditional_t<
			std::is_same<tag::point_pair, typename attribute_traits<T>::tag_type>::value,
			detail::point_pair_attribute_proxy<T>,
		std::conditional_t<
			std::is_same<tag::point_and_size, typename attribute_traits<T>::tag_type>::value,
			detail::point_and_size_attribute_proxy<T>,
			void
		>>>>;
public:
	constexpr attribute_proxy(const T& t)noexcept:parent_type{t}{}
	constexpr attribute_proxy(const attribute_proxy&)noexcept = default;
	constexpr attribute_proxy(attribute_proxy&&)noexcept = default;
};

template<typename T>
struct attribute_traits<detail::point_attribute_proxy<T>>{
	using tag_type = tag::point;
	using element_type = typename attribute_traits<T>::element_type;
	static constexpr element_type x(const detail::point_attribute_proxy<T>& xy)noexcept{return xy.x();}
	static constexpr element_type y(const detail::point_attribute_proxy<T>& xy)noexcept{return xy.y();}
	static constexpr detail::point_attribute_proxy<T> create(element_type x, element_type y)noexcept{return attribute_proxy<T>{attribute_traits<T>::create(x, y)};}
};

template<typename T>
struct attribute_traits<detail::size_attribute_proxy<T>>{
	using tag_type = tag::size;
	using element_type = typename attribute_traits<T>::element_type;
	static constexpr element_type w(const detail::size_attribute_proxy<T>& wh)noexcept{return wh.w();}
	static constexpr element_type h(const detail::size_attribute_proxy<T>& wh)noexcept{return wh.h();}
	static constexpr detail::size_attribute_proxy<T> create(element_type w, element_type h)noexcept{return attribute_proxy<T>{attribute_traits<T>::create(w, h)};}
};

template<typename T>
struct attribute_traits<detail::point_pair_attribute_proxy<T>>{
	using tag_type = tag::point_pair;
	using element_type = typename attribute_traits<T>::element_type;
	static constexpr auto _1(const detail::point_pair_attribute_proxy<T>& xyxy)noexcept->decltype(xyxy._1()){return xyxy._1();}
	static constexpr auto _2(const detail::point_pair_attribute_proxy<T>& xyxy)noexcept->decltype(xyxy._2()){return xyxy._2();}
	static constexpr element_type x1(const detail::point_pair_attribute_proxy<T>& xyxy)noexcept{return xyxy.x1();}
	static constexpr element_type y1(const detail::point_pair_attribute_proxy<T>& xyxy)noexcept{return xyxy.y1();}
	static constexpr element_type x2(const detail::point_pair_attribute_proxy<T>& xyxy)noexcept{return xyxy.x2();}
	static constexpr element_type y2(const detail::point_pair_attribute_proxy<T>& xyxy)noexcept{return xyxy.y2();}
	static constexpr detail::point_pair_attribute_proxy<T> create(element_type x1, element_type y1, element_type x2, element_type y2)noexcept{return attribute_proxy<T>{attribute_traits<T>::create(x1, y1, x2, y2)};}
};

template<typename T>
struct attribute_traits<detail::point_and_size_attribute_proxy<T>>{
	using tag_type = tag::point_and_size;
	using point_element_type = typename attribute_traits<T>::point_element_type;
	using size_element_type = typename attribute_traits<T>::size_element_type;
	static constexpr auto xy(const detail::point_and_size_attribute_proxy<T>& xywh)noexcept->decltype(xywh.xy()){return xywh.xy();}
	static constexpr auto wh(const detail::point_and_size_attribute_proxy<T>& xywh)noexcept->decltype(xywh.wh()){return xywh.wh();}
	static constexpr point_element_type x(const detail::point_and_size_attribute_proxy<T>& xywh)noexcept{return xywh.x();}
	static constexpr point_element_type y(const detail::point_and_size_attribute_proxy<T>& xywh)noexcept{return xywh.y();}
	static constexpr size_element_type w(const detail::point_and_size_attribute_proxy<T>& xywh)noexcept{return xywh.w();}
	static constexpr size_element_type h(const detail::point_and_size_attribute_proxy<T>& xywh)noexcept{return xywh.h();}
	static constexpr detail::point_and_size_attribute_proxy<T> create(point_element_type x, point_element_type y, size_element_type w, size_element_type h)noexcept{return attribute_proxy<T>{attribute_traits<T>::create(x, y, w, h)};}
};

template<typename T>
constexpr attribute_proxy<std::decay_t<T>> attribute(T&& t)noexcept{return {std::forward<T>(t)};}
template<typename T, typename U>
constexpr T attribute(U&& u)noexcept{return attribute(std::forward<U>(u));}

}
}

template<typename T, typename U, std::enable_if_t<std::is_same<will::two_dim::tag::point, typename will::two_dim::attribute_traits<std::decay_t<T>>::tag_type>::value && std::is_same<will::two_dim::tag::point, typename will::two_dim::attribute_traits<std::decay_t<U>>::tag_type>::value, std::nullptr_t> = nullptr>
constexpr will::two_dim::attribute_proxy<will::two_dim::wh<std::make_signed_t<std::common_type_t<typename will::two_dim::attribute_traits<std::decay_t<T>>::element_type, typename will::two_dim::attribute_traits<std::decay_t<T>>::element_type>>>> operator-(const T& lhs, const U& rhs)noexcept{
	using LHS = will::two_dim::attribute_traits<std::decay_t<T>>;
	using LHSET = typename LHS::element_type;
	using RHS = will::two_dim::attribute_traits<std::decay_t<U>>;
	using RHSET = typename RHS::element_type;
	using common_t = std::make_signed_t<std::common_type_t<LHSET, RHSET>>;
	return will::two_dim::wh<common_t>{static_cast<common_t>(LHS::x(lhs) - RHS::x(rhs)), static_cast<common_t>(LHS::y(lhs) - RHS::y(rhs))};
}
template<typename T, typename U, std::enable_if_t<std::is_same<will::two_dim::tag::size, typename will::two_dim::attribute_traits<std::decay_t<T>>::tag_type>::value && std::is_same<will::two_dim::tag::size, typename will::two_dim::attribute_traits<std::decay_t<U>>::tag_type>::value, std::nullptr_t> = nullptr>
constexpr will::two_dim::attribute_proxy<will::two_dim::wh<std::make_signed_t<std::common_type_t<typename will::two_dim::attribute_traits<std::decay_t<T>>::element_type, typename will::two_dim::attribute_traits<std::decay_t<T>>::element_type>>>> operator-(const T& lhs, const U& rhs)noexcept{
	using LHS = will::two_dim::attribute_traits<std::decay_t<T>>;
	using LHSET = typename LHS::element_type;
	using RHS = will::two_dim::attribute_traits<std::decay_t<U>>;
	using RHSET = typename RHS::element_type;
	using common_t = std::make_signed_t<std::common_type_t<LHSET, RHSET>>;
	return will::two_dim::wh<common_t>{static_cast<common_t>(LHS::w(lhs) - RHS::w(rhs)), static_cast<common_t>(LHS::h(lhs) - RHS::h(rhs))};
}
template<typename T, typename U, std::enable_if_t<std::is_same<will::two_dim::tag::point, typename will::two_dim::attribute_traits<std::decay_t<T>>::tag_type>::value && std::is_same<will::two_dim::tag::size, typename will::two_dim::attribute_traits<std::decay_t<U>>::tag_type>::value, std::nullptr_t> = nullptr>
constexpr will::two_dim::attribute_proxy<will::two_dim::xy<std::common_type_t<typename will::two_dim::attribute_traits<std::decay_t<T>>::element_type, typename will::two_dim::attribute_traits<std::decay_t<T>>::element_type>>> operator+(const T& lhs, const U& rhs)noexcept{
	using LHS = will::two_dim::attribute_traits<std::decay_t<T>>;
	using LHSET = typename LHS::element_type;
	using RHS = will::two_dim::attribute_traits<std::decay_t<U>>;
	using RHSET = typename RHS::element_type;
	using common_t = std::common_type_t<LHSET, RHSET>;
	return will::two_dim::xy<common_t>{static_cast<common_t>(LHS::x(lhs) + RHS::w(rhs)), static_cast<common_t>(LHS::y(lhs) + RHS::h(rhs))};
}
template<typename T, typename U, std::enable_if_t<std::is_same<will::two_dim::tag::size, typename will::two_dim::attribute_traits<std::decay_t<T>>::tag_type>::value && std::is_same<will::two_dim::tag::point, typename will::two_dim::attribute_traits<std::decay_t<U>>::tag_type>::value, std::nullptr_t> = nullptr>
constexpr will::two_dim::attribute_proxy<will::two_dim::xy<std::common_type_t<typename will::two_dim::attribute_traits<std::decay_t<T>>::element_type, typename will::two_dim::attribute_traits<std::decay_t<T>>::element_type>>> operator+(const T& lhs, const U& rhs)noexcept{
	using LHS = will::two_dim::attribute_traits<std::decay_t<T>>;
	using LHSET = typename LHS::element_type;
	using RHS = will::two_dim::attribute_traits<std::decay_t<U>>;
	using RHSET = typename RHS::element_type;
	using common_t = std::common_type_t<LHSET, RHSET>;
	return will::two_dim::xy<common_t>{static_cast<common_t>(LHS::w(lhs) + RHS::x(rhs)), static_cast<common_t>(LHS::h(lhs) + RHS::y(rhs))};
}

template<typename T, typename U, std::enable_if_t<std::is_arithmetic<std::decay_t<U>>::value>* = nullptr>
constexpr will::two_dim::xy<T>& operator*=(will::two_dim::xy<T>& lhs, U&& rhs)noexcept{lhs.x *= rhs; lhs.y *= rhs; return lhs;}
template<typename T, typename U, std::enable_if_t<std::is_arithmetic<std::decay_t<U>>::value>* = nullptr>
constexpr will::two_dim::xy<T> operator*(will::two_dim::xy<T> lhs, U&& rhs)noexcept{return lhs *= rhs;}
template<typename T, typename U, std::enable_if_t<std::is_arithmetic<std::decay_t<U>>::value>* = nullptr>
constexpr will::two_dim::xy<T> operator*(U&& lhs, will::two_dim::xy<T> rhs)noexcept{return rhs *= lhs;}
template<typename T, typename U, std::enable_if_t<std::is_arithmetic<std::decay_t<U>>::value>* = nullptr>
constexpr will::two_dim::xy<T>& operator/=(will::two_dim::xy<T>& lhs, U&& rhs)noexcept{lhs.x /= rhs; lhs.y /= rhs; return lhs;}
template<typename T, typename U, std::enable_if_t<std::is_arithmetic<std::decay_t<U>>::value>* = nullptr>
constexpr will::two_dim::xy<T> operator/(will::two_dim::xy<T> lhs, U&& rhs)noexcept{return lhs /= rhs;}
template<typename T, typename U, std::enable_if_t<std::is_arithmetic<std::decay_t<U>>::value>* = nullptr>
constexpr will::two_dim::xy<T> operator/(U&& lhs, will::two_dim::xy<T> rhs)noexcept{return rhs /= lhs;}
template<typename T, typename U, std::enable_if_t<std::is_arithmetic<std::decay_t<U>>::value>* = nullptr>
constexpr will::two_dim::wh<T>& operator*=(will::two_dim::wh<T>& lhs, U&& rhs)noexcept{lhs.w *= rhs; lhs.h *= rhs; return lhs;}
template<typename T, typename U, std::enable_if_t<std::is_arithmetic<std::decay_t<U>>::value>* = nullptr>
constexpr will::two_dim::wh<T> operator*(will::two_dim::wh<T> lhs, U&& rhs)noexcept{return lhs *= rhs;}
template<typename T, typename U, std::enable_if_t<std::is_arithmetic<std::decay_t<U>>::value>* = nullptr>
constexpr will::two_dim::wh<T> operator*(U&& lhs, will::two_dim::wh<T> rhs)noexcept{return rhs *= lhs;}
template<typename T, typename U, std::enable_if_t<std::is_arithmetic<std::decay_t<U>>::value>* = nullptr>
constexpr will::two_dim::wh<T>& operator/=(will::two_dim::wh<T>& lhs, U&& rhs)noexcept{lhs.w /= rhs; lhs.h /= rhs; return lhs;}
template<typename T, typename U, std::enable_if_t<std::is_arithmetic<std::decay_t<U>>::value>* = nullptr>
constexpr will::two_dim::wh<T> operator/(will::two_dim::wh<T> lhs, U&& rhs)noexcept{return lhs /= rhs;}
template<typename T, typename U, std::enable_if_t<std::is_arithmetic<std::decay_t<U>>::value>* = nullptr>
constexpr will::two_dim::wh<T> operator/(U&& lhs, will::two_dim::wh<T> rhs)noexcept{return rhs /= lhs;}
template<typename T, typename U, std::enable_if_t<std::is_arithmetic<std::decay_t<U>>::value>* = nullptr>
constexpr will::two_dim::xyxy<T>& operator*=(will::two_dim::xyxy<T>& lhs, U&& rhs)noexcept{lhs._1 *= rhs; lhs._2 *= rhs; return lhs;}
template<typename T, typename U, std::enable_if_t<std::is_arithmetic<std::decay_t<U>>::value>* = nullptr>
constexpr will::two_dim::xyxy<T> operator*(will::two_dim::xyxy<T> lhs, U&& rhs)noexcept{return lhs *= rhs;}
template<typename T, typename U, std::enable_if_t<std::is_arithmetic<std::decay_t<U>>::value>* = nullptr>
constexpr will::two_dim::xyxy<T> operator*(U&& lhs, will::two_dim::xyxy<T> rhs)noexcept{return rhs *= lhs;}
template<typename T, typename U, std::enable_if_t<std::is_arithmetic<std::decay_t<U>>::value>* = nullptr>
constexpr will::two_dim::xyxy<T>& operator/=(will::two_dim::xyxy<T>& lhs, U&& rhs)noexcept{lhs._1 /= rhs; lhs._2 /= rhs; return lhs;}
template<typename T, typename U, std::enable_if_t<std::is_arithmetic<std::decay_t<U>>::value>* = nullptr>
constexpr will::two_dim::xyxy<T> operator/(will::two_dim::xyxy<T> lhs, U&& rhs)noexcept{return lhs /= rhs;}
template<typename T, typename U, std::enable_if_t<std::is_arithmetic<std::decay_t<U>>::value>* = nullptr>
constexpr will::two_dim::xyxy<T> operator/(U&& lhs, will::two_dim::xyxy<T> rhs)noexcept{return rhs /= lhs;}
template<typename T, typename U, typename V, std::enable_if_t<std::is_arithmetic<std::decay_t<V>>::value>* = nullptr>
constexpr will::two_dim::xywh<T, U>& operator*=(will::two_dim::xywh<T, U>& lhs, V&& rhs)noexcept{lhs.xy *= rhs; lhs.wh *= rhs; return lhs;}
template<typename T, typename U, typename V, std::enable_if_t<std::is_arithmetic<std::decay_t<V>>::value>* = nullptr>
constexpr will::two_dim::xywh<T, U> operator*(will::two_dim::xywh<T, U> lhs, V&& rhs)noexcept{return lhs *= rhs;}
template<typename T, typename U, typename V, std::enable_if_t<std::is_arithmetic<std::decay_t<V>>::value>* = nullptr>
constexpr will::two_dim::xywh<T, U> operator*(V&& lhs, will::two_dim::xywh<T, U> rhs)noexcept{return rhs *= lhs;}
template<typename T, typename U, typename V, std::enable_if_t<std::is_arithmetic<std::decay_t<V>>::value>* = nullptr>
constexpr will::two_dim::xywh<T, U>& operator/=(will::two_dim::xywh<T, U>& lhs, V&& rhs)noexcept{lhs.xy /= rhs; lhs.wh /= rhs; return lhs;}
template<typename T, typename U, typename V, std::enable_if_t<std::is_arithmetic<std::decay_t<V>>::value>* = nullptr>
constexpr will::two_dim::xywh<T, U> operator/(will::two_dim::xywh<T, U> lhs, V&& rhs)noexcept{return lhs /= rhs;}
template<typename T, typename U, typename V, std::enable_if_t<std::is_arithmetic<std::decay_t<V>>::value>* = nullptr>
constexpr will::two_dim::xywh<T, U> operator/(V&& lhs, will::two_dim::xywh<T, U> rhs)noexcept{return rhs /= lhs;}